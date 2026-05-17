include(GoogleTest)

file(GLOB_RECURSE LISPLE_PACKAGE_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testpackage ${LISPLE_PACKAGE_TEST_SOURCES})

set_target_properties(testpackage PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:lisple_shared>;$<TARGET_FILE_DIR:lisple-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testpackage
  gtest
  gtest_main
  gmock
  lisple-package_shared
)

target_compile_definitions(testpackage
  PRIVATE LISPLE_PACKAGE_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

gtest_discover_tests(testpackage)
