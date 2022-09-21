cmake_minimum_required(VERSION 3.23)
project(user_thread C)

set(CMAKE_C_STANDARD 99)

add_executable(user_thread main.c Queue.h)
