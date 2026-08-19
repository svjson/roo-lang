install(FILES package.edn README.md DESTINATION share/roo/pkg/cli-trooper)
install(DIRECTORY src/ DESTINATION share/roo/pkg/cli-trooper/src
        FILES_MATCHING
          PATTERN "*.roo")
