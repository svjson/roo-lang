install(DIRECTORY src/
  DESTINATION share/roo/pkg/proof/src
  FILES_MATCHING
    PATTERN "*.roo"
)

install(FILES package.edn README.md
  DESTINATION share/roo/pkg/proof
)

add_subdirectory(native)
if(ROO_BUILD_TESTS)
  add_subdirectory(test)
endif()
