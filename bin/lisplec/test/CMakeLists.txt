include(GoogleTest)

file(GLOB_RECURSE LISPLEC_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testlisplec
  ${LISPLEC_TEST_SOURCES}
  $<TARGET_OBJECTS:lisplec_objects>
)

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
)

gtest_discover_tests(testlisplec)
