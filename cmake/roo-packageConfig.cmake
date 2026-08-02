include(CMakeFindDependencyMacro)

find_dependency(roo REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/roo-packageTargets.cmake")
