cmake_minimum_required (VERSION 3.0.0)
project (przyklad4)

add_executable(przyklad4 main.cpp)
add_compile_options( -Wall -g )
set(CMAKE_CXX_FLAGS "-std=c++17 -ltbb") 
