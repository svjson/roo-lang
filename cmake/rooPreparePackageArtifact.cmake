if(NOT DEFINED ROO_ARTIFACT_NATIVE_LIBRARY)
  message(FATAL_ERROR "ROO_ARTIFACT_NATIVE_LIBRARY is required.")
endif()

if(NOT EXISTS "${ROO_ARTIFACT_NATIVE_LIBRARY}")
  message(FATAL_ERROR
    "Package artifact native library does not exist: ${ROO_ARTIFACT_NATIVE_LIBRARY}"
  )
endif()

if(APPLE)
  execute_process(
    COMMAND install_name_tool
            -rpath
            "${ROO_ARTIFACT_NATIVE_OLD_RPATH}"
            "${ROO_ARTIFACT_NATIVE_RPATH}"
            "${ROO_ARTIFACT_NATIVE_LIBRARY}"
    RESULT_VARIABLE prepare_result
    ERROR_VARIABLE prepare_error
  )
elseif(UNIX)
  find_program(PATCHELF_EXECUTABLE patchelf REQUIRED)
  execute_process(
    COMMAND "${PATCHELF_EXECUTABLE}"
            --set-rpath
            "${ROO_ARTIFACT_NATIVE_RPATH}"
            "${ROO_ARTIFACT_NATIVE_LIBRARY}"
    RESULT_VARIABLE prepare_result
    ERROR_VARIABLE prepare_error
  )

  if(prepare_result EQUAL 0)
    execute_process(
      COMMAND "${PATCHELF_EXECUTABLE}"
              --print-rpath
              "${ROO_ARTIFACT_NATIVE_LIBRARY}"
      RESULT_VARIABLE verify_result
      OUTPUT_VARIABLE installed_rpath
      ERROR_VARIABLE verify_error
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT verify_result EQUAL 0)
      set(prepare_result "${verify_result}")
      set(prepare_error "${verify_error}")
    elseif(NOT installed_rpath STREQUAL ROO_ARTIFACT_NATIVE_RPATH)
      set(prepare_result 1)
      set(prepare_error
        "expected '${ROO_ARTIFACT_NATIVE_RPATH}', got '${installed_rpath}'"
      )
    endif()
  endif()
else()
  set(prepare_result 0)
endif()

if(NOT prepare_result EQUAL 0)
  message(FATAL_ERROR
    "Could not prepare package artifact native library '${ROO_ARTIFACT_NATIVE_LIBRARY}': ${prepare_error}"
  )
endif()
