cmake_minimum_required(VERSION 3.22)
project(exam03 C)

set(CMAKE_C_STANDARD 11)

add_executable(exam03
        "micro_paint (1).c"
        "mini_paint (1).c"
        my_micro.c
        mymicro.c)
