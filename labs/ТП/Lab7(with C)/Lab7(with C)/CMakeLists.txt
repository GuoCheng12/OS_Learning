cmake_minimum_required(VERSION 3.23)
project(Lab7_with_C_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab7_with_C_
        main.cpp
        mtuThread.c
        mtuThread.h
        mtuThreadCore.c
        mtuThreadErrmsg.h
        mtuThreadMessage.c
        mtuThreadPrototypes.h
        mtuThreadQueue.c
        smoker.cpp
        smoker.h
        smokers-sem.c
        test.c)
