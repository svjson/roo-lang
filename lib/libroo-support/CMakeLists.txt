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
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(roo-support_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(ROO_BUILD_TESTS)
  add_subdirectory(test)
endif()
