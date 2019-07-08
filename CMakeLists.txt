cmake_minimum_required(VERSION 3.5)
project(subc)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB SOURCE *.c *.h)

add_executable(subc ${SOURCE})
