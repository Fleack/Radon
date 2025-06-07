cmake_minimum_required(VERSION 3.15)
project(Radon)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(URHO3D_HOME "../rbfx")
set(URHO3D_BUILD "${CMAKE_BINARY_DIR}/rbfx-build")
add_subdirectory(${URHO3D_HOME} ${URHO3D_BUILD})

add_subdirectory(Source)

include_directories (${CMAKE_SOURCE_DIR}/Plugins)
add_subdirectory (${CMAKE_SOURCE_DIR}/Plugins/Core.SamplePlugin)

add_executable(Radon RadonMain.cpp)
set_target_properties(Radon PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(Radon
        PRIVATE
        Urho3D
        RadonEngine
        RadonGame
        RadonClient
        RadonShared
        dbghelp)
