# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ProjectDemo_2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ProjectDemo_2_autogen.dir\\ParseCache.txt"
  "ProjectDemo_2_autogen"
  )
endif()
