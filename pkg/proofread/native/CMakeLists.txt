add_library(proofread_native SHARED
  src/native.cpp
)

set_target_properties(proofread_native PROPERTIES
  OUTPUT_NAME proofread-native
  INSTALL_RPATH "${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
)

target_include_directories(proofread_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(proofread_native
  PRIVATE
    PROOFREAD_NATIVE_BUILDING
)

target_link_libraries(proofread_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

install(TARGETS proofread_native
  LIBRARY DESTINATION share/roo/pkg/proofread/native
  RUNTIME DESTINATION share/roo/pkg/proofread/native
)

install(DIRECTORY include/
  DESTINATION include
)
