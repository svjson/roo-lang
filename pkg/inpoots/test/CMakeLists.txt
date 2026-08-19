include(${ROO_ROOT_DIR}/cmake/rooTestDeps.cmake)
include(${ROO_ROOT_DIR}/cmake/rooTestRuntime.cmake)
include(GoogleTest)

file(GLOB_RECURSE INPOOTS_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testinpoots ${INPOOTS_TEST_SOURCES})

target_include_directories(testinpoots
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../native/src
)

set_target_properties(testinpoots PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:roo_shared>;$<TARGET_FILE_DIR:roo-package_shared>;$<TARGET_FILE_DIR:inpoots_native>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testinpoots
  GTest::gtest
  GTest::gtest_main
  Roo::roo_shared
  Roo::package_shared
  inpoots_native
)

roo_copy_runtime_dlls(testinpoots
  roo_shared
  roo-package_shared
  inpoots_native
)

target_compile_definitions(testinpoots
  PRIVATE INPOOTS_PACKAGE_DIR="${ROO_PACKAGE_STAGE_DIR}/inpoots"
)

gtest_discover_tests(testinpoots)
