cmake_minimum_required (VERSION 2.8)

project (ProjIMA)

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g -ggdb" )
set ( SRC_DIR "src" )
set ( BIN_DIR "bin" )

find_package( OpenCV REQUIRED )

add_executable(${BIN_DIR}/Denoise
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/Args.cc
  ${SRC_DIR}/TVL0DecompositionMinimizer.cc
  ${SRC_DIR}/maxflow/graph.cpp
  ${SRC_DIR}/maxflow/maxflow.cpp
)

target_link_libraries (${BIN_DIR}/Denoise
  ${OpenCV_LIBS}
)
