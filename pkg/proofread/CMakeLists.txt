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
  add_subdirectory(cmake_test)
endif()
