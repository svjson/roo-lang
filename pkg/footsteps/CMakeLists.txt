install(FILES package.edn README.md DESTINATION share/roo/pkg/footsteps)
install(DIRECTORY src/ DESTINATION share/roo/pkg/footsteps/src
        FILES_MATCHING
          PATTERN "*.roo")
