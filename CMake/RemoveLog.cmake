add_custom_target(remove_log ALL
        COMMENT "Removing old ${CMAKE_BINARY_DIR}/bin/Radon.log file"
        COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_BINARY_DIR}/bin/Radon.log"
)