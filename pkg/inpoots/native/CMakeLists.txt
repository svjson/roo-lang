add_library(inpoots_native SHARED
  src/native.cpp
  src/native_package.cpp
  src/binding/input_namespace.cpp
  src/binding/input/input_adapter.cpp
  src/terminal/decoder.cpp
  src/terminal/event.cpp
)

if(WIN32)
  target_sources(inpoots_native PRIVATE src/terminal/input_windows.cpp)
else()
  target_sources(inpoots_native PRIVATE src/terminal/input_posix.cpp)
endif()

set_target_properties(inpoots_native PROPERTIES
  OUTPUT_NAME inpoots-native
  INSTALL_RPATH "${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
)

target_include_directories(inpoots_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(inpoots_native
  PRIVATE
    INPOOTS_NATIVE_BUILDING
)

target_link_libraries(inpoots_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

install(TARGETS inpoots_native
  LIBRARY DESTINATION share/roo/pkg/inpoots/native
  RUNTIME DESTINATION share/roo/pkg/inpoots/native
)

install(DIRECTORY include/
  DESTINATION include
  COMPONENT Unspecified
)
