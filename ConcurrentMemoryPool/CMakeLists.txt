cmake_minimum_required(VERSION 3.23)
project(test1)

set(CMAKE_CXX_STANDARD 14)

add_executable(test1 main.cpp ObjectPool.hpp Common.hpp ThreadCache.hpp ThreadCache.cpp ConcurrentAlloc.hpp CentralCache.hpp CentralCache.cpp)
include_directories(/Users/wuguocheng/CLionProjects/ConcurrentMemoryPool/ThreadCache.hpp)

