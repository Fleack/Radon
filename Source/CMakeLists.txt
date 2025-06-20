file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(RadonGame ${GAME_SOURCES})

target_include_directories(
        RadonGame
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
)

target_link_libraries(
        RadonGame
        PRIVATE
        Urho3D
)

set_target_properties(
        RadonGame
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        WIN32_EXECUTABLE TRUE
)

include(CopyData)
include(RemoveLog)
