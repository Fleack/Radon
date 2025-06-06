file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS *.cpp *.h)

add_library(RadonShared STATIC ${GAME_SRC})
target_include_directories(RadonShared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RadonShared PUBLIC Urho3D)
