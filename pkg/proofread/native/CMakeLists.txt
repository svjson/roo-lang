add_library(proofread_native SHARED
  src/native.cpp
)

set_target_properties(proofread_native PROPERTIES
  OUTPUT_NAME proofread-native
)

target_include_directories(proofread_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(proofread_native
  PUBLIC
    Roo::roo_shared
    Roo::package_shared
)

add_custom_command(TARGET proofread_native POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/../native
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:proofread_native>
          ${CMAKE_CURRENT_SOURCE_DIR}/../native/$<TARGET_FILE_NAME:proofread_native>
  COMMENT "Copying proofread native package artifact into pkg/proofread/native"
)

install(TARGETS proofread_native
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
)
