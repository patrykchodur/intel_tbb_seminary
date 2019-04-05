cmake_minimum_required (VERSION 3.0.0)
project (zadanie3)

add_executable(zadanie3 main.cpp)
add_compile_options( -Wall -g )
set(CMAKE_CXX_FLAGS "-std=c++17 -ltbb") 
