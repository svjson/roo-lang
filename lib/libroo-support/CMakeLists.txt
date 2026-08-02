cmake_minimum_required(VERSION 3.20)

get_filename_component(ROO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
include("${ROO_ROOT_DIR}/cmake/rooVersion.cmake")
if (NOT DEFINED ROO_ABI_VERSION)
  set(ROO_ABI_VERSION 0)
endif()

project(roo-support VERSION ${ROO_PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/W4)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Werror)
  if (DEFINED ENV{DEBUG_SYMBOLS})
    add_compile_options(-g)
  endif()
endif()

set(roo-support_SOURCES
  src/lib/version.cpp
)

add_library(roo-support_objects OBJECT ${roo-support_SOURCES})
target_include_directories(roo-support_objects
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(roo-support_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(roo-support_static STATIC $<TARGET_OBJECTS:roo-support_objects>)
add_library(roo-support_shared SHARED $<TARGET_OBJECTS:roo-support_objects>)

foreach(target roo-support_static roo-support_shared)
  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME roo-support
    POSITION_INDEPENDENT_CODE ON
  )

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
endforeach()

set_target_properties(roo-support_static PROPERTIES
  EXPORT_NAME support_static
)

if(WIN32)
  set_target_properties(roo-support_static PROPERTIES
    ARCHIVE_OUTPUT_NAME roo-support-static
  )
  set_target_properties(roo-support_shared PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
endif()

set_target_properties(roo-support_shared PROPERTIES
  EXPORT_NAME support_shared
  VERSION ${PROJECT_VERSION}
  SOVERSION ${ROO_ABI_VERSION}
  INSTALL_RPATH "${ROO_LIB_INSTALL_RPATH}"
)

add_library(Roo::support_static ALIAS roo-support_static)
add_library(Roo::support_shared ALIAS roo-support_shared)

if(ROO_BUILD_TESTS)
  add_subdirectory(test)
endif()

install(TARGETS roo-support_static roo-support_shared
  EXPORT roo-supportTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT roo-supportTargets
  FILE roo-supportTargets.cmake
  NAMESPACE Roo::
  DESTINATION lib/cmake/roo-support
)

install(FILES ${ROO_ROOT_DIR}/cmake/roo-supportConfig.cmake
  DESTINATION lib/cmake/roo-support
)
