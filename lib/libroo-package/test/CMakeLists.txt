include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(${ROO_ROOT_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE ROO_PACKAGE_TEST_SOURCES tests/*.cpp)
list(FILTER ROO_PACKAGE_TEST_SOURCES EXCLUDE REGEX "native_test_package\\.cpp$")

enable_testing()

add_library(roo-package-test-native SHARED
  tests/native_test_package.cpp
)

target_link_libraries(roo-package-test-native
  PRIVATE
    Roo::roo_shared
)

target_include_directories(roo-package-test-native
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_executable(testpackage ${ROO_PACKAGE_TEST_SOURCES})
add_dependencies(testpackage roo-package-test-native)

set_target_properties(testpackage PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>;$<TARGET_FILE_DIR:roo-support_shared>;$<TARGET_FILE_DIR:roo-package_shared>;$<TARGET_FILE_DIR:roo-package-test-native>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testpackage
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  roo-package_shared
)

roo_copy_runtime_dlls(testpackage
  roo_shared
  roo-support_shared
  roo-package_shared
  roo-package-test-native
)

target_compile_definitions(testpackage
  PRIVATE
    ROO_PACKAGE_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    ROO_PACKAGE_TEST_NATIVE_LIBRARY="$<TARGET_FILE:roo-package-test-native>"
)

gtest_discover_tests(testpackage)
