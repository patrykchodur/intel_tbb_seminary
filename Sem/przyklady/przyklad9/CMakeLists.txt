cmake_minimum_required (VERSION 3.0.0)
project (przyklad9)

add_executable(przyklad9 main.cpp)
add_compile_options( -Wall -g )
set(CMAKE_CXX_FLAGS "-std=c++17 -ltbb") 
