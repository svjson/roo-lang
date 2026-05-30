install(DIRECTORY src/
  DESTINATION share/lisple/pkg/proof/src
  FILES_MATCHING
    PATTERN "*.lisple"
    PATTERN "*.lspl"
    PATTERN "*.roo"
)

install(FILES package.edn README.md
  DESTINATION share/lisple/pkg/proof
)

add_subdirectory(native)
add_subdirectory(test)
