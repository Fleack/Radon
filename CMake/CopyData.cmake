add_custom_command(TARGET RadonGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/bin/Data
)