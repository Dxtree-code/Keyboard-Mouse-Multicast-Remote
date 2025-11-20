add_library(cgo_compatible STATIC 
    extern.cpp
    ${CMAKE_SOURCE_DIR}/src/listener_client.cpp
    ${CMAKE_SOURCE_DIR}/src/track_server.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse_capture.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse_state.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse_factory.cpp
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard.cpp
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_capture.cpp
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_state.cpp
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_factory.cpp
    ${CMAKE_SOURCE_DIR}/src/network/multicast_client.cpp
    ${CMAKE_SOURCE_DIR}/src/network/multicast_server.cpp
    ${CMAKE_SOURCE_DIR}/src/network/helper.cpp
    ${CMAKE_SOURCE_DIR}/src/network/net.cpp
    ${CMAKE_SOURCE_DIR}/src/system/system.cpp
    ${CMAKE_SOURCE_DIR}/src/tools/serializer.cpp
)

# Add platform-specific sources
if(WIN32)
    target_sources(cgo_compatible PRIVATE
        ${CMAKE_SOURCE_DIR}/src/mouse/mouse_windows.cpp
        ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_windows.cpp
        ${CMAKE_SOURCE_DIR}/src/system/display.cpp
    )
elseif(APPLE)
    target_sources(cgo_compatible PRIVATE
        ${CMAKE_SOURCE_DIR}/src/mouse/mouse_mac.cpp
        ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_mac.cpp
    )
endif()

target_include_directories(cgo_compatible PRIVATE ${INCLUDES} ./)

if(WIN32)
    target_compile_definitions(cgo_compatible PRIVATE _WIN32_WINNT=0x0A00)
elseif(APPLE)
    # Add macOS-specific frameworks (these will be needed when linking with Go)
    # Note: Go will need to link these frameworks: -framework CoreGraphics -framework Carbon
endif()

set_target_properties(cgo_compatible PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/cgo_compatible"
    POSITION_INDEPENDENT_CODE ON
)
target_compile_options(cgo_compatible PRIVATE -Wall)

target_compile_features(cgo_compatible PRIVATE cxx_std_20)