enable_testing()

set(ROO_NATIVE_PACKAGE_STAGE_DIR
  "${CMAKE_BINARY_DIR}/native-package-stage/pkg"
  CACHE PATH
  "Directory used to stage Roo packages with their built native libraries."
)

add_subdirectory(proof)
add_subdirectory(i18n)
add_subdirectory(loom)
add_subdirectory(proofread)
add_subdirectory(lookup)
add_subdirectory(spool)
add_subdirectory(workbook)
add_subdirectory(footsteps)

function(roo_add_stage_package_target package_name)
  set(options)
  set(one_value_args NATIVE_TARGET)
  set(multi_value_args)
  cmake_parse_arguments(STAGE_PACKAGE
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )

  set(package_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/${package_name}")
  set(package_stage_dir "${ROO_NATIVE_PACKAGE_STAGE_DIR}/${package_name}")
  set(package_target "stage_${package_name}_package")

  add_custom_target(${package_target}
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${package_stage_dir}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${package_stage_dir}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${package_source_dir}/package.edn"
            "${package_stage_dir}/package.edn"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${package_source_dir}/README.md"
            "${package_stage_dir}/README.md"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${package_source_dir}/src"
            "${package_stage_dir}/src"
    VERBATIM
  )

  if(EXISTS "${package_source_dir}/test")
    add_custom_command(TARGET ${package_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${package_source_dir}/test"
              "${package_stage_dir}/test"
      VERBATIM
    )
  endif()

  if(STAGE_PACKAGE_NATIVE_TARGET)
    add_dependencies(${package_target} ${STAGE_PACKAGE_NATIVE_TARGET})
    add_custom_command(TARGET ${package_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
              "${package_stage_dir}/native"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "$<TARGET_FILE:${STAGE_PACKAGE_NATIVE_TARGET}>"
              "${package_stage_dir}/native/$<TARGET_FILE_NAME:${STAGE_PACKAGE_NATIVE_TARGET}>"
      VERBATIM
    )
  endif()
endfunction()

roo_add_stage_package_target(proof NATIVE_TARGET proof_native)
roo_add_stage_package_target(lookup NATIVE_TARGET lookup_native)
roo_add_stage_package_target(proofread NATIVE_TARGET proofread_native)
roo_add_stage_package_target(loom)
roo_add_stage_package_target(workbook)
roo_add_stage_package_target(footsteps)

add_custom_target(stage_native_packages
  DEPENDS
    stage_proof_package
    stage_lookup_package
    stage_proofread_package
    stage_loom_package
    stage_workbook_package
    stage_footsteps_package
)

if(TARGET testproof)
  add_dependencies(testproof stage_proof_package)
endif()
