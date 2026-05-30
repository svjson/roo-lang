get_filename_component(ROO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(roo_SOURCES
  src/roo/adapter.cpp
  src/roo/bind.cpp
  src/roo/context.cpp
  src/roo/io/dir_root_file_system.cpp
  src/roo/io/file_system.cpp
  src/roo/io/file_system_namespace_source.cpp
  src/roo/io/null_file_system.cpp
  src/roo/namespace_loader.cpp
  src/roo/exec.cpp
  src/roo/runtime/dict.cpp
  src/roo/runtime/eval_plan.cpp
  src/roo/runtime/exec_node.cpp
  src/roo/runtime/node.cpp
  src/roo/runtime/lower.cpp
  src/roo/runtime/pool.cpp
  src/roo/runtime/seq.cpp
  src/roo/runtime/value.cpp
  src/roo/form.cpp
  src/roo/host/object.cpp
  src/roo/host/schema.cpp
  src/roo/host/transform.cpp
  src/roo/host/type.cpp
  src/roo/impl.cpp
  src/roo/lang.cpp
  src/roo/lang/base.cpp
  src/roo/lang/bind_form.cpp
  src/roo/lang/branch.cpp
  src/roo/lang/func.cpp
  src/roo/lang/io/file.cpp
  src/roo/lang/io/fs.cpp
  src/roo/lang/io/io_namespace.cpp
  src/roo/lang/io/path.cpp
  src/roo/lang/loop.cpp
  src/roo/lang/math.cpp
  src/roo/lang/num.cpp
  src/roo/lang/operator.cpp
  src/roo/lang/predicate.cpp
  src/roo/lang/rewrite.cpp
  src/roo/lang/seq.cpp
  src/roo/lang/seq_func.cpp
  src/roo/lang/string.cpp
  src/roo/lang/struct.cpp
  src/roo/lang/time.cpp
  src/roo/lexer.cpp
  src/roo/exception.cpp
  src/roo/namespace.cpp
  src/roo/parser.cpp
  src/roo/reader.cpp
  src/roo/runtime.cpp
  src/roo/scope.cpp
  src/roo/struct.cpp
  src/roo/type.cpp
)

add_library(roo_objects OBJECT ${roo_SOURCES})
target_include_directories(roo_objects
  PRIVATE
    ${ROO_ROOT_DIR}/lib/libroo/include
)

set_target_properties(roo_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(roo INTERFACE)
add_library(roo_static STATIC $<TARGET_OBJECTS:roo_objects>)
add_library(roo_shared SHARED $<TARGET_OBJECTS:roo_objects>)

add_library(Roo::roo ALIAS roo)
add_library(Roo::roo_static ALIAS roo_static)
add_library(Roo::roo_shared ALIAS roo_shared)

foreach(target roo_static roo_shared)
  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME roo
  )

  target_link_libraries(${target})

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${ROO_ROOT_DIR}/lib/libroo/include>
      $<INSTALL_INTERFACE:include>
  )
endforeach()

target_link_libraries(roo INTERFACE roo_shared)

add_subdirectory(test)
