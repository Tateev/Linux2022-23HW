cmake_minimum_required(VERSION 3.23)
project(cp)


add_executable(cp main.cpp)

target_include_directories(cp PUBLIC ../utils)