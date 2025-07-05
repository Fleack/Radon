cmake_minimum_required(VERSION 3.16)
project(Radon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(RADON_DEBUG)
endif ()
add_compile_options(
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-DNDEBUG>
)

set(URHO3D_HOME "${CMAKE_SOURCE_DIR}/../rbfx")
set(URHO3D_BUILD "${CMAKE_BINARY_DIR}/rbfx-build")
add_subdirectory(${URHO3D_HOME} ${URHO3D_BUILD})

add_subdirectory(Source)

include_directories(${CMAKE_SOURCE_DIR}/Plugins)

set(APP_PLUGIN_LIST
        Plugin.Radon.Player
)
