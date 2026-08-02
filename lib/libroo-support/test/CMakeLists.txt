include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(${ROO_ROOT_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE ROO_SUPPORT_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testsupport ${ROO_SUPPORT_TEST_SOURCES})

set_target_properties(testsupport PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:roo-support_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testsupport
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  roo-support_shared
)

roo_copy_runtime_dlls(testsupport
  roo-support_shared
)
