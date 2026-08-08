install(FILES package.edn README.md DESTINATION share/roo/pkg/voodoo)
install(DIRECTORY src/ DESTINATION share/roo/pkg/voodoo/src
        FILES_MATCHING
          PATTERN "*.roo")
