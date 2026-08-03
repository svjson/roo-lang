install(FILES package.edn README.md DESTINATION share/roo/pkg/boodle)
install(DIRECTORY src/ DESTINATION share/roo/pkg/boodle/src
        FILES_MATCHING
          PATTERN "*.roo")
