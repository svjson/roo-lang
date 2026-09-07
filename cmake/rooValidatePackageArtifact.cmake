if(NOT DEFINED ROO_ARTIFACT_DIR)
  message(FATAL_ERROR "ROO_ARTIFACT_DIR is required.")
endif()

if(NOT DEFINED ROO_ARTIFACT_PACKAGE)
  message(FATAL_ERROR "ROO_ARTIFACT_PACKAGE is required.")
endif()

file(TO_CMAKE_PATH "${ROO_ARTIFACT_DIR}" artifact_dir)
string(FIND "${artifact_dir}/" "/share/roo/pkg/" installed_repository_offset)
if(NOT installed_repository_offset EQUAL -1)
  message(FATAL_ERROR
    "Package artifact '${ROO_ARTIFACT_PACKAGE}' is inside an installed package repository: ${ROO_ARTIFACT_DIR}"
  )
endif()

set(manifest_path "${ROO_ARTIFACT_DIR}/package.edn")
if(NOT EXISTS "${manifest_path}")
  message(FATAL_ERROR
    "Package artifact '${ROO_ARTIFACT_PACKAGE}' has no package.edn: ${ROO_ARTIFACT_DIR}"
  )
endif()

file(READ "${manifest_path}" manifest)
string(REGEX MATCH
  ":name[ \t\r\n]+\"?([-A-Za-z0-9_.]+)\"?"
  manifest_name
  "${manifest}"
)
if(NOT CMAKE_MATCH_1 STREQUAL ROO_ARTIFACT_PACKAGE)
  message(FATAL_ERROR
    "Package artifact '${ROO_ARTIFACT_PACKAGE}' has a mismatched or invalid :name in ${manifest_path}"
  )
endif()

string(REGEX MATCH
  ":version[ \t\r\n]+\"([^\"]+)\""
  manifest_version
  "${manifest}"
)
if(CMAKE_MATCH_1 STREQUAL "")
  message(FATAL_ERROR
    "Package artifact '${ROO_ARTIFACT_PACKAGE}' has no valid :version in ${manifest_path}"
  )
endif()

set(declared_payload_paths)
foreach(payload_path IN LISTS ROO_ARTIFACT_PAYLOAD_PATHS)
  list(APPEND declared_payload_paths "${payload_path}")
  if(NOT EXISTS "${ROO_ARTIFACT_DIR}/${payload_path}")
    message(FATAL_ERROR
      "Package artifact '${ROO_ARTIFACT_PACKAGE}' is missing staged payload '${payload_path}'."
    )
  endif()
endforeach()

foreach(required_file IN LISTS ROO_ARTIFACT_REQUIRED_FILES)
  if(NOT EXISTS "${ROO_ARTIFACT_DIR}/${required_file}")
    message(FATAL_ERROR
      "Package artifact '${ROO_ARTIFACT_PACKAGE}' is missing generated payload '${required_file}'."
    )
  endif()

  set(payload_declared false)
  foreach(declared_path IN LISTS declared_payload_paths)
    string(FIND "${required_file}/" "${declared_path}/" declared_path_offset)
    if(declared_path_offset EQUAL 0)
      set(payload_declared true)
    endif()
  endforeach()
  if(NOT payload_declared)
    message(FATAL_ERROR
      "Generated payload '${required_file}' is not beneath a staged package path."
    )
  endif()
endforeach()
