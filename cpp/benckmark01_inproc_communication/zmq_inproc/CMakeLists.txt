cmake_minimum_required(VERSION 3.0)
project(zmq_inproc)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND add_cppflags -g)
endif()
list(APPEND add_cppflags -fvisibility=hidden)
list(APPEND add_cppflags -std=c++11 -O2)
list(APPEND add_cppflags -Wall -Wextra)

# include
list(APPEND add_includes /usr/include)
list(APPEND add_libraries pthread zmq)

add_executable(zmq_inproc bench_zmq_inproc.cpp)
target_compile_definitions(zmq_inproc PRIVATE ${add_defines})
target_compile_options(zmq_inproc PRIVATE ${add_cppflags})
target_include_directories(zmq_inproc PRIVATE ${add_includes})
target_link_libraries(zmq_inproc ${add_libraries})

