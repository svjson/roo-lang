set(INPOOTS_TERMINAL_SOURCES
  src/terminal/decoder.cpp
  src/terminal/event.cpp
)

if(WIN32)
  list(APPEND INPOOTS_TERMINAL_SOURCES src/terminal/input_windows.cpp)
else()
  list(APPEND INPOOTS_TERMINAL_SOURCES src/terminal/input_posix.cpp)
endif()

add_library(inpoots_terminal STATIC ${INPOOTS_TERMINAL_SOURCES})
set_target_properties(inpoots_terminal PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
target_include_directories(inpoots_terminal
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(inpoots_terminal
  PUBLIC
    Roo::roo_shared
)

add_library(inpoots_native SHARED
  src/native.cpp
  src/native_package.cpp
  src/binding/input_namespace.cpp
  src/binding/input/input_adapter.cpp
)

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
  PRIVATE
    inpoots_terminal
)

install(TARGETS inpoots_native
  LIBRARY DESTINATION share/roo/pkg/inpoots/native
  RUNTIME DESTINATION share/roo/pkg/inpoots/native
)

install(DIRECTORY include/
  DESTINATION include
  COMPONENT Unspecified
)
