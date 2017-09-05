cmake_minimum_required(VERSION 3.8)
project(pocketDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES ../pocketDB2/input.c treemap.c treemap.h input.h main.c)
add_executable(pocketDB ${SOURCE_FILES})