cmake_minimum_required(VERSION 3.16)
project (Plugin.Radon.Player)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file (GLOB_RECURSE SOURCE_FILES *.hpp *.cpp)
add_plugin (${PROJECT_NAME} "${SOURCE_FILES}")

rbfx_configure_cmake_props()
