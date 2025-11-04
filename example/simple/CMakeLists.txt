add_executable(main_client main_client.cpp)
target_include_directories(main_client PRIVATE ${INCLUDES})
set_target_properties(main_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/simple")

add_executable(main_server main_server.cpp)
target_include_directories(main_server PRIVATE ${INCLUDES})
set_target_properties(main_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/simple")

# Platform-specific settings
if(WIN32)
    target_compile_definitions(main_client PRIVATE _WIN32_WINNT=0x0A00)
    target_link_libraries(main_client PRIVATE mmki_lib ws2_32)
    target_link_libraries(main_server PRIVATE mmki_lib ws2_32)
elseif(APPLE)
    target_link_libraries(main_client PRIVATE mmki_lib)
    target_link_libraries(main_server PRIVATE mmki_lib)
    # Add macOS-specific frameworks if needed
    target_link_libraries(main_client PRIVATE "-framework CoreGraphics" "-framework Carbon")
    target_link_libraries(main_server PRIVATE "-framework CoreGraphics" "-framework Carbon")
else()
    target_link_libraries(main_client PRIVATE mmki_lib)
    target_link_libraries(main_server PRIVATE mmki_lib)
endif()
