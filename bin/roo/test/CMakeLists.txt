add_library(roo-cli-test-native SHARED
  native/test_package.cpp
)
target_link_libraries(roo-cli-test-native
  PRIVATE
    Roo::roo_shared
)
target_include_directories(roo-cli-test-native
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libroo-package/include
)
