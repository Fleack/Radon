file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS *.cpp *.h)

add_library(RadonClient STATIC ${GAME_SRC})
target_include_directories(RadonClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RadonClient PUBLIC Urho3D)
