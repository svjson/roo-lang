cmake_minimum_required(VERSION 3.20)

project(lisple-server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Werror)
  if (DEFINED ENV{DEBUG_SYMBOLS})
    add_compile_options(-g)
  endif()
elseif (MSVC)
  add_compile_options(/W4)
endif()

if (NOT TARGET Lisple::lisple_shared)
  find_package(lisple REQUIRED)
endif()

set(lisple-server_SOURCES
  src/lib/server.cpp
  src/lib/message.cpp
  src/lib/dispatch.cpp
)

# =========================
# Embeddable Server Library
# =========================
add_library(lisple-server INTERFACE)
add_library(lisple-server_static STATIC ${lisple-server_SOURCES})
add_library(lisple-server_shared SHARED ${lisple-server_SOURCES})

foreach(target lisple-server_static lisple-server_shared)

  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME lisple-server
    POSITION_INDEPENDENT_CODE ON
  )

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lisple-server>
      $<INSTALL_INTERFACE:include>
  )

endforeach()

target_link_libraries(lisple-server_static
  PUBLIC
    Lisple::lisple_static
)

target_link_libraries(lisple-server_shared
  PUBLIC
    Lisple::lisple_shared
)

# =========================
# Stand-alone Executable
# =========================
add_executable(lisple-server-exec
  src/main.cpp
)

target_link_libraries(lisple-server-exec
  PRIVATE
    lisple-server_shared
    Lisple::lisple_shared
)

set_target_properties(lisple-server-exec PROPERTIES
  OUTPUT_NAME "lisple-server"
  INSTALL_RPATH "$ORIGIN/../lib"
)

# =========================
# Install
# =========================

install(TARGETS lisple-server_static lisple-server_shared lisple-server-exec
  EXPORT lisple-serverTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT lisple-serverTargets
  FILE lisple-serverTargets.cmake
  NAMESPACE LispleServer::
  DESTINATION lib/cmake/lisple-server
)

# Config file
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/lisple-serverConfig.cmake")
  install(FILES cmake/lisple-serverConfig.cmake
    DESTINATION lib/cmake/lisple-server
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
  set_property(TARGET lisple-server_shared PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

# =========================
# Tests
# =========================
option(LISPLE_SERVER_BUILD_TESTS "Build lisple-server test suite." OFF)
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR OR LISPLE_SERVER_BUILD_TESTS)
  add_subdirectory(test)
endif()
