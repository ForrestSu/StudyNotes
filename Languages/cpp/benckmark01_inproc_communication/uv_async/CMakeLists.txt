cmake_minimum_required(VERSION 3.0)
project(uv_async)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND add_cppflags -g)
endif()
list(APPEND add_cppflags -fvisibility=hidden)
list(APPEND add_cppflags -std=c++11 -O2)
list(APPEND add_cppflags -Wall -Wextra)
list(APPEND add_cppflags -Wno-unused-but-set-variable)
list(APPEND add_cppflags -Wno-unused-parameter)

# include
list(APPEND add_includes /usr/include)
list(APPEND add_libraries pthread uv)


add_executable(uv_async async_threads.cpp)
target_compile_definitions(uv_async PRIVATE ${add_defines})
target_compile_options(uv_async PRIVATE ${add_cppflags})
target_include_directories(uv_async PRIVATE ${add_includes})
target_link_libraries(uv_async ${add_libraries})

add_executable(bench_async benchmark_async_threads.cpp)
target_compile_definitions(bench_async PRIVATE ${add_defines})
target_compile_options(bench_async PRIVATE ${add_cppflags})
target_include_directories(bench_async PRIVATE ${add_includes})
target_link_libraries(bench_async ${add_libraries})
