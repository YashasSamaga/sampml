#include "main.h"
#include "version.h"

#include <sampml/svm_classifier.hpp>

#include <sdk/amx/amx.h>
#include <sdk/plugincommon.h>

#include "natives/classifier.hpp"

extern void *pAMXFunctions;
logprintf_t logprintf;

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{	
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	 
	logprintf("***********************************************************");
	logprintf("  AntiAimbot v%d.%d.%d loaded", PLUGIN_MAJOR_VERSION, PLUGIN_MINOR_VERSION, PLUGIN_PATCH_VERSION);
	logprintf("  Version Key: %h", PLUGIN_VERSION_KEY);
	logprintf("***********************************************************");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("*** AntiAimbot v%d.%d.%d unloaded ***", PLUGIN_MAJOR_VERSION, PLUGIN_MINOR_VERSION, PLUGIN_PATCH_VERSION);
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{	
    AMX_NATIVE_INFO PluginNatives[] =
    {
        {"test_vector", natives::test_vector},
        {0, 0}
    };
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{	
	return AMX_ERR_NONE;
}