
#ifndef ANTIAIMBOT_MAIN_H_INCLUDED
#define ANTIAIMBOT_MAIN_H_INCLUDED

#include <sdk/amx/amx.h>
#include <sdk/plugincommon.h>

#include <string_view>
#include <tools/config_reader.hpp>

constexpr std::string_view config_file = "plugins/anti-aimbot/config.cfg";

typedef void (*logprintf_t)(const char*, ...);
extern logprintf_t logprintf;

extern tools::config_reader config_reader;

#define BYTES_PER_CELL sizeof(cell)

#define warn_if(x,...) if(x) logprintf(__VA_ARGS__), 0
#define error_if(x,...) if(x) return logprintf(__VA_ARGS__), 0

#define check_params(n) (params[0] == n*BYTES_PER_CELL)
#define check_params_min(n) (params[0] >= n*BYTES_PER_CELL)
#define get_params_count() (params[0]/(BYTES_PER_CELL))

#endif /* ANTIAIMBOT_MAIN_H_INCLUDED */