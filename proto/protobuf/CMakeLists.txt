cmake_minimum_required(VERSION 3.0)
project(adcproto)

## generate cpp 
execute_process(COMMAND sh ${PROJECT_SOURCE_DIR}/build_cpp.sh ${PROJECT_SOURCE_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../build)

if(MSVC)
  list(APPEND add_cppflags /W4)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
     list(APPEND add_cppflags /O2)
  endif()
  # list(APPEND add_cppflags  /std:c++11)
elseif(CMAKE_C_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND add_cppflags -g)
  endif()
  #list(APPEND add_cppflags -fvisibility=hidden)
  list(APPEND add_cppflags --std=c++11 -O2)
  list(APPEND add_cppflags -Wall -Wextra)
  list(APPEND add_cppflags -Wno-unused-but-set-variable)
  list(APPEND add_cppflags -Wno-unused-parameter)
endif()

if(MSVC)
  if(NOT DEFINED ENV{3RDPARTY})
      message(FATAL_ERROR "not defined system environment variable:3RDPARTY") 
  else()
     list(APPEND add_includes $ENV{3RDPARTY}/protobuf/src)
  endif()
  list(APPEND add_libraries  $ENV{3RDPARTY}/protobuf/libprotobuf.lib)
else()
  list(APPEND add_includes /usr/include /usr/local/include)
  list(APPEND add_libraries pthread protobuf)
endif()

file(GLOB project_sources ${PROJECT_SOURCE_DIR}/*.proto
                          ${PROJECT_SOURCE_DIR}/gen_cpp/*.h
                          ${PROJECT_SOURCE_DIR}/gen_cpp/*.cpp)

## build as static library on Windows
if(MSVC)
  add_library(adcproto STATIC ${project_sources})
else()
  add_library(adcproto SHARED ${project_sources})
endif()


target_compile_options(adcproto PRIVATE ${add_cppflags})
target_include_directories(adcproto PRIVATE ${add_includes})
target_link_libraries(adcproto ${add_libraries})

if(UNIX)
  install (TARGETS adcproto LIBRARY DESTINATION /usr/lib/uvframe)
endif()