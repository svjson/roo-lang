include(FetchContent)

find_package(GTest QUIET)

if(NOT GTest_FOUND)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  )
  FetchContent_MakeAvailable(googletest)
endif()

if(NOT TARGET GTest::gtest AND TARGET gtest)
  add_library(GTest::gtest ALIAS gtest)
endif()
if(NOT TARGET GTest::gtest_main AND TARGET gtest_main)
  add_library(GTest::gtest_main ALIAS gtest_main)
endif()
if(NOT TARGET GTest::gmock AND TARGET gmock)
  add_library(GTest::gmock ALIAS gmock)
endif()
