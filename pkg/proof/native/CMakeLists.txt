add_library(proof_native SHARED
  src/native.cpp
  src/native_package.cpp
  src/binding/syntax_namespace.cpp
  src/binding/syntax/fixture_namespace.cpp
  src/binding/syntax/scenario_namespace.cpp
  src/scenario.cpp
)

set_target_properties(proof_native PROPERTIES
  OUTPUT_NAME proof-native
  INSTALL_RPATH "${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
)

target_include_directories(proof_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(proof_native
  PRIVATE
    PROOF_NATIVE_BUILDING
)

target_link_libraries(proof_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

install(TARGETS proof_native
  LIBRARY DESTINATION share/roo/pkg/proof/native
  RUNTIME DESTINATION share/roo/pkg/proof/native
)

install(DIRECTORY include/
  DESTINATION include
  COMPONENT Unspecified
)
