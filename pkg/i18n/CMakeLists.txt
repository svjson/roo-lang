install(FILES package.edn README.md DESTINATION share/roo/pkg/i18n)
install(DIRECTORY src/ DESTINATION share/roo/pkg/i18n/src
        FILES_MATCHING
          PATTERN "*.roo")
