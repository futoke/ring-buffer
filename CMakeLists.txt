cmake_minimum_required(VERSION 3.0)
project(ring_buffer C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(SOURCE_FILES main.c)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
if(CMAKE_USE_PTHREADS_INIT)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
endif()

add_executable(ring_buffer ${SOURCE_FILES})
target_link_libraries( ring_buffer ${CMAKE_THREAD_LIBS_INIT} )