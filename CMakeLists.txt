cmake_minimum_required(VERSION 3.10)

project(roo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  if (DEFINED ENV{DEBUG_SYMBOLS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  endif()
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

add_subdirectory(lib/libroo)
add_subdirectory(lib/libroo-package)
add_subdirectory(lib/libroo-server)
add_subdirectory(pkg)

add_executable(roo_cli
  bin/roo/main.cpp
)

set(rooc_SOURCES
  bin/rooc/src/builder.cpp
  bin/rooc/src/generator.cpp
  bin/rooc/src/options.cpp
  bin/rooc/src/project.cpp
)

add_library(rooc_objects OBJECT ${rooc_SOURCES})

target_include_directories(rooc_objects
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/rooc/include
)
target_link_libraries(rooc_objects
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)
target_compile_definitions(rooc_objects
  PUBLIC
    ROO_VERSION="${PROJECT_VERSION}"
    ROO_SOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
    ROO_BUILD_ROOT="${CMAKE_CURRENT_BINARY_DIR}"
)

add_executable(rooc_cli
  bin/rooc/main.cpp
  $<TARGET_OBJECTS:rooc_objects>
)

set_target_properties(roo_cli PROPERTIES
  OUTPUT_NAME roo
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>;$<TARGET_FILE_DIR:roo-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
  INSTALL_RPATH "\$ORIGIN/../lib"
)

set_target_properties(rooc_cli PROPERTIES
  OUTPUT_NAME rooc
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>;$<TARGET_FILE_DIR:roo-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
  INSTALL_RPATH "\$ORIGIN/../lib"
)

target_link_libraries(roo_cli
  PRIVATE
    Roo::roo_shared
    Roo::package_shared
)
target_compile_definitions(roo_cli
  PRIVATE ROO_VERSION="${PROJECT_VERSION}"
)

target_link_libraries(rooc_cli
  PRIVATE
    rooc_objects
)

enable_testing()
add_test(
  NAME roo_cli_runs_package_file_with_file_dependency
  COMMAND $<TARGET_FILE:roo_cli>
          ${CMAKE_CURRENT_SOURCE_DIR}/lib/libroo-package/test/tests/assets/packages/cafe-register/src/cafe/run.roo
)
add_test(
  NAME roo_cli_runs_package_directory_entry_point
  COMMAND $<TARGET_FILE:roo_cli> .
)
set_tests_properties(
  roo_cli_runs_package_directory_entry_point
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libroo-package/test/tests/assets/packages/cafe-register
)
add_test(
  NAME roo_cli_runs_package_with_dynamic_proof_dependency
  COMMAND $<TARGET_FILE:roo_cli> .
)
set_tests_properties(
  roo_cli_runs_package_with_dynamic_proof_dependency
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/pkg/proof/test/assets/dynamic-smoke
)
add_test(
  NAME roo_cli_runs_package_tool_from_dependency
  COMMAND $<TARGET_FILE:roo_cli> proof
)
set_tests_properties(
  roo_cli_runs_package_tool_from_dependency
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/pkg/proof/test/assets/dynamic-smoke
)
add_test(
  NAME roo_cli_runs_package_with_namespace_roots
  COMMAND $<TARGET_FILE:roo_cli> .
)
set_tests_properties(
  roo_cli_runs_package_with_namespace_roots
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libroo-package/test/tests/assets/packages/namespace-root-app
)
add_test(
  NAME roo_cli_runs_package_with_autoloads
  COMMAND $<TARGET_FILE:roo_cli> .
)
set_tests_properties(
  roo_cli_runs_package_with_autoloads
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libroo-package/test/tests/assets/packages/autoload-app
)

add_test(
  NAME rooc_cli_generates_cafe_register
  COMMAND $<TARGET_FILE:rooc_cli>
          generate
          ${CMAKE_CURRENT_SOURCE_DIR}/lib/libroo-package/test/tests/assets/packages/cafe-register
          --build-dir
          ${CMAKE_CURRENT_BINARY_DIR}/rooc-test-generate
          --name
          cafe_register_compiled
)
add_test(
  NAME rooc_cli_builds_cafe_register
  COMMAND $<TARGET_FILE:rooc_cli>
          build
          ${CMAKE_CURRENT_SOURCE_DIR}/lib/libroo-package/test/tests/assets/packages/cafe-register
          --build-dir
          ${CMAKE_CURRENT_BINARY_DIR}/rooc-test-cafe
          --name
          cafe_register_compiled
)
add_test(
  NAME rooc_cli_runs_compiled_cafe_register
  COMMAND
    ${CMAKE_CURRENT_BINARY_DIR}/rooc-test-cafe/build/cafe_register_compiled${CMAKE_EXECUTABLE_SUFFIX}
)
set_tests_properties(
  rooc_cli_runs_compiled_cafe_register
  PROPERTIES
    DEPENDS rooc_cli_builds_cafe_register
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(bin/rooc/test)

install(CODE [[
  set(roo_installed_include_dir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/include/roo")
  if(EXISTS "${roo_installed_include_dir}")
    message(STATUS "Removing stale installed Roo headers: ${roo_installed_include_dir}")
    file(REMOVE_RECURSE "${roo_installed_include_dir}")
  endif()
]])

install(DIRECTORY lib/libroo/include/ DESTINATION include)

install(TARGETS roo_static roo_shared roo roo_cli rooc_cli
  EXPORT rooTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(EXPORT rooTargets
  FILE rooTargets.cmake
  NAMESPACE Roo::
  DESTINATION lib/cmake/roo
)

install(FILES cmake/rooConfig.cmake
  DESTINATION lib/cmake/roo
)

#Enable Include What You Use analyzer with USE_IWYU = true
if(DEFINED ENV{USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for roo...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET roo PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
