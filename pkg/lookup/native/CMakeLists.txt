add_library(lookup_native SHARED
  src/analysis.cpp
  src/native.cpp
  src/reader.cpp
  src/source_values.cpp
  src/system.cpp
)

set_target_properties(lookup_native PROPERTIES
  OUTPUT_NAME lookup-native
  INSTALL_RPATH "${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
)

target_include_directories(lookup_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(lookup_native
  PRIVATE
    LOOKUP_NATIVE_BUILDING
)

target_link_libraries(lookup_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

install(TARGETS lookup_native
  LIBRARY DESTINATION share/roo/pkg/lookup/native
  RUNTIME DESTINATION share/roo/pkg/lookup/native
)

install(DIRECTORY include/
  DESTINATION include
)
