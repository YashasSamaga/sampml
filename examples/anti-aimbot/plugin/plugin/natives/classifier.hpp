#include "main.h"

namespace classifier {
    extern void ProcessTick();

    namespace natives {
        extern cell AMX_NATIVE_CALL test_vector(AMX * amx, cell* params);
    }
}
