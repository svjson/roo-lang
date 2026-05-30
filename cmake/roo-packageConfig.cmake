include(CMakeFindDependencyMacro)

find_dependency(lisple REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/roo-packageTargets.cmake")
