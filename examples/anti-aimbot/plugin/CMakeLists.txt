cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(AntiAimbot VERSION 1.0.0 LANGUAGES C CXX)

# cmake directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# project directories
set(LIBRARY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib)

set(PLUGIN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/plugin)
set(PLUGIN_BUILD_DIR ${CMAKE_BINARY_DIR}/plugin)

set(PAWN_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/includes)

# cmake-modules https://github.com/Zeex/cmake-modules
list(APPEND CMAKE_MODULE_PATH ${LIBRARY_SOURCE_DIR}/cmake-modules)

# options


# plugin code
add_subdirectory(${PLUGIN_SOURCE_DIR} ${PLUGIN_BUILD_DIR})
