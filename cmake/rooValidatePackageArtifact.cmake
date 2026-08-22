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

string(REGEX MATCH
  ":load-roots[ \t\r\n]+\\[([^]]*)\\]"
  manifest_load_roots
  "${manifest}"
)
if(manifest_load_roots STREQUAL "")
  message(FATAL_ERROR
    "Package artifact '${ROO_ARTIFACT_PACKAGE}' has no valid :load-roots in ${manifest_path}"
  )
endif()

string(REGEX MATCHALL
  "\"[^\"]+\""
  declared_load_roots
  "${CMAKE_MATCH_1}"
)
set(declared_payload_paths)
foreach(quoted_load_root IN LISTS declared_load_roots)
  string(REGEX REPLACE "^\"|\"$" "" load_root "${quoted_load_root}")
  list(APPEND declared_payload_paths "${load_root}")
  if(NOT EXISTS "${ROO_ARTIFACT_DIR}/${load_root}")
    message(FATAL_ERROR
      "Package artifact '${ROO_ARTIFACT_PACKAGE}' is missing declared load root '${load_root}'."
    )
  endif()
endforeach()

string(REGEX MATCHALL
  ":path[ \t\r\n]+\"[^\"]+\""
  declared_path_entries
  "${manifest}"
)
foreach(path_entry IN LISTS declared_path_entries)
  string(REGEX REPLACE
    "^:path[ \t\r\n]+\"|\"$"
    ""
    declared_path
    "${path_entry}"
  )
  list(APPEND declared_payload_paths "${declared_path}")
  if(NOT EXISTS "${ROO_ARTIFACT_DIR}/${declared_path}")
    message(FATAL_ERROR
      "Package artifact '${ROO_ARTIFACT_PACKAGE}' is missing declared path '${declared_path}'."
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
      "Generated payload '${required_file}' is not beneath a manifest-declared package path."
    )
  endif()
endforeach()
