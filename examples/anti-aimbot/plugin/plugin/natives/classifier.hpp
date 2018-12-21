#include "main.h"

namespace classifier {
    constexpr std::string_view svm_model_default = "plugins/anti-aimbot/models/svm_classifier.dat";
    constexpr std::string_view rf_model_default = "plugins/anti-aimbot/models/rf_classifier.dat";
    constexpr std::string_view dnn_model_default = "plugins/anti-aimbot/models/dnn_classifier.dat";
    constexpr int num_detector_threads_default = 2;

    extern void ProcessTick();
    extern void Load();
    namespace natives {
        extern cell AMX_NATIVE_CALL submit_vector(AMX * amx, cell* params);
    }
}
