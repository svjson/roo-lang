include(${CMAKE_SOURCE_DIR}/cmake/rooTestDeps.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE ROOC_TEST_SOURCES tests/*.cpp)

enable_testing()

add_library(rooc-test-native SHARED
  native/test_package.cpp
)
target_link_libraries(rooc-test-native
  PRIVATE
    Roo::roo_shared
)
target_include_directories(rooc-test-native
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libroo-package/include
)

add_executable(testrooc
  ${ROOC_TEST_SOURCES}
  $<TARGET_OBJECTS:rooc_objects>
)
add_dependencies(testrooc
  rooc-test-native
  stage_proof_package
)

set_target_properties(testrooc PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>;$<TARGET_FILE_DIR:roo-package_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testrooc
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  rooc_objects
)

roo_copy_runtime_dlls(testrooc
  roo_shared
  roo-package_shared
  rooc-test-native
)

target_compile_definitions(testrooc
  PRIVATE
    ROOC_TEST_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
    ROOC_TEST_BUILD_ROOT="${CMAKE_BINARY_DIR}"
    ROOC_TEST_STAGED_PACKAGE_REPOSITORY_ROOT="${ROO_PACKAGE_STAGE_DIR}"
    ROOC_TEST_CMAKE_COMMAND="${CMAKE_COMMAND}"
    ROOC_TEST_EXECUTABLE_SUFFIX="${CMAKE_EXECUTABLE_SUFFIX}"
    ROOC_TEST_NATIVE_LIBRARY="$<TARGET_FILE:rooc-test-native>"
)

gtest_discover_tests(testrooc)
