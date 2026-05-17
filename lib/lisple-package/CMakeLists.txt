cmake_minimum_required(VERSION 3.20)

project(lisple-package VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

get_filename_component(LISPLE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(lisple-package_SOURCES
  src/lib/manifest.cpp
  src/lib/native_loader.cpp
)

add_library(lisple-package_objects OBJECT ${lisple-package_SOURCES})
target_include_directories(lisple-package_objects
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (EXISTS "${LISPLE_ROOT_DIR}/lib/liblisple/include")
  target_include_directories(lisple-package_objects
    PRIVATE
      ${LISPLE_ROOT_DIR}/lib/liblisple/include
  )
endif()

if (TARGET Lisple::lisple_shared)
  get_target_property(_lisple_include_dirs Lisple::lisple_shared INTERFACE_INCLUDE_DIRECTORIES)
  if (_lisple_include_dirs)
    target_include_directories(lisple-package_objects PRIVATE ${_lisple_include_dirs})
  endif()
endif()

set_target_properties(lisple-package_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(lisple-package_static STATIC $<TARGET_OBJECTS:lisple-package_objects>)
add_library(lisple-package_shared SHARED $<TARGET_OBJECTS:lisple-package_objects>)

foreach(target lisple-package_static lisple-package_shared)
  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME lisple-package
    POSITION_INDEPENDENT_CODE ON
  )

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
endforeach()

target_link_libraries(lisple-package_static
  PUBLIC Lisple::lisple_static
  PRIVATE ${CMAKE_DL_LIBS}
)

target_link_libraries(lisple-package_shared
  PUBLIC Lisple::lisple_shared
  PRIVATE ${CMAKE_DL_LIBS}
)

add_library(Lisple::package_static ALIAS lisple-package_static)
add_library(Lisple::package_shared ALIAS lisple-package_shared)

add_subdirectory(test)

install(TARGETS lisple-package_static lisple-package_shared
  EXPORT lisple-packageTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT lisple-packageTargets
  FILE lisple-packageTargets.cmake
  NAMESPACE Lisple::
  DESTINATION lib/cmake/lisple-package
)
