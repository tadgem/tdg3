function(add_shaders DEPENDENCY_TARGET SHADERS_DIR)
    set(SKL_SHDC "${TOOLS_ROOT}/bin/win32/sokol-shdc.exe")
    file(GLOB_RECURSE GLSL_SOURCE_FILES
            "${SHADERS_DIR}/*.glsl"
    )

    foreach (GLSL ${GLSL_SOURCE_FILES})
        get_filename_component(FILE_NAME ${GLSL} NAME)
        set(SOKOL_SHADER_HEADER "${SHADERS_DIR}/${FILE_NAME}.h")
        add_custom_command(
                OUTPUT ${SOKOL_SHADER_HEADER}
                COMMENT "Compile Shader: ${SKL_SHDC} -l glsl430 -i ${GLSL} -o ${SOKOL_SHADER_HEADER} -f sokol"
                COMMAND ${SKL_SHDC} -l glsl430 -i ${GLSL} -o ${SOKOL_SHADER_HEADER} -f sokol
                DEPENDS ${GLSL})
        list(APPEND SOKOL_SHADER_HEADERS ${SOKOL_SHADER_HEADER})
    endforeach (GLSL)

    add_custom_target(
            Shaders
            DEPENDS ${SOKOL_SHADER_HEADERS}
    )

    add_dependencies(${DEPENDENCY_TARGET} Shaders)

    set(INTERNAL_SHADERS_DIR_HACK ${SHADERS_DIR} CACHE INTERNAL "")
endfunction()

function(add_shaders_dependency DEPENDENCY_TARGET)
    add_dependencies(${DEPENDENCY_TARGET} Shaders)

    add_custom_command(TARGET ${DEPENDENCY_TARGET}
            DEPENDS Shaders
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${INTERNAL_SHADERS_DIR_HACK}"
            "${CMAKE_CURRENT_BINARY_DIR}/shaders/")
endfunction()