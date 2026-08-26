cmake_minimum_required(VERSION 3.10)

foreach(required_variable
        ROO_BUILD_LOCK_PATH
        ROO_BUILD_MAKEFILE
        ROO_BUILD_TARGET
        ROO_MAKE_PROGRAM
        ROO_SOURCE_DIR)
  if(NOT DEFINED ${required_variable})
    message(FATAL_ERROR "Missing required variable: ${required_variable}")
  endif()
endforeach()

file(LOCK "${ROO_BUILD_LOCK_PATH}"
  GUARD PROCESS
  TIMEOUT 0
  RESULT_VARIABLE lock_result
)

if(NOT lock_result STREQUAL "0")
  message(STATUS "Another Roo build is active; waiting for it to finish")
  file(LOCK "${ROO_BUILD_LOCK_PATH}" GUARD PROCESS)
  message(STATUS "Roo build lock acquired")
endif()

execute_process(
  COMMAND "${ROO_MAKE_PROGRAM}"
          -f "${ROO_BUILD_MAKEFILE}"
          --no-print-directory
          ROO_BUILD_LOCKED=1
          "${ROO_BUILD_TARGET}"
  WORKING_DIRECTORY "${ROO_SOURCE_DIR}"
  RESULT_VARIABLE build_result
)

if(NOT build_result STREQUAL "0")
  message(FATAL_ERROR
    "Make target '${ROO_BUILD_TARGET}' failed with exit code ${build_result}"
  )
endif()
