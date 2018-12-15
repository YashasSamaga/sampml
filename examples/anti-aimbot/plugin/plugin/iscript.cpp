#include "main.h"

#include <set>

namespace iscript {
    static std::set<AMX*> instances;

    void AmxLoad(AMX *amx) {
        instances.insert(amx);
    }

    void AmxUnload(AMX *amx) {
        instances.erase(amx);
    }

    bool IsValidAmx(AMX *amx) {
        return (instances.find(amx) != instances.end());
    }
}