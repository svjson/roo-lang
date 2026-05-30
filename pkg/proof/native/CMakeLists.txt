add_library(proof_native SHARED
  src/native.cpp
)

set_target_properties(proof_native PROPERTIES
  OUTPUT_NAME proof-native
)

target_include_directories(proof_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(proof_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

add_custom_command(TARGET proof_native POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/../native
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:proof_native>
          ${CMAKE_CURRENT_SOURCE_DIR}/../native/$<TARGET_FILE_NAME:proof_native>
  COMMENT "Copying proof native package artifact into pkg/proof/native"
)

install(TARGETS proof_native
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
)
