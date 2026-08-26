if(ROO_BUILD_TESTS)
  enable_testing()
endif()

# Legacy package-repository install rules remain available as an explicit
# component during the Loom migration, but supported aggregate installation
# does not select them. Package-native development headers opt back into the
# ordinary non-package install component in their own CMake files.
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME RooPackageRepositoryLegacy)

set(ROO_PACKAGE_STAGE_DIR
  "${CMAKE_BINARY_DIR}/package-stage/pkg"
  CACHE PATH
  "Directory used to stage Roo packages for testing and cross-package dependency resolution, including their built native libraries where present."
)

set(ROO_PACKAGE_ARTIFACT_DIR
  "${CMAKE_BINARY_DIR}/package-artifacts"
  CACHE PATH
  "Directory containing install-ready Roo package inputs for Loom."
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
  set(one_value_args NATIVE_PATH NATIVE_TARGET)
  set(multi_value_args LOAD_ROOTS)
  cmake_parse_arguments(STAGE_PACKAGE
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )

  set(package_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/${package_name}")
  set(package_stage_dir "${ROO_PACKAGE_STAGE_DIR}/${package_name}")
  set(package_artifact_dir "${ROO_PACKAGE_ARTIFACT_DIR}/${package_name}")
  set(package_artifact_destination "package-artifacts/${package_name}")
  set(package_artifact_component "RooPackageArtifact-${package_name}")
  set(package_target "stage_${package_name}_package")
  set(package_artifact_target "package_artifact_${package_name}")

  if(NOT STAGE_PACKAGE_LOAD_ROOTS)
    set(STAGE_PACKAGE_LOAD_ROOTS src)
  endif()

  if(STAGE_PACKAGE_NATIVE_TARGET AND NOT STAGE_PACKAGE_NATIVE_PATH)
    set(STAGE_PACKAGE_NATIVE_PATH native)
  endif()

  install(FILES
    "${package_source_dir}/package.edn"
    "${package_source_dir}/README.md"
    DESTINATION "${package_artifact_destination}"
    COMPONENT "${package_artifact_component}"
    EXCLUDE_FROM_ALL
  )

  foreach(load_root IN LISTS STAGE_PACKAGE_LOAD_ROOTS)
    install(DIRECTORY "${package_source_dir}/${load_root}/"
      DESTINATION "${package_artifact_destination}/${load_root}"
      COMPONENT "${package_artifact_component}"
      EXCLUDE_FROM_ALL
    )
  endforeach()

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

  foreach(load_root IN LISTS STAGE_PACKAGE_LOAD_ROOTS)
    add_custom_command(TARGET ${package_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${package_source_dir}/${load_root}"
              "${package_stage_dir}/${load_root}"
      VERBATIM
    )
  endforeach()

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
              "${package_stage_dir}/${STAGE_PACKAGE_NATIVE_PATH}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "$<TARGET_FILE:${STAGE_PACKAGE_NATIVE_TARGET}>"
              "${package_stage_dir}/${STAGE_PACKAGE_NATIVE_PATH}/$<TARGET_FILE_NAME:${STAGE_PACKAGE_NATIVE_TARGET}>"
      VERBATIM
    )

    install(TARGETS ${STAGE_PACKAGE_NATIVE_TARGET}
      LIBRARY
        DESTINATION "${package_artifact_destination}/${STAGE_PACKAGE_NATIVE_PATH}"
        COMPONENT "${package_artifact_component}"
        EXCLUDE_FROM_ALL
      RUNTIME
        DESTINATION "${package_artifact_destination}/${STAGE_PACKAGE_NATIVE_PATH}"
        COMPONENT "${package_artifact_component}"
        EXCLUDE_FROM_ALL
    )

    set(package_artifact_required_files
      "${STAGE_PACKAGE_NATIVE_PATH}/$<TARGET_FILE_NAME:${STAGE_PACKAGE_NATIVE_TARGET}>"
    )
    set(package_artifact_prepare_command
      COMMAND ${CMAKE_COMMAND}
              "-DROO_ARTIFACT_NATIVE_LIBRARY=${package_artifact_dir}/${STAGE_PACKAGE_NATIVE_PATH}/$<TARGET_FILE_NAME:${STAGE_PACKAGE_NATIVE_TARGET}>"
              "-DROO_ARTIFACT_NATIVE_OLD_RPATH=${ROO_PACKAGE_NATIVE_INSTALL_RPATH}"
              "-DROO_ARTIFACT_NATIVE_RPATH=${ROO_PACKAGE_NATIVE_ARTIFACT_RPATH}"
              -P "${ROO_ROOT_DIR}/cmake/rooPreparePackageArtifact.cmake"
    )
  else()
    set(package_artifact_required_files)
    set(package_artifact_prepare_command)
  endif()

  add_custom_target(${package_artifact_target}
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${package_artifact_dir}"
    COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}"
            --config "$<CONFIG>"
            --prefix "${CMAKE_BINARY_DIR}"
            --component "${package_artifact_component}"
    ${package_artifact_prepare_command}
    COMMAND ${CMAKE_COMMAND}
            "-DROO_ARTIFACT_DIR=${package_artifact_dir}"
            "-DROO_ARTIFACT_PACKAGE=${package_name}"
            "-DROO_ARTIFACT_REQUIRED_FILES=${package_artifact_required_files}"
            -P "${ROO_ROOT_DIR}/cmake/rooValidatePackageArtifact.cmake"
    VERBATIM
  )

  if(STAGE_PACKAGE_NATIVE_TARGET)
    add_dependencies(${package_artifact_target} ${STAGE_PACKAGE_NATIVE_TARGET})
  endif()

  set_property(TARGET ${package_artifact_target}
    PROPERTY ROO_PACKAGE_ARTIFACT_DIR "${package_artifact_dir}"
  )
  set_property(GLOBAL APPEND PROPERTY ROO_PACKAGE_ARTIFACT_TARGETS
    "${package_artifact_target}"
  )
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

add_dependencies(stage_loom_package stage_lookup_package)

get_property(package_artifact_targets GLOBAL
  PROPERTY ROO_PACKAGE_ARTIFACT_TARGETS
)
add_custom_target(package_artifacts DEPENDS ${package_artifact_targets})

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
