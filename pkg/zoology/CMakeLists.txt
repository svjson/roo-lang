install(FILES package.edn README.md DESTINATION share/roo/pkg/zoology)
install(DIRECTORY src/ DESTINATION share/roo/pkg/zoology/src
        FILES_MATCHING
          PATTERN "*.roo")

