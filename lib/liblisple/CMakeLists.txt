get_filename_component(LISPLE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(lisple_SOURCES
  src/lisple/adapter.cpp
  src/lisple/bind.cpp
  src/lisple/context.cpp
  src/lisple/dir_root_file_system.cpp
  src/lisple/file_system_namespace_source.cpp
  src/lisple/namespace_loader.cpp
  src/lisple/exec.cpp
  src/lisple/runtime/dict.cpp
  src/lisple/runtime/eval_plan.cpp
  src/lisple/runtime/exec_node.cpp
  src/lisple/runtime/node.cpp
  src/lisple/runtime/lower.cpp
  src/lisple/runtime/pool.cpp
  src/lisple/runtime/seq.cpp
  src/lisple/runtime/value.cpp
  src/lisple/form.cpp
  src/lisple/host/object.cpp
  src/lisple/host/schema.cpp
  src/lisple/host/transform.cpp
  src/lisple/host/type.cpp
  src/lisple/impl.cpp
  src/lisple/lang.cpp
  src/lisple/lang/base.cpp
  src/lisple/lang/bind_form.cpp
  src/lisple/lang/branch.cpp
  src/lisple/lang/func.cpp
  src/lisple/lang/loop.cpp
  src/lisple/lang/math.cpp
  src/lisple/lang/num.cpp
  src/lisple/lang/operator.cpp
  src/lisple/lang/predicate.cpp
  src/lisple/lang/rewrite.cpp
  src/lisple/lang/seq.cpp
  src/lisple/lang/seq_func.cpp
  src/lisple/lang/string.cpp
  src/lisple/lang/struct.cpp
  src/lisple/lang/time.cpp
  src/lisple/lexer.cpp
  src/lisple/exception.cpp
  src/lisple/namespace.cpp
  src/lisple/parser.cpp
  src/lisple/reader.cpp
  src/lisple/runtime.cpp
  src/lisple/scope.cpp
  src/lisple/struct.cpp
  src/lisple/type.cpp
)

add_library(lisple_objects OBJECT ${lisple_SOURCES})
target_include_directories(lisple_objects
  PRIVATE
    ${LISPLE_ROOT_DIR}/lib/liblisple/include
)

set_target_properties(lisple_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(lisple INTERFACE)
add_library(lisple_static STATIC $<TARGET_OBJECTS:lisple_objects>)
add_library(lisple_shared SHARED $<TARGET_OBJECTS:lisple_objects>)

add_library(Lisple::lisple ALIAS lisple)
add_library(Lisple::lisple_static ALIAS lisple_static)
add_library(Lisple::lisple_shared ALIAS lisple_shared)

foreach(target lisple_static lisple_shared)
  set_target_properties(${target} PROPERTIES
    OUTPUT_NAME lisple
  )

  target_link_libraries(${target})

  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${LISPLE_ROOT_DIR}/lib/liblisple/include>
      $<INSTALL_INTERFACE:include>
  )
endforeach()

target_link_libraries(lisple INTERFACE lisple_shared)
