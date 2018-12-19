include(AMXConfig)
include(AddSAMPPlugin)

set(SAMP_SDK_ROOT "${LIBRARY_SOURCE_DIR}/sdk")
find_package(SAMPSDK REQUIRED)

file(GLOB PLUGIN_SOURCE_FILES
    ${PLUGIN_SOURCE_DIR}/natives/*.cpp
    ${PLUGIN_SOURCE_DIR}/*.cpp
)

add_samp_plugin(plugin
	${SAMP_SDK_ROOT}/amxplugin.cpp
	${SAMP_SDK_ROOT}/amx/getch.c
	${PLUGIN_SOURCE_FILES}
	${LIBRARY_SOURCE_DIR}/dlib/all/source.cpp
	exports.def
)

target_include_directories(plugin
	PRIVATE
	${LIBRARY_SOURCE_DIR}
	${PLUGIN_SOURCE_DIR}
)


target_compile_definitions(plugin
	PRIVATE	
	PLUGIN_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
	PLUGIN_MINOR_VERSION=${PROJECT_VERSION_MINOR}
	PLUGIN_PATCH_VERSION=${PROJECT_VERSION_PATCH}
	DLIB_NO_GUI_SUPPORT
)

set_target_properties(plugin PROPERTIES 
	OUTPUT_NAME "AntiAimbot"
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
	FIND_LIBRARY_USE_LIB64_PATHS OFF
)


if(CMAKE_COMPILER_IS_GNUXX)
	target_link_libraries(plugin PRIVATE m)
	target_compile_options(plugin PRIVATE -fvisibility=hidden -std=c++17)
endif()

if(MSVC)
	target_compile_definitions(plugin
		PRIVATE	
		_CRT_SECURE_NO_WARNINGS
		NOMINMAX
	)
	
	target_compile_options(plugin PRIVATE /std:c++17)
endif()
