cmake_minimum_required(VERSION 4.0)

add_library(mmki_lib STATIC)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2/src
)
target_include_directories(mmki_lib PRIVATE ${INCLUDES})

add_subdirectory(src)

if(WIN32)
    link_libraries(ws_32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()