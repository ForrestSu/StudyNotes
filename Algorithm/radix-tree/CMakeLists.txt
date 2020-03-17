cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(radix-tree VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND add_cppflags -O0 -g -Wall -Wextra)

## radix-tree
add_library(radix-tree radix_tree.cpp radix_tree.hpp)
target_compile_options(radix-tree PRIVATE ${add_cppflags})


## main
add_executable(main main.cpp)
target_link_libraries(main radix-tree)
target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(main PRIVATE ${add_cppflags})