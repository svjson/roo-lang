
include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(GoogleTest)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TEST_SOURCES *.cpp)

enable_testing()

add_executable(testserver ${TEST_SOURCES})

target_link_libraries(testserver
  GTest::gtest
  GTest::gmock
  Roo::roo_shared
  roo-server_shared
)

target_include_directories(testserver
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/roo-server
)
