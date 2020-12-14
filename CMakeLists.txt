cmake_minimum_required(VERSION 2.8)
project( TAREA2 )

set(CMAKE_CXX_COMPILER "/usr/bin/gcc")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable( TAREA2 main.c sudokuUtils.h)
