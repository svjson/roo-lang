install(FILES package.edn DESTINATION share/lisple/pkg/loom)
install(DIRECTORY src/ DESTINATION share/lisple/pkg/loom/src
        FILES_MATCHING
          PATTERN "*.lisple"
          PATTERN "*.roo")
