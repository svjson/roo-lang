include(GoogleTest)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

enable_testing()

ADD_EXECUTABLE(testlisple ${TEST_SOURCES})

target_link_libraries(testlisple
  gtest
  gmock
  lisple
)

# Enable Include What You Use analyzer with TESTS_USE_IWYU=true
if(DEFINED ENV{TESTS_USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for lisple tests...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET testlisple PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()


include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/../src"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests"
)

gtest_discover_tests(testlisple)
