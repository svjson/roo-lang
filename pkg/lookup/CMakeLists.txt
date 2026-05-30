install(DIRECTORY src/
  DESTINATION share/roo/pkg/lookup/src
  FILES_MATCHING
    PATTERN "*.roo"
)

install(FILES package.edn README.md
  DESTINATION share/roo/pkg/lookup
)

add_subdirectory(native)
