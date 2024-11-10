
include(GoogleTest)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TEST_SOURCES *.cpp)

enable_testing()

add_executable(testserver ${TEST_SOURCES})

target_link_libraries(testserver
  gtest
  gmock
  lisple
  lisple-server-lib
)
