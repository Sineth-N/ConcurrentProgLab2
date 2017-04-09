cmake_minimum_required(VERSION 3.7)
project(Lab2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c ReadWriteLock.c)
add_executable(Lab2 ${SOURCE_FILES})