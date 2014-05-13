cmake_minimum_required (VERSION 2.8)

project (ProjIMA)

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror -g -ggdb " )
set ( SRC_DIR "src" )
set ( BIN_DIR "bin" )

find_package( OpenCV REQUIRED )
include_directories(/Path/to/libSAR/include/)

add_executable(${BIN_DIR}/Denoise
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/Args.cc
  ${SRC_DIR}/maxflow/graph.cpp
  ${SRC_DIR}/maxflow/maxflow.cpp
)

target_link_libraries (${BIN_DIR}/Denoise
  ${OpenCV_LIBS} SAR
)

add_executable(${BIN_DIR}/Viewer
  ${SRC_DIR}/viewer.cc
)

target_link_libraries (${BIN_DIR}/Viewer
  ${OpenCV_LIBS} SAR
)
