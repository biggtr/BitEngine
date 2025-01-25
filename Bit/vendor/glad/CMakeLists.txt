cmake_minimum_required(VERSION 3.30)


add_library(glad STATIC src/glad.c)

target_include_directories(glad PUBLIC include)
