cmake_minimum_required(VERSION 2.8)
project(quantlib_demo)

 
list(APPEND add_cppflags -Wall -Wextra -std=c++11)
list(APPEND add_cppflags -Wno-unused-parameter)
# list(APPEND add_cppflags -fPIC -shared)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  list(APPEND add_cppflags -O2 -DNDEBUG)
else()
  list(APPEND add_cppflags -O2 -g)
endif()
 
list(APPEND add_libraries QuantLib)
file(GLOB add_sources time/calendars/*.cpp  main.cpp)

add_executable(quantlib_demo ${add_sources})
target_compile_options(quantlib_demo PRIVATE ${add_cppflags})
target_link_libraries(quantlib_demo ${add_libraries})

#if(UNIX)
#  install (TARGETS quantlib_demo RUNTIME DESTINATION /usr/bin)
#endif()