include(GoogleTest)

file(GLOB_RECURSE LISPLEC_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testlisplec
  ${LISPLEC_TEST_SOURCES}
  $<TARGET_OBJECTS:lisplec_objects>
)
add_dependencies(testlisplec lisple-package-test-native)

set_target_properties(testlisplec PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:lisple_shared>;$<TARGET_FILE_DIR:lisple-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testlisplec
  gtest
  gtest_main
  gmock
  lisplec_objects
)

target_compile_definitions(testlisplec
  PRIVATE
    LISPLEC_TEST_REPO_ROOT="${CMAKE_SOURCE_DIR}"
    LISPLEC_TEST_BUILD_ROOT="${CMAKE_BINARY_DIR}"
    LISPLEC_TEST_CMAKE_COMMAND="${CMAKE_COMMAND}"
    LISPLEC_TEST_EXECUTABLE_SUFFIX="${CMAKE_EXECUTABLE_SUFFIX}"
    LISPLEC_TEST_NATIVE_LIBRARY="$<TARGET_FILE:lisple-package-test-native>"
)

gtest_discover_tests(testlisplec)
