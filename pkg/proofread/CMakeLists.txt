install(DIRECTORY src/
  DESTINATION share/roo/pkg/proofread/src
  FILES_MATCHING
    PATTERN "*.roo"
)

install(FILES package.edn README.md
  DESTINATION share/roo/pkg/proofread
)

add_subdirectory(native)

if(ROO_BUILD_TESTS)
  enable_testing()

  set(PROOFREAD_TEST_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/test/assets")
  set(PROOFREAD_TEST_PACKAGE "${ROO_NATIVE_PACKAGE_STAGE_DIR}/proofread")

  add_test(
    NAME proofread_cli_checks_single_roo_file
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            ${PROOFREAD_TEST_ASSETS}/valid/sample.roo
  )

  add_test(
    NAME proofread_cli_checks_globbed_roo_and_edn_files
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            "${PROOFREAD_TEST_ASSETS}/valid/*"
  )

  add_test(
    NAME proofread_cli_checks_directory_recursively
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            ${PROOFREAD_TEST_ASSETS}/directory
  )
  set_tests_properties(
    proofread_cli_checks_directory_recursively
    PROPERTIES
      PASS_REGULAR_EXPRESSION "proofread: 2 files, 0 warnings, 0 errors"
  )

  add_test(
    NAME proofread_cli_checks_package_root_sources
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            ${PROOFREAD_TEST_ASSETS}/package-root
  )
  set_tests_properties(
    proofread_cli_checks_package_root_sources
    PROPERTIES
      PASS_REGULAR_EXPRESSION "proofread: 2 files, 0 warnings, 0 errors"
  )

  add_test(
    NAME proofread_cli_rejects_invalid_file
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            ${PROOFREAD_TEST_ASSETS}/invalid/broken.roo
  )
  set_tests_properties(proofread_cli_rejects_invalid_file PROPERTIES WILL_FAIL TRUE)

  add_test(
    NAME proofread_cli_rejects_invalid_directory
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            ${PROOFREAD_TEST_ASSETS}/invalid
  )
  set_tests_properties(proofread_cli_rejects_invalid_directory PROPERTIES WILL_FAIL TRUE)

  add_test(
    NAME proofread_cli_allows_style_warnings
    COMMAND $<TARGET_FILE:roo_cli>
            ${PROOFREAD_TEST_PACKAGE}
            ${PROOFREAD_TEST_ASSETS}/style/redundant-do.roo
  )

  add_test(
    NAME proofread_rule_tests
    COMMAND $<TARGET_FILE:roo_cli> proof
  )
  set_tests_properties(
    proofread_rule_tests
    PROPERTIES
      WORKING_DIRECTORY "${PROOFREAD_TEST_PACKAGE}/test"
  )
endif()
