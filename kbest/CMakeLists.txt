cmake_minimum_required(VERSION 2.8)

PROJECT(kbest)

SET (BIN_DIR "${CMAKE_INSTALL_PREFIX}")
SET (SRC_DIR "${kbest_SOURCE_DIR}/src")

IF(${CMAKE_BUILD_TYPE} MATCHES "D")
  MESSAGE("[*] Build type: DEBUG")
  ADD_DEFINITIONS(-DDEBUG=1)
ELSE(${CMAKE_BUILD_TYPE} MATCHES "D")
  MESSAGE("[*] Build type: RELEASE")
  ADD_DEFINITIONS(-DDEBUG=0)
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "D")



INCLUDE_DIRECTORIES(${SRC_DIR})

MESSAGE("Compiled binaries will be copied to : ${CMAKE_INSTALL_PREFIX} ")

CONFIGURE_FILE(solve_problem_list.sh "${BIN_DIR}/solve_problem_list.sh" COPYONLY)
CONFIGURE_FILE(solve_problem_list.bat "${BIN_DIR}/solve_problem_list.bat" COPYONLY)
CONFIGURE_FILE(problemlist.py "${BIN_DIR}/problemlist.py" COPYONLY)
CONFIGURE_FILE(kbest.py "${BIN_DIR}/kbest.py" COPYONLY)
CONFIGURE_FILE(plots.R "${BIN_DIR}/plots.R" COPYONLY)

add_subdirectory(src)
