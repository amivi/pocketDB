cmake_minimum_required(VERSION 3.8)
project(pocketDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES ../pocketDB2/ input.c treemap.c treemap.h input.h main.c output.c output.h disk_level.c disk_level.h engine_manager.c engine_manager.h merger.c merger.h)
add_executable(pocketDB ${SOURCE_FILES})