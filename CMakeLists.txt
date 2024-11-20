cmake_minimum_required(VERSION 3.29)
project(open_csv C)

set(CMAKE_C_STANDARD 99)

add_executable(open_csv main.c
        open_csv.c
        open_csv.h
        inc/utils.h)
