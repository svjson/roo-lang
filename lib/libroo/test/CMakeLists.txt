include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(${ROO_ROOT_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testroo ${TEST_SOURCES})

set_target_properties(testroo PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_include_directories(testroo
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/tests"
)

target_link_libraries(testroo
  GTest::gtest
  GTest::gmock
  roo_shared
)

roo_copy_runtime_dlls(testroo
  roo_shared
)

# Enable Include What You Use analyzer with TESTS_USE_IWYU=true
if(DEFINED ENV{TESTS_USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for roo tests...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET testroo PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

gtest_discover_tests(testroo)
