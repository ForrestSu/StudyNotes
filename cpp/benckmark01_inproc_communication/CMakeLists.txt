cmake_minimum_required(VERSION 3.5)
set(PROJECT bench_inproc_chat)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(uv_async)
add_subdirectory(zmq_inproc)
