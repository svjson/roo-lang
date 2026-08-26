add_library(roopl_native SHARED
  src/native.cpp
  src/native_package.cpp
  src/binding/target_runtime_namespace.cpp
  src/binding/target/dedicated_adapter.cpp
  src/target/dedicated.cpp
)

set_target_properties(roopl_native PROPERTIES
  OUTPUT_NAME roopl-native
  INSTALL_RPATH "${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
)

target_include_directories(roopl_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(roopl_native
  PRIVATE
    ROOPL_NATIVE_BUILDING
)

target_link_libraries(roopl_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

install(TARGETS roopl_native
  LIBRARY DESTINATION share/roo/pkg/roopl/native
  RUNTIME DESTINATION share/roo/pkg/roopl/native
)

install(DIRECTORY include/
  DESTINATION include
  COMPONENT Unspecified
)
