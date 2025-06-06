file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS *.cpp *.h)

add_library(RadonServer STATIC ${GAME_SRC})
target_include_directories(RadonServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RadonServer PUBLIC Urho3D)
