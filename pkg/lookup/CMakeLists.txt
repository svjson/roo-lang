install(DIRECTORY src/
  DESTINATION share/lisple/pkg/lookup/src
  FILES_MATCHING
    PATTERN "*.lisple"
    PATTERN "*.lspl"
)

install(FILES package.edn README.md
  DESTINATION share/lisple/pkg/lookup
)

add_subdirectory(native)
