include(GoogleTest)

file(GLOB_RECURSE LISPLE_PACKAGE_TEST_SOURCES tests/*.cpp)
list(FILTER LISPLE_PACKAGE_TEST_SOURCES EXCLUDE REGEX "native_test_package\\.cpp$")

enable_testing()

add_library(lisple-package-test-native SHARED
  tests/native_test_package.cpp
)

target_link_libraries(lisple-package-test-native
  PRIVATE
    Lisple::lisple_shared
)

target_include_directories(lisple-package-test-native
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_executable(testpackage ${LISPLE_PACKAGE_TEST_SOURCES})
add_dependencies(testpackage lisple-package-test-native)

set_target_properties(testpackage PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:lisple_shared>;$<TARGET_FILE_DIR:lisple-package_shared>;$<TARGET_FILE_DIR:lisple-package-test-native>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testpackage
  gtest
  gtest_main
  gmock
  lisple-package_shared
)

target_compile_definitions(testpackage
  PRIVATE
    LISPLE_PACKAGE_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    LISPLE_PACKAGE_TEST_NATIVE_LIBRARY="$<TARGET_FILE:lisple-package-test-native>"
)

gtest_discover_tests(testpackage)
