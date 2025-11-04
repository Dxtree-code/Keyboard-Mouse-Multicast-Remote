cmake_minimum_required(VERSION 3.16)

project(KeyboardMouseMulticast)

add_library(mmki_lib STATIC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2/src
)
target_include_directories(mmki_lib PRIVATE ${INCLUDES})

# Only set Windows-specific definitions on Windows
if(WIN32)
    target_compile_definitions(mmki_lib PRIVATE _WIN32_WINNT=0x0A00)
endif()

target_compile_options(mmki_lib PRIVATE -Wall)

# Ensure the compiler actually uses C++20 (adds -std=... when supported)
target_compile_features(mmki_lib PRIVATE cxx_std_20)

add_subdirectory(src)

add_subdirectory(example/simple)

add_subdirectory(example/cgo_compatible_extern)