if(ROO_BUILD_TESTS)
  enable_testing()
endif()

set(ROO_PACKAGE_STAGE_DIR
  "${CMAKE_BINARY_DIR}/package-stage/pkg"
  CACHE PATH
  "Directory used to stage Roo packages for testing and cross-package dependency resolution, including their built native libraries where present."
)

add_subdirectory(proof)
add_subdirectory(i18n)
add_subdirectory(loom)
add_subdirectory(proofread)
add_subdirectory(boodle)
add_subdirectory(lookup)
add_subdirectory(moordown)
add_subdirectory(spool)
add_subdirectory(workbook)
add_subdirectory(footsteps)
add_subdirectory(zoology)
add_subdirectory(soot)
add_subdirectory(voodoo)
add_subdirectory(inpoots)
add_subdirectory(roopl)
add_subdirectory(cli-trooper)

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
  set(package_stage_dir "${ROO_PACKAGE_STAGE_DIR}/${package_name}")
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
    VERBATIM
  )

  if(EXISTS "${package_source_dir}/src")
    add_custom_command(TARGET ${package_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${package_source_dir}/src"
              "${package_stage_dir}/src"
      VERBATIM
    )
  endif()

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
roo_add_stage_package_target(boodle)
roo_add_stage_package_target(lookup NATIVE_TARGET lookup_native)
roo_add_stage_package_target(proofread NATIVE_TARGET proofread_native)
roo_add_stage_package_target(loom)
roo_add_stage_package_target(moordown)
roo_add_stage_package_target(workbook)
roo_add_stage_package_target(footsteps)
roo_add_stage_package_target(zoology)
roo_add_stage_package_target(soot)
roo_add_stage_package_target(voodoo)
roo_add_stage_package_target(inpoots NATIVE_TARGET inpoots_native)
roo_add_stage_package_target(roopl NATIVE_TARGET roopl_native)
roo_add_stage_package_target(i18n)
roo_add_stage_package_target(spool)
roo_add_stage_package_target(cli-trooper)

add_custom_target(stage_packages
  DEPENDS
    stage_proof_package
    stage_boodle_package
    stage_lookup_package
    stage_proofread_package
    stage_loom_package
    stage_moordown_package
    stage_workbook_package
    stage_footsteps_package
    stage_zoology_package
    stage_soot_package
    stage_voodoo_package
    stage_inpoots_package
    stage_roopl_package
    stage_i18n_package
    stage_spool_package
    stage_cli-trooper_package
)

if(TARGET testproof)
  add_dependencies(testproof stage_proof_package stage_soot_package stage_voodoo_package)
endif()

if(TARGET testinpoots)
  add_dependencies(testinpoots stage_inpoots_package stage_proof_package)
endif()
