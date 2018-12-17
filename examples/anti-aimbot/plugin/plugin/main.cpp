#include "main.h"
#include "version.h"

#include <iostream>
#include <thread>

#include <sdk/amx/amx.h>
#include <sdk/plugincommon.h>
#include <cassert>
#include "natives/classifier.hpp"
#include "iscript.hpp"

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

    classifier::Load();
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
        {"submit_vector", classifier::natives::submit_vector},
        {0, 0}
    };

    iscript::AmxLoad(amx);
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{	
    iscript::AmxUnload(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
    classifier::ProcessTick();
}