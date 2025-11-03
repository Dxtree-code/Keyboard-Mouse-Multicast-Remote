
add_subdirectory(./keyboard)
add_subdirectory(./mouse)
add_subdirectory(./network)
add_subdirectory(./system)
add_subdirectory(./tools)
target_sources(mmki_lib PRIVATE ./listener_client.cpp)
target_sources(mmki_lib PRIVATE ./track_server.cpp)