include(GoogleTest)

file(GLOB_RECURSE PROOF_TEST_SOURCES tests/*.cpp)

enable_testing()

add_executable(testproof ${PROOF_TEST_SOURCES})

set_target_properties(testproof PROPERTIES
  BUILD_RPATH "$<TARGET_FILE_DIR:lisple_shared>;$<TARGET_FILE_DIR:lisple-package_shared>;$<TARGET_FILE_DIR:proof_native>"
  BUILD_RPATH_USE_LINK_PATH OFF
)

target_link_libraries(testproof
  gtest
  gtest_main
  gmock
  lisple_shared
  lisple-package_shared
  proof_native
)

target_compile_definitions(testproof
  PRIVATE PROOF_PACKAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/.."
)

gtest_discover_tests(testproof)
