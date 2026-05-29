add_library(lookup_native SHARED
  src/native.cpp
)

set_target_properties(lookup_native PROPERTIES
  OUTPUT_NAME lookup-native
)

target_include_directories(lookup_native
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lookup_native
  PUBLIC
    Lisple::lisple_shared
    Lisple::package_shared
)

add_custom_command(TARGET lookup_native POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/../native
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:lookup_native>
          ${CMAKE_CURRENT_SOURCE_DIR}/../native/$<TARGET_FILE_NAME:lookup_native>
  COMMENT "Copying lookup native package artifact into pkg/lookup/native"
)

install(TARGETS lookup_native
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
)
