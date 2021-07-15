cmake_minimum_required(VERSION 3.19)
project(SingleProject3 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_C_STANDARD 99)

add_executable(SingleProject3 main.c semaphore.h mutex.h test/semaphore/test_semaphore.c)
#add_executable(SingleProject3 main.c semaphore.h read_lock.h main_2.c)