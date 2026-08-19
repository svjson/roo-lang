install(FILES package.edn README.md DESTINATION share/roo/pkg/roopl)
install(DIRECTORY src/ DESTINATION share/roo/pkg/roopl/src
        FILES_MATCHING
          PATTERN "*.roo")

add_subdirectory(native)
