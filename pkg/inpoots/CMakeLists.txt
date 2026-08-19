install(FILES package.edn README.md
  DESTINATION share/roo/pkg/inpoots
)

install(DIRECTORY src/
  DESTINATION share/roo/pkg/inpoots/src
  FILES_MATCHING
    PATTERN "*.roo"
)

add_subdirectory(native)
if(ROO_BUILD_TESTS)
  add_subdirectory(test)
endif()
