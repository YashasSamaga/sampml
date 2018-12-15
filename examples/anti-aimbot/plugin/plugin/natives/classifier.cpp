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

    struct process_functor_t {
        void operator()(input_queue_item_t& input, output_queue_item_t& output) {
            sample_type& sample = input.sample;
            output.probabilities[0] = test_vector_svm(sample);
            output.probabilities[1] = test_vector_dnn(sample);
            output.tag = input.tag;
        }
    };

    fixed_thread_pool<input_queue_item_t, output_queue_item_t, process_functor_t> pool(2);
    void ProcessTick() {
        std::vector<output_queue_item_t> results;
        pool.deqeue_all(results);

        for (auto&& item : results) {
            AMX* amx = item.tag.amx;
            if (iscript::IsValidAmx(amx)) {
                int cb_idx = -1;
                if (amx_FindPublic(amx, "OnPlayerSuspectedForAimbot", &cb_idx) != AMX_ERR_NONE || cb_idx < 0) {
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

        cell AMX_NATIVE_CALL test_vector(AMX * amx, cell* params)
        {
            cell playerid = params[1];
            cell *data;
            amx_GetAddr(amx, params[2], &data);

            auto vector = pawn_array_to_vector(data);

            static std::array<transformer, MAX_PLAYERS> transformers;

            transformers[playerid].submit(vector);
            if (transformers[playerid].pool.size()) {
                auto sample = transformers[playerid].pool.back();
                transformers[playerid].pool.pop_back();

                float prob_svm = test_vector_svm(sample),
                    prob_dnn = test_vector_dnn(sample);

                cell *prob_arr;
                amx_GetAddr(amx, params[3], &prob_arr);
                prob_arr[0] = amx_ftoc(prob_svm);
                prob_arr[1] = amx_ftoc(prob_dnn);
                return true;
            }
            return false;
        }
    }
}