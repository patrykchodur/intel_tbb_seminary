cmake_minimum_required (VERSION 3.0.0)
project (przyklad11)

add_executable(przyklad11 main.cpp)
add_compile_options( -Wall -g )
set(CMAKE_CXX_FLAGS "-std=c++17 -ltbb") 
