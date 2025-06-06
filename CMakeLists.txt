cmake_minimum_required(VERSION 3.15)
project(Radon)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Source)

set(URHO3D_HOME "../rbfx")
set(URHO3D_BUILD "${CMAKE_BINARY_DIR}/rbfx-build")
add_subdirectory(${URHO3D_HOME} ${URHO3D_BUILD})

add_executable(Radon RadonMain.cpp)
set_target_properties(Radon PROPERTIES WIN32_EXECUTABLE TRUE)

add_custom_command(TARGET Radon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets"
        "${CMAKE_BINARY_DIR}/Assets"
)

target_link_libraries(Radon
        PRIVATE
        Urho3D
        RadonEngine
        RadonGame
        RadonClient
        RadonShared
        dbghelp)
