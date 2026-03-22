cmake_minimum_required(VERSION 3.10)

project(lisple VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  if (DEFINED ENV{DEBUG_SYMBOLS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  endif()
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

set(lisple_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/adapter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/bind.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/dir_root_file_system.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/exec.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime/eval_plan.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime/exec_node.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime/exec_tree.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime/lower.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime/pool.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime/value.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/form.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/host.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/impl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lang.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lang/base.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lang/bind_form.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lang/func.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lang/loop.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lang/struct.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/lexer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/exception.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/namespace.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/parser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/reader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/runtime.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/scope.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/struct.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lisple/type.cpp
)

add_library(lisple SHARED ${lisple_SOURCES})

target_link_libraries(lisple)

target_include_directories(lisple
  PUBLIC
    src
)

# Enable Include What You Use analyzer with USE_IWYU=true
if(DEFINED ENV{USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for lisple...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET lisple PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

add_subdirectory("test")
