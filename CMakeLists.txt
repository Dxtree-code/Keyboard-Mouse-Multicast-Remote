cmake_minimum_required(VERSION 3.20) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_library(mmki_lib STATIC)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/asio/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2/src
)
target_include_directories(mmki_lib PRIVATE ${INCLUDES})

if(WIN32)
    target_compile_definitions(mmki_lib PRIVATE _WIN32_WINNT=0x0A00)
endif()

target_compile_options(mmki_lib PRIVATE -Wall)
if(APPLE)
    find_library(CARBON_LIB Carbon)
    find_library(COCOA_LIB Cocoa)
    find_library(COREGRAPHICS_LIB CoreGraphics)
    find_library(IOKIT_LIB IOKit)
    find_library(COREFOUNDATION_LIB CoreFoundation)

    # Use PUBLIC so that any executable (like main_client) 
    # that links mmki_lib AUTOMATICALLY links these too.
    target_link_libraries(mmki_lib PUBLIC 
        ${CARBON_LIB} 
        ${COCOA_LIB} 
        ${COREGRAPHICS_LIB} 
        ${IOKIT_LIB}
        ${COREFOUNDATION_LIB}
    )
endif()
add_subdirectory(src)
add_subdirectory(example/simple)
add_subdirectory(example/cgo_compatible_extern)