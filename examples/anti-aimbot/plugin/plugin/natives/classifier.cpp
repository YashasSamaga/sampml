#include "main.h"

#include <dlib/dnn.h>
#include "classifier.hpp"

#include <sampml/svm_classifier.hpp>
#include "transform.hpp"

 using sample_type = output_vector;

 double test_vector_svm(const output_vector& sample) {
    static sampml::trainer::svm_classifier<sample_type> svm;

    static bool loaded = false;
    if (loaded == false) {
        svm.deserialize("models/svm_classifier.dat");
        loaded = true;
    }
    return svm.test(sample);
 }

 double test_vector_dnn(const output_vector& sample) {
    using net_type = dlib::loss_multiclass_log<
                    dlib::fc<2,
                    dlib::prelu<dlib::fc<8,
                    dlib::prelu<dlib::fc<12,
                    dlib::input<output_vector> 
                    >>>>>>;

    static net_type net;

    static bool loaded = false;
    if (loaded == false) {
            dlib::deserialize("models/dnn_classifier.dat") >> net;
        loaded = true;
    }
    return net(sample);
 }

namespace natives {
    cell AMX_NATIVE_CALL test_vector(AMX * amx, cell* params)
    {
        cell playerid = params[1];
        cell *data;
        amx_GetAddr(amx, params[2], &data);

        input_vector vector;
        for (int i = 0; i < 54; i++) {
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
            case 38:
            case 48:
            case 50:
            case 52:
            case 53:
                vector(i) = (data[i]);
                break;
            default:
                vector(i) = amx_ctof(data[i]);
            }
        }

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