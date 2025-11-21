add_executable(main_client main_client.cpp)

target_link_libraries(main_client PRIVATE mmki_lib)

target_include_directories(main_client PRIVATE ${INCLUDES})
set_target_properties(main_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/simple")

if(WIN32)
    target_link_libraries(main_client PRIVATE ws2_32)
    target_compile_definitions(main_client PRIVATE _WIN32_WINNT=0x0A00)
endif()

add_executable(main_server main_server.cpp)

target_link_libraries(main_server PRIVATE mmki_lib)

target_include_directories(main_server PRIVATE ${INCLUDES})
set_target_properties(main_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/simple")

if(WIN32)
    target_link_libraries(main_server PRIVATE ws2_32)
    target_compile_definitions(main_server PRIVATE _WIN32_WINNT=0x0A00)
endif()