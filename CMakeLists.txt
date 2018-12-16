cmake_minimum_required(VERSION 3.12)
project(timetable C)

set(CMAKE_C_STANDARD 99)

add_executable(timetable main.c utils.c utils.h structures.h modules.c modules.h schemes.c schemes.h)