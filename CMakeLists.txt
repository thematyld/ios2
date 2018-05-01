cmake_minimum_required(VERSION 3.10)
project(Proj2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread")

add_executable(Proj2 proj2.c)