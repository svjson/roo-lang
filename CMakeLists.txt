cmake_minimum_required(VERSION 3.16)

project(lisple-server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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

# Embeddable Server library
add_library(lisple-server-lib STATIC src/lib/server.cpp src/lib/message.cpp src/lib/dispatch.cpp)
target_include_directories(lisple-server-lib PUBLIC include/lisple_server_lib)
set_target_properties(lisple-server-lib PROPERTIES OUTPUT_NAME "lisple-server")
target_link_libraries(lisple-server-lib PRIVATE lisple)

# Standalone server process
add_executable(lisple-server-exec src/main.cpp)
target_link_libraries(lisple-server-exec PRIVATE lisple-server-lib)
set_target_properties(lisple-server-exec PROPERTIES OUTPUT_NAME "lisple-server")

#add_executable(lisple-server ${SOURCES})

# Enable Include What You Use analyzer with USE_IWYU=true
if(DEFINED ENV{USE_IWYU})
  find_program(iwyu_path NAMES include-what-you-use iwyu)
  message("-- Configuring include-what-you-use for GTK frontend...")
  if(NOT iwyu_path)
    message(FATAL_ERROR "Could not find the program include-what-you-use")
  endif()
  set_property(TARGET crawler PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

find_package(PkgConfig)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/lisple-server"
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/lisple/src"
)

add_subdirectory(lib)
add_subdirectory(test)
