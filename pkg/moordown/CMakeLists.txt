install(FILES package.edn README.md DESTINATION share/roo/pkg/moordown)
install(DIRECTORY src/ DESTINATION share/roo/pkg/moordown/src
        FILES_MATCHING
          PATTERN "*.roo")
