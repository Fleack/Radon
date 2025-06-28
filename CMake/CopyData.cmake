add_custom_target(copy_data ALL
        COMMENT "Copying ${CMAKE_SOURCE_DIR}/Data directory to ${CMAKE_BINARY_DIR}/bin/Data"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Data"
        "${CMAKE_BINARY_DIR}/bin/Data"
)