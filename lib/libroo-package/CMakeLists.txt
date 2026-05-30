cmake_minimum_required(VERSION 3.20)

project(roo-package VERSION 0.1 LANGUAGES CXX)

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

get_filename_component(ROO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(roo-package_SOURCES
  src/lib/manifest.cpp
  src/lib/native_loader.cpp
)

add_library(roo-package_objects OBJECT ${roo-package_SOURCES})
target_include_directories(roo-package_objects
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (EXISTS "${ROO_ROOT_DIR}/lib/liblisple/include")
  target_include_directories(roo-package_objects
    PRIVATE
      ${ROO_ROOT_DIR}/lib/liblisple/include
  )
endif()

if (TARGET Lisple::lisple_shared)
  get_target_property(_lisple_include_dirs Lisple::lisple_shared INTERFACE_INCLUDE_DIRECTORIES)
  if (_lisple_include_dirs)
    target_include_directories(roo-package_objects PRIVATE ${_lisple_include_dirs})
  endif()
endif()

set_target_properties(roo-package_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(roo-package_static STATIC $<TARGET_OBJECTS:roo-package_objects>)
add_library(roo-package_shared SHARED $<TARGET_OBJECTS:roo-package_objects>)

foreach(target roo-package_static roo-package_shared)
  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME roo-package
    POSITION_INDEPENDENT_CODE ON
  )

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
endforeach()

set_target_properties(roo-package_static PROPERTIES
  EXPORT_NAME package_static
)

set_target_properties(roo-package_shared PROPERTIES
  EXPORT_NAME package_shared
)

target_link_libraries(roo-package_static
  PUBLIC Lisple::lisple_static
  PRIVATE ${CMAKE_DL_LIBS}
)

target_link_libraries(roo-package_shared
  PUBLIC Lisple::lisple_shared
  PRIVATE ${CMAKE_DL_LIBS}
)

add_library(Roo::package_static ALIAS roo-package_static)
add_library(Roo::package_shared ALIAS roo-package_shared)

add_subdirectory(test)

install(TARGETS roo-package_static roo-package_shared
  EXPORT roo-packageTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT roo-packageTargets
  FILE roo-packageTargets.cmake
  NAMESPACE Roo::
  DESTINATION lib/cmake/roo-package
)

install(FILES ${ROO_ROOT_DIR}/cmake/roo-packageConfig.cmake
  DESTINATION lib/cmake/roo-package
)
