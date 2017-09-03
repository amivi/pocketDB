cmake_minimum_required(VERSION 3.8)
project(pocketDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c input.c treemap.c treemap.h input.h)
add_executable(pocketDB ${SOURCE_FILES})