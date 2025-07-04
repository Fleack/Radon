file(GLOB_RECURSE GAME_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(RadonGameLib STATIC ${GAME_SOURCES})

target_compile_features(RadonGameLib PUBLIC cxx_std_23)
target_include_directories(RadonGameLib
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
        ${CMAKE_SOURCE_DIR}/Plugins
)

target_link_libraries(RadonGameLib
        PUBLIC
        RadonEngine
)
