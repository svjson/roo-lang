include(GoogleTest)

file(GLOB_RECURSE ROOC_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testrooc
  ${ROOC_TEST_SOURCES}
  $<TARGET_OBJECTS:rooc_objects>
)
add_dependencies(testrooc roo-package-test-native)

set_target_properties(testrooc PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:lisple_shared>;$<TARGET_FILE_DIR:roo-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testrooc
  gtest
  gtest_main
  gmock
  rooc_objects
)

target_compile_definitions(testrooc
  PRIVATE
    ROOC_TEST_REPO_ROOT="${CMAKE_SOURCE_DIR}"
    ROOC_TEST_BUILD_ROOT="${CMAKE_BINARY_DIR}"
    ROOC_TEST_CMAKE_COMMAND="${CMAKE_COMMAND}"
    ROOC_TEST_EXECUTABLE_SUFFIX="${CMAKE_EXECUTABLE_SUFFIX}"
    ROOC_TEST_NATIVE_LIBRARY="$<TARGET_FILE:roo-package-test-native>"
)

gtest_discover_tests(testrooc)
