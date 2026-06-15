install(FILES package.edn README.md DESTINATION share/roo/pkg/spool)
install(DIRECTORY src/ DESTINATION share/roo/pkg/spool/src
        FILES_MATCHING
          PATTERN "*.roo")
