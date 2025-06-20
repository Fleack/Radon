add_custom_command(TARGET RadonGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_BINARY_DIR}/bin/Radon.log"
)