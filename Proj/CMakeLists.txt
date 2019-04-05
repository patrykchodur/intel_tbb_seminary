cmake_minimum_required (VERSION 3.0.0)
project (zadania)

add_executable(zadanie1 zadanie1/main.cpp)
add_executable(zadanie2 zadanie2/main.cpp)
add_executable(zadanie3 zadanie3/main.cpp)
add_compile_options( -Wall -g )
set(CMAKE_CXX_FLAGS "-std=c++17 -ltbb") 
