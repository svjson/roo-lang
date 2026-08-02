include(CMakeFindDependencyMacro)

find_dependency(roo REQUIRED)
find_dependency(roo-support REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/roo-packageTargets.cmake")
