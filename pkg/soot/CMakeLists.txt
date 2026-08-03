install(FILES package.edn README.md DESTINATION share/roo/pkg/soot)
install(DIRECTORY src/ DESTINATION share/roo/pkg/soot/src
        FILES_MATCHING
          PATTERN "*.roo")
