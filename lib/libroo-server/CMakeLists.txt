cmake_minimum_required(VERSION 3.20)

get_filename_component(ROO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
if (NOT DEFINED ROO_PROJECT_VERSION)
  file(STRINGS "${ROO_ROOT_DIR}/VERSION" ROO_PROJECT_VERSION LIMIT_COUNT 1)
  string(STRIP "${ROO_PROJECT_VERSION}" ROO_PROJECT_VERSION)
endif()
if (NOT DEFINED ROO_ABI_VERSION)
  set(ROO_ABI_VERSION 0)
endif()

project(roo-server VERSION ${ROO_PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if (MSVC)
  add_compile_options(/W4)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Werror)
  if (DEFINED ENV{DEBUG_SYMBOLS})
    add_compile_options(-g)
  endif()
endif()

if (NOT TARGET Roo::roo_shared)
  find_package(roo REQUIRED)
endif()

set(roo-server_SOURCES
  src/lib/server.cpp
  src/lib/message.cpp
  src/lib/dispatch.cpp
)

add_library(roo-server_objects OBJECT ${roo-server_SOURCES})
target_include_directories(roo-server_objects
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/roo-server
)
target_compile_definitions(roo-server_objects
  PRIVATE
    ROO_SERVER_BUILDING
)

if (EXISTS "${ROO_ROOT_DIR}/lib/libroo/include")
  target_include_directories(roo-server_objects
    PRIVATE
      ${ROO_ROOT_DIR}/lib/libroo/include
  )
endif()

if (TARGET Roo::roo_shared)
  get_target_property(_roo_include_dirs Roo::roo_shared INTERFACE_INCLUDE_DIRECTORIES)
  if (_roo_include_dirs)
    target_include_directories(roo-server_objects PRIVATE ${_roo_include_dirs})
  endif()
endif()

set_target_properties(roo-server_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# =========================
# Embeddable Server Library
# =========================
add_library(roo-server_static STATIC $<TARGET_OBJECTS:roo-server_objects>)
add_library(roo-server_shared SHARED $<TARGET_OBJECTS:roo-server_objects>)

foreach(target roo-server_static roo-server_shared)

  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME roo-server
    POSITION_INDEPENDENT_CODE ON
  )

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/roo-server>
      $<INSTALL_INTERFACE:include>
  )

endforeach()

target_compile_definitions(roo-server_static
  INTERFACE
    ROO_SERVER_STATIC_DEFINE
)

target_link_libraries(roo-server_static
  PUBLIC
    Roo::roo_static
)

target_link_libraries(roo-server_shared
  PUBLIC
    Roo::roo_shared
)

if(WIN32)
  target_link_libraries(roo-server_static PUBLIC ws2_32)
  target_link_libraries(roo-server_shared PUBLIC ws2_32)
endif()

set_target_properties(roo-server_static PROPERTIES
  EXPORT_NAME server_static
)

if(WIN32)
  set_target_properties(roo-server_static PROPERTIES
    ARCHIVE_OUTPUT_NAME roo-server-static
  )
  set_target_properties(roo-server_shared PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
endif()

set_target_properties(roo-server_shared PROPERTIES
  EXPORT_NAME server_shared
  VERSION ${PROJECT_VERSION}
  SOVERSION ${ROO_ABI_VERSION}
  INSTALL_RPATH "${ROO_LIB_INSTALL_RPATH}"
)

add_library(Roo::server_static ALIAS roo-server_static)
add_library(Roo::server_shared ALIAS roo-server_shared)

# =========================
# Stand-alone Executable
# =========================
add_executable(roo-server-exec
  src/main.cpp
)

target_link_libraries(roo-server-exec
  PRIVATE
    Roo::server_shared
    Roo::roo_shared
)

set_target_properties(roo-server-exec PROPERTIES
  OUTPUT_NAME "roo-server"
  INSTALL_RPATH "${ROO_BIN_INSTALL_RPATH}"
)

# =========================
# Install
# =========================

install(TARGETS roo-server_static roo-server_shared roo-server-exec
  EXPORT roo-serverTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT roo-serverTargets
  FILE roo-serverTargets.cmake
  NAMESPACE Roo::
  DESTINATION lib/cmake/roo-server
)

# Config file
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/roo-serverConfig.cmake")
  install(FILES cmake/roo-serverConfig.cmake
    DESTINATION lib/cmake/roo-server
  )
endif()

# =========================
# IWYU
# =========================

# Enable Include What You Use analyzer with USE_IWYU=true
if(DEFINED ENV{USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for GTK frontend...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET roo-server_shared PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

# =========================
# Tests
# =========================
option(ROO_SERVER_BUILD_TESTS "Build roo-server test suite." OFF)
if(ROO_BUILD_TESTS AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR OR ROO_SERVER_BUILD_TESTS))
  add_subdirectory(test)
endif()
