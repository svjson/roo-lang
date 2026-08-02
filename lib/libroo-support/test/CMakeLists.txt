include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(${ROO_ROOT_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE ROO_SUPPORT_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testsupport ${ROO_SUPPORT_TEST_SOURCES})

target_link_libraries(testsupport
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  roo-support_objects
)
