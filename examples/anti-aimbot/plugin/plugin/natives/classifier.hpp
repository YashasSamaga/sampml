#include "main.h"

namespace classifier {
    extern void ProcessTick();
    extern void Load();
    namespace natives {
        extern cell AMX_NATIVE_CALL submit_vector(AMX * amx, cell* params);
    }
}
