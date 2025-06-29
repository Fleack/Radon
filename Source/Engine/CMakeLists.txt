file(GLOB_RECURSE ENGINE_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(RadonEngine STATIC ${ENGINE_SOURCES})

target_compile_features(RadonEngine PUBLIC cxx_std_23)
target_include_directories(RadonEngine
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
)

target_link_libraries(RadonEngine
        PUBLIC
        Urho3D
)
