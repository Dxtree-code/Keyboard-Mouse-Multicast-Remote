cmake_minimum_required(VERSION 4.0)

add_library(mmki_lib STATIC)

target_include_directories(mmki_lib PRIVATE ./include)
target_include_directories(mmki_lib PRIVATE ./extern)

add_subdirectory(src)