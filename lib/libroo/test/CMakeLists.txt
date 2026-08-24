include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(${ROO_ROOT_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
list(FILTER TEST_SOURCES EXCLUDE REGEX "/benchmark/.*[.]cpp$")

file(GLOB BENCHMARK_SOURCES tests/benchmark/*.cpp)
list(APPEND BENCHMARK_SOURCES
  tests/fake_file_system.cpp
  tests/runtime_fixture.cpp
  tests/host/test_adapters/vectorgfx_native_adapters.cpp
  tests/host/test_adapters/vectorgfx_native_cachedprops_adapters.cpp
)

enable_testing()

add_executable(testroo ${TEST_SOURCES})
add_executable(benchmarkroo EXCLUDE_FROM_ALL
  ${BENCHMARK_SOURCES}
  $<TARGET_OBJECTS:roo_benchmark_objects>
)

set_target_properties(testroo PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_include_directories(testroo
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/tests"
    "${ROO_ROOT_DIR}/lib/libroo/src"
)
target_include_directories(benchmarkroo
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/tests"
    "${ROO_ROOT_DIR}/lib/libroo/include"
)

target_compile_definitions(benchmarkroo
  PRIVATE
    ROO_ENABLE_BENCHMARK_COUNTERS
    ROO_STATIC_DEFINE
)

target_link_libraries(testroo
  GTest::gtest
  GTest::gmock
  roo_shared
)
target_link_libraries(benchmarkroo
  GTest::gtest
  GTest::gmock
  Threads::Threads
)

if(COMMAND target_precompile_headers AND NOT DEFINED ENV{TESTS_USE_IWYU})
  target_precompile_headers(testroo
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/tests/precompiled.h"
  )
  target_precompile_headers(benchmarkroo
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/tests/precompiled.h"
  )
endif()

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
