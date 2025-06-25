file(GLOB_RECURSE ALL_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(REMOVE_ITEM LIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
)

add_library(RadonGameLib STATIC
        ${ALL_SOURCES}
)

target_include_directories(RadonGameLib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RadonGameLib
        PUBLIC
        Urho3D
)

add_executable(RadonGame
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
)

target_link_libraries(RadonGame
        PRIVATE
        RadonGameLib
)

set_target_properties(RadonGame PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        WIN32_EXECUTABLE TRUE
)

include(CopyData)
add_dependencies(RadonGame copy_data)

include(RemoveLog)
