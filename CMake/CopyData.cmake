add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Data"
        "${CMAKE_BINARY_DIR}/bin/Data"
        COMMENT "Copying Data directory to bin/Data/"
)