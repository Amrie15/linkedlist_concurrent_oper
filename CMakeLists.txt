cmake_minimum_required(VERSION 3.13)
project(Linked_list_Kir C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Linked_list_Kir read_write_lock.c)

