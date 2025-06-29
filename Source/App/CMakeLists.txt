file(GLOB_RECURSE APP_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_executable(RadonApp ${APP_SOURCES})

target_compile_features(RadonApp PRIVATE cxx_std_23)
target_include_directories(RadonApp
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
)

target_link_libraries(RadonApp
        PRIVATE
        RadonEngine
        RadonGameLib
)

include(${CMAKE_SOURCE_DIR}/CMake/CopyData.cmake)
include(${CMAKE_SOURCE_DIR}/CMake/RemoveLog.cmake)
add_dependencies(RadonApp copy_data remove_log)

set_target_properties(RadonApp PROPERTIES
        OUTPUT_NAME "Radon"
        WIN32_EXECUTABLE TRUE
)
