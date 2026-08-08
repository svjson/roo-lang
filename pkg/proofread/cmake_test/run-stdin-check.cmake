if(NOT DEFINED ROO OR NOT DEFINED PROOFREAD_PACKAGE OR
   NOT DEFINED LOGICAL_PATH OR NOT DEFINED INPUT_PATH)
  message(FATAL_ERROR "Missing stdin check test arguments")
endif()

execute_process(
  COMMAND "${ROO}" "${PROOFREAD_PACKAGE}"
          --reporter simple --stdin-filename "${LOGICAL_PATH}"
  INPUT_FILE "${INPUT_PATH}"
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error_output
  RESULT_VARIABLE result
)

if(NOT result EQUAL 0)
  message(FATAL_ERROR "proofread stdin check failed:\n${output}\n${error_output}")
endif()

string(FIND "${output}" "redundant-do" diagnostic_index)
if(diagnostic_index EQUAL -1)
  message(FATAL_ERROR
          "proofread checked the on-disk logical file instead of stdin:\n${output}")
endif()

get_filename_component(logical_directory "${LOGICAL_PATH}" DIRECTORY)
string(FIND "${output}" "${logical_directory}/" path_index)
if(path_index EQUAL -1)
  message(FATAL_ERROR "proofread did not preserve the logical path:\n${output}")
endif()

string(FIND "${output}" "proofread: 1 file" summary_index)
if(summary_index EQUAL -1)
  message(FATAL_ERROR "proofread did not report stdin as one file:\n${output}")
endif()
