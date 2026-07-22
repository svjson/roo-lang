cmake_minimum_required(VERSION 3.20)

get_filename_component(ROO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
if (NOT DEFINED ROO_PROJECT_VERSION)
  file(STRINGS "${ROO_ROOT_DIR}/VERSION" ROO_PROJECT_VERSION LIMIT_COUNT 1)
  string(STRIP "${ROO_PROJECT_VERSION}" ROO_PROJECT_VERSION)
endif()
if (NOT DEFINED ROO_ABI_VERSION)
  set(ROO_ABI_VERSION 0)
endif()

project(roo-package VERSION ${ROO_PROJECT_VERSION} LANGUAGES CXX)

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

if (NOT TARGET Roo::roo_shared)
  find_package(roo REQUIRED)
endif()

set(roo-package_SOURCES
  src/lib/manifest.cpp
  src/lib/native_loader.cpp
)

add_library(roo-package_objects OBJECT ${roo-package_SOURCES})
target_include_directories(roo-package_objects
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (EXISTS "${ROO_ROOT_DIR}/lib/libroo/include")
  target_include_directories(roo-package_objects
    PRIVATE
      ${ROO_ROOT_DIR}/lib/libroo/include
  )
endif()

if (TARGET Roo::roo_shared)
  get_target_property(_roo_include_dirs Roo::roo_shared INTERFACE_INCLUDE_DIRECTORIES)
  if (_roo_include_dirs)
    target_include_directories(roo-package_objects PRIVATE ${_roo_include_dirs})
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

if(WIN32)
  set_target_properties(roo-package_static PROPERTIES
    ARCHIVE_OUTPUT_NAME roo-package-static
  )
endif()

set_target_properties(roo-package_shared PROPERTIES
  EXPORT_NAME package_shared
  VERSION ${PROJECT_VERSION}
  SOVERSION ${ROO_ABI_VERSION}
  INSTALL_RPATH "${ROO_LIB_INSTALL_RPATH}"
)

target_link_libraries(roo-package_static
  PUBLIC Roo::roo_static
  PRIVATE ${CMAKE_DL_LIBS}
)

target_link_libraries(roo-package_shared
  PUBLIC Roo::roo_shared
  PRIVATE ${CMAKE_DL_LIBS}
)

add_library(Roo::package_static ALIAS roo-package_static)
add_library(Roo::package_shared ALIAS roo-package_shared)

if(ROO_BUILD_TESTS)
  add_subdirectory(test)
endif()

install(TARGETS roo-package_static roo-package_shared
  EXPORT roo-packageTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
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
