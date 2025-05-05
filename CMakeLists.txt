cmake_minimum_required(VERSION 3.30)
project(CProgrammingProject C)

set(CMAKE_C_STANDARD 23)

include_directories(include)
add_executable(CProgrammingProject src/main.c src/data.c)
