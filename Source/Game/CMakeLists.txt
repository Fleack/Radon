file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS *.cpp *.h)

add_library(RadonGame STATIC ${GAME_SRC})
target_include_directories(RadonGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RadonGame PUBLIC Urho3D)
