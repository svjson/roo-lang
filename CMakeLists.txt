cmake_minimum_required(VERSION 3.10)

project(lisple VERSION 0.1 LANGUAGES CXX)

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

add_subdirectory(lib/liblisple)
add_subdirectory(lib/lisple-package)
add_subdirectory(lib/lisple-server)
add_subdirectory(pkg)

add_executable(lisple_cli
  bin/lisple/main.cpp
)

set_target_properties(lisple_cli PROPERTIES
  OUTPUT_NAME lisple
  BUILD_RPATH "$<TARGET_FILE_DIR:lisple_shared>;$<TARGET_FILE_DIR:lisple-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
  INSTALL_RPATH "\$ORIGIN/../lib"
)

target_link_libraries(lisple_cli
  PRIVATE
    Lisple::lisple_shared
    Lisple::package_shared
)
target_compile_definitions(lisple_cli
  PRIVATE LISPLE_VERSION="${PROJECT_VERSION}"
)

enable_testing()
add_test(
  NAME lisple_cli_runs_package_file_with_file_dependency
  COMMAND $<TARGET_FILE:lisple_cli>
          ${CMAKE_CURRENT_SOURCE_DIR}/lib/lisple-package/test/tests/assets/packages/cafe-register/src/cafe/run.lisple
)
add_test(
  NAME lisple_cli_runs_package_directory_entry_point
  COMMAND $<TARGET_FILE:lisple_cli> .
)
set_tests_properties(
  lisple_cli_runs_package_directory_entry_point
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/lisple-package/test/tests/assets/packages/cafe-register
)
add_test(
  NAME lisple_cli_runs_package_with_dynamic_proof_dependency
  COMMAND $<TARGET_FILE:lisple_cli> .
)
set_tests_properties(
  lisple_cli_runs_package_with_dynamic_proof_dependency
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/pkg/proof/test/assets/dynamic-smoke
)
add_test(
  NAME lisple_cli_runs_package_with_namespace_roots
  COMMAND $<TARGET_FILE:lisple_cli> .
)
set_tests_properties(
  lisple_cli_runs_package_with_namespace_roots
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/lisple-package/test/tests/assets/packages/namespace-root-app
)
add_test(
  NAME lisple_cli_runs_package_with_autoloads
  COMMAND $<TARGET_FILE:lisple_cli> .
)
set_tests_properties(
  lisple_cli_runs_package_with_autoloads
  PROPERTIES
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/lisple-package/test/tests/assets/packages/autoload-app
)

install(CODE [[
  set(lisple_installed_include_dir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/include/lisple")
  if(EXISTS "${lisple_installed_include_dir}")
    message(STATUS "Removing stale installed Lisple headers: ${lisple_installed_include_dir}")
    file(REMOVE_RECURSE "${lisple_installed_include_dir}")
  endif()
]])

install(DIRECTORY lib/liblisple/include/ DESTINATION include)

install(TARGETS lisple_static lisple_shared lisple lisple_cli
  EXPORT lispleTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(EXPORT lispleTargets
  FILE lispleTargets.cmake
  NAMESPACE Lisple::
  DESTINATION lib/cmake/lisple
)

install(FILES cmake/lispleConfig.cmake
  DESTINATION lib/cmake/lisple
)

# Enable Include What You Use analyzer with USE_IWYU=true
if(DEFINED ENV{USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for lisple...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET lisple PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
