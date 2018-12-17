#include "main.h"

#include <dlib/dnn.h>
#include <sampml/svm_classifier.hpp>
#include "tools/fixed_thread_pool.hpp"

#include "iscript.hpp"
#include "classifier.hpp"
#include "transform.hpp"
#include "dnn.hpp"

namespace classifier {
    using sample_type = output_vector;

    double test_vector_svm(const sample_type& sample) {
        static thread_local sampml::trainer::svm_classifier<sample_type> svm;
        static thread_local bool loaded = false;
        if (loaded == false) {
            svm.deserialize("models/svm_classifier.dat");
            loaded = true;
        }
        return svm.test(sample);
    }

    double test_vector_dnn(const sample_type& sample) {
        static thread_local aa_network_type net;
        static thread_local bool loaded = false;
        if (loaded == false) {
            dlib::deserialize("models/dnn_classifier.dat") >> net;
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
        float probabilities[2];
        queue_item_tag_t tag;
    };

    struct process_functor_t {
        void operator()(input_queue_item_t& input, output_queue_item_t& output) {
            auto start = std::chrono::steady_clock::now();
            sample_type& sample = input.sample;
            output.probabilities[0] = test_vector_svm(sample);
            auto svm = std::chrono::steady_clock::now();
            output.probabilities[1] = test_vector_dnn(sample);
            output.tag = input.tag;
            auto dnn = std::chrono::steady_clock::now();
            std::cout << "Timings: " << std::chrono::duration_cast<std::chrono::microseconds>(svm - start).count() << " " << std::chrono::duration_cast<std::chrono::microseconds>(dnn - svm).count();
        }
    };

    fixed_thread_pool<input_queue_item_t, output_queue_item_t, process_functor_t> pool;
    void Load() {
        pool.start(2);
    }

    void ProcessTick() {
        std::vector<output_queue_item_t> results;
        pool.deqeue_all(results);

        for (auto&& item : results) {
            AMX* amx = item.tag.amx;
            if (iscript::IsValidAmx(amx)) {
                int cb_idx = -1;
                if (amx_FindPublic(amx, "OnPlayerSuspectedForAimbot", &cb_idx) == AMX_ERR_NONE) {
                    // OnPlayerSuspectedForAimbot(playerid, Float:probablities[2])
                    cell probablities[2] = { amx_ftoc(item.probabilities[0]), amx_ftoc(item.probabilities[1]) };
                    cell amx_addr, *phys_addr;
                    amx_Allot(amx, sizeof(probablities) / sizeof(cell), &amx_addr, &phys_addr);
                    memcpy(phys_addr, probablities, sizeof(probablities));
                    amx_Push(amx, amx_addr);
                    amx_Push(amx, item.tag.playerid);
                    amx_Exec(amx, NULL, cb_idx);
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
            cell playerid = params[1];
            cell *data;
            amx_GetAddr(amx, params[2], &data);

            auto vector = pawn_array_to_vector(data);

            static std::array<transformer, MAX_PLAYERS> transformers;

            transformers[playerid].submit(vector);
            if (transformers[playerid].pool.size()) {
                input_queue_item_t item;
                item.sample = transformers[playerid].pool.back();
                item.tag.amx = amx;
                item.tag.playerid = playerid;
                transformers[playerid].pool.pop_back();
                pool.enqueue(item);
                return true;
            }
            return false;
        }
    }
}