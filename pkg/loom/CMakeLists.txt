install(FILES package.edn DESTINATION share/roo/pkg/loom)
install(DIRECTORY src/ DESTINATION share/roo/pkg/loom/src
        FILES_MATCHING
          PATTERN "*.roo")
