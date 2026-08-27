install(FILES package.edn README.md DESTINATION share/roo/pkg/wraparoo)
install(DIRECTORY src/ DESTINATION share/roo/pkg/wraparoo/src
        FILES_MATCHING
          PATTERN "*.roo")
