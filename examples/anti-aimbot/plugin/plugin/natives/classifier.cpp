#include "main.h"

#include "classifier.hpp"

#include <sampml/svm_classifier.hpp>
#include "transform.hpp"

std::array<transformer, MAX_PLAYERS> transformers;

namespace natives {
    cell AMX_NATIVE_CALL test_vector(AMX * amx, cell* params)
    {
        logprintf("test_vector invoked");
        cell playerid = params[1];
        cell *data;
        amx_GetAddr(amx, params[2], &data);

        using sample_type = output_vector;
        static sampml::trainer::svm_classifier<sample_type> skin_aimbot_classifier;

        static bool loaded = false;
        if (loaded == false) {
            skin_aimbot_classifier.deserialize("models/classifier.dat");
            loaded = true;
        }

        input_vector vector;
        for (int i = 0; i < 53; i++) {
            switch (i) {
            case 0:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 30:
            case 31:
            case 32:
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
            case 47:
            case 49:
            case 51:
            case 52:
                vector(i) = (data[i]);
                break;
            default:
                vector(i) = amx_ctof(data[i]);
            }
        }

        transformers[playerid].submit(vector);
        if (transformers[playerid].pool.size()) {
            logprintf("obtained a transformed vector");
            auto sample = transformers[playerid].pool.back();
            transformers[playerid].pool.pop_back();

            float prob = skin_aimbot_classifier.test(sample);
            cell *ref;
            amx_GetAddr(amx, params[3], &ref);
            logprintf("prob: %f", prob);
            *ref = amx_ftoc(prob);
            return true;
        }
        return false;
    }
}