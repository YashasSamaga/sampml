#include <filesystem>

#include "main.h"
#include "classifier.hpp"

#include <sampml/svm_classifier.hpp>
#include <sampml/random_forest.hpp>
#include "dnn.hpp"

#include "transform.hpp"

#include "tools/fixed_thread_pool.hpp"
#include "iscript.hpp"

namespace classifier {
    using sample_type = output_vector;

    double test_vector_svm(const sample_type& sample) {
        static thread_local sampml::trainer::svm_classifier<sample_type> svm;
        static thread_local bool loaded = false;
        if (loaded == false) {
            std::string model;
            config_reader.get("svm_model_file", model, std::string(svm_model_default));
            svm.deserialize(model);
            loaded = true;
        }
        return svm.test(sample);
    }

    double test_vector_rf(const sample_type& sample) {
        static thread_local sampml::trainer::random_forest<sample_type> rf;
        static thread_local bool loaded = false;
        if (loaded == false) {
            std::string model;
            config_reader.get("rf_model_file", model, std::string(rf_model_default));
            rf.deserialize(model);
            loaded = true;
        }
        return std::clamp(rf.test(sample), 0.0, 1.0);
    }

    double test_vector_dnn(const sample_type& sample) {
        static thread_local aa_network_type net;
        static thread_local bool loaded = false;
        if (loaded == false) {
            std::string model;
            config_reader.get("dnn_model_file", model, std::string(dnn_model_default));
            dlib::deserialize(model) >> net;
            loaded = true;
        }
        return net(sample);
    }

    struct queue_item_tag_t {
        int playerid;
        AMX *amx;
    };

    struct input_queue_item_t {
        sample_type sample;
        queue_item_tag_t tag;
    };

    struct output_queue_item_t {
        float probabilities[3];
        int time[3];
        queue_item_tag_t tag;
    };

    struct process_functor_t {
        void operator()(input_queue_item_t& input, output_queue_item_t& output) {
            auto start = std::chrono::steady_clock::now();
            sample_type& sample = input.sample;
            output.probabilities[0] = test_vector_rf(sample);
            auto rf = std::chrono::steady_clock::now();
            output.probabilities[1] = test_vector_svm(sample);
            auto svm = std::chrono::steady_clock::now();
            output.probabilities[2] = test_vector_dnn(sample);
            output.tag = input.tag;
            auto dnn = std::chrono::steady_clock::now();
            output.time[0] = std::chrono::duration_cast<std::chrono::microseconds>(rf - start).count();
            output.time[1] = std::chrono::duration_cast<std::chrono::microseconds>(svm - rf).count();
            output.time[2] = std::chrono::duration_cast<std::chrono::microseconds>(dnn - svm).count();
        }
    };

    tools::fixed_thread_pool<input_queue_item_t, output_queue_item_t, process_functor_t> pool;
    void Load() {
        int num;
        config_reader.get("thread_pool_size", num, num_detector_threads_default);
        pool.start(num);

        std::string svm_model_file;
        config_reader.get("svm_model_file", svm_model_file, std::string(svm_model_default));
        assert(std::filesystem::exists(svm_model_file));

        std::string rf_model_file;
        config_reader.get("rf_model_file", rf_model_file, std::string(rf_model_default));
        assert(std::filesystem::exists(rf_model_file));

        std::string dnn_model_file;
        config_reader.get("dnn_model_file", dnn_model_file, std::string(dnn_model_default));
        assert(std::filesystem::exists(dnn_model_file));      
    }

    void ProcessTick() {
        std::vector<output_queue_item_t> results;
        pool.deqeue_all(results);

        for (auto&& item : results) {
            AMX* amx = item.tag.amx;
            if (iscript::IsValidAmx(amx)) {
                int cb_idx = -1;
                if (amx_FindPublic(amx, "OnPlayerSuspectedForAimbot", &cb_idx) == AMX_ERR_NONE) {
                    // OnPlayerSuspectedForAimbot(playerid, Float:probabilities[3], times[3])
                    cell probabilities[3] = { amx_ftoc(item.probabilities[0]), amx_ftoc(item.probabilities[1]), amx_ftoc(item.probabilities[2]) };
                    cell time[3] = { item.time[0], item.time[1], item.time[2] };

                    cell amx_addr_time, *phys_addr_time;
                    amx_Allot(amx, sizeof(time) / sizeof(cell), &amx_addr_time, &phys_addr_time);
                    memcpy(phys_addr_time, time, sizeof(time));
                    amx_Push(amx, amx_addr_time);

                    cell amx_addr_probabilities, *phys_addr_probabilities;
                    amx_Allot(amx, sizeof(probabilities) / sizeof(cell), &amx_addr_probabilities, &phys_addr_probabilities);
                    memcpy(phys_addr_probabilities, probabilities, sizeof(probabilities));
                    amx_Push(amx, amx_addr_probabilities);
                    
                    amx_Push(amx, item.tag.playerid);
                    amx_Exec(amx, NULL, cb_idx);
                    amx_Release(amx, amx_addr_time);
                }
            }
        }
    }

    namespace natives {
        static input_vector pawn_array_to_vector(cell data[]) {
            input_vector vector;
            for (int i = 0; i < input_vector::NR; i++) {
                switch (i) {
                case bHit:
                case iShooterCameraMode:
                case iShooterState:
                case iShooterSpecialAction:
                case bShooterInVehicle:
                case bShooterSurfingVehicle:
                case bShooterSurfingObject:
                case iShooterWeaponID:
                case iShooterSkinID:
                case iShooterID:
                case iVictimState:
                case iVictimSpecialAction:
                case bVictimInVehicle:
                case bVictimSurfingVehicle:
                case bVictimSurfingObject:
                case iVictimWeaponID:
                case iVictimSkinID:
                case iVictimID:
                case iHitType:
                case iShooterPing:
                case iVictimPing:
                case iSecond:
                case iTick:
                    vector(i) = (data[i]);
                    break;
                default:
                    vector(i) = amx_ctof(data[i]);
                }
            }
            return vector;
        }

        cell AMX_NATIVE_CALL submit_vector(AMX * amx, cell* params)
        {
            error_if(!check_params(2), "[AntiAimbot] classifier>> submit_vector: expected 2 parameters but found %d parameters.", get_params_count());

            cell playerid = params[1];
            cell *data;
            amx_GetAddr(amx, params[2], &data);

            auto vector = pawn_array_to_vector(data);

            static std::vector<transformer> transformers;
            if (playerid == samp::INVALID_PLAYER_ID || playerid < 0)
                return -1;

            if (static_cast<std::size_t>(playerid) >= transformers.size())
                transformers.resize(playerid + 1);

            transformers[playerid].submit(vector);
            if (transformers[playerid].pool.size()) {
                input_queue_item_t item;
                item.sample = transformers[playerid].pool.back();
                item.tag.amx = amx;
                item.tag.playerid = playerid;
                transformers[playerid].pool.pop_back();
                pool.enqueue(item);
                return 1;
            }
            return 0;
        }
    }
}
