install(DIRECTORY src/
  DESTINATION share/lisple/pkg/proofread/src
  FILES_MATCHING
    PATTERN "*.lisple"
    PATTERN "*.lspl"
    PATTERN "*.roo"
)

install(FILES package.edn README.md
  DESTINATION share/lisple/pkg/proofread
)

add_subdirectory(native)

enable_testing()

set(PROOFREAD_TEST_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/test/assets")

add_test(
  NAME proofread_cli_checks_single_lisple_file
  COMMAND $<TARGET_FILE:lisple_cli>
          ${CMAKE_CURRENT_SOURCE_DIR}
          ${PROOFREAD_TEST_ASSETS}/valid/sample.lisple
)

add_test(
  NAME proofread_cli_checks_globbed_lisple_and_edn_files
  COMMAND $<TARGET_FILE:lisple_cli>
          ${CMAKE_CURRENT_SOURCE_DIR}
          "${PROOFREAD_TEST_ASSETS}/valid/*"
)

add_test(
  NAME proofread_cli_rejects_invalid_file
  COMMAND $<TARGET_FILE:lisple_cli>
          ${CMAKE_CURRENT_SOURCE_DIR}
          ${PROOFREAD_TEST_ASSETS}/invalid/broken.lisple
)
set_tests_properties(proofread_cli_rejects_invalid_file PROPERTIES WILL_FAIL TRUE)
