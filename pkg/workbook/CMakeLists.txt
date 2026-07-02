install(FILES package.edn README.md DESTINATION share/roo/pkg/workbook)
install(DIRECTORY src/ DESTINATION share/roo/pkg/workbook/src
        FILES_MATCHING
          PATTERN "*.roo")
