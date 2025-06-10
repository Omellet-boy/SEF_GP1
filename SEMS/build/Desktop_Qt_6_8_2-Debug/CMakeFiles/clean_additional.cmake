# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SEMS_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SEMS_autogen.dir/ParseCache.txt"
  "SEMS_autogen"
  )
endif()
