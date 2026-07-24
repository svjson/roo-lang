if(NOT DEFINED ROO_ROOT_DIR)
  get_filename_component(ROO_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif()

if(NOT DEFINED ROO_VERSION)
  file(STRINGS "${ROO_ROOT_DIR}/VERSION" ROO_VERSION LIMIT_COUNT 1)
  string(STRIP "${ROO_VERSION}" ROO_VERSION)
endif()

if(NOT ROO_VERSION MATCHES "^[0-9][0-9]*\\.[0-9][0-9]*\\.[0-9][0-9]*")
  message(FATAL_ERROR "Invalid Roo version '${ROO_VERSION}'")
endif()

if(NOT DEFINED ROO_PROJECT_VERSION)
  string(REGEX MATCH "^[0-9][0-9]*\\.[0-9][0-9]*\\.[0-9][0-9]*" ROO_PROJECT_VERSION "${ROO_VERSION}")
endif()
