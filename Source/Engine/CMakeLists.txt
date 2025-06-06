file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS *.cpp *.hpp)

add_library(RadonEngine STATIC ${ENGINE_SRC})
target_include_directories(RadonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RadonEngine PUBLIC Urho3D)
