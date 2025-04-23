# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\managementsystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\managementsystem_autogen.dir\\ParseCache.txt"
  "managementsystem_autogen"
  )
endif()
