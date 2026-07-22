function(roo_copy_runtime_dlls target)
  if(NOT WIN32)
    return()
  endif()

  foreach(runtime_target IN LISTS ARGN)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "$<TARGET_FILE:${runtime_target}>"
              "$<TARGET_FILE_DIR:${target}>"
      VERBATIM
    )
  endforeach()
endfunction()
