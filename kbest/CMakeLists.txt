cmake_minimum_required(VERSION 2.8)

PROJECT(kbest)

SET (BIN_DIR "${CMAKE_INSTALL_PREFIX}")
SET (SRC_DIR "${kbest_SOURCE_DIR}/src")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG" )

INCLUDE_DIRECTORIES(${SRC_DIR})

MESSAGE("Compiled binaries will be copied to : ${CMAKE_INSTALL_PREFIX} ")

CONFIGURE_FILE(solve.sh "${BIN_DIR}/solve.sh" COPYONLY)
CONFIGURE_FILE(kbest.py "${BIN_DIR}/kbest.py" COPYONLY)
CONFIGURE_FILE(plot.R "${BIN_DIR}/plot.R" COPYONLY)

add_subdirectory(src)
