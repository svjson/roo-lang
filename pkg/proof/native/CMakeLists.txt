add_library(proof_native SHARED
  src/native.cpp
)

set_target_properties(proof_native PROPERTIES
  OUTPUT_NAME proof-native
  INSTALL_RPATH "${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
)

target_include_directories(proof_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
)
