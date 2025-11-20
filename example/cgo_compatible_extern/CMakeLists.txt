# Create the CGO library exactly like mmki_lib, but with extern.cpp added
add_library(cgo_compatible STATIC extern.cpp)

# Add all the same sources that mmki_lib has, by replicating the target_sources calls
target_sources(cgo_compatible PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/listener_client.cpp
    ${CMAKE_SOURCE_DIR}/src/track_server.cpp
)

# Add keyboard sources (mirroring src/keyboard/CMakeLists.txt)
target_sources(cgo_compatible PRIVATE
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_capture.cpp 
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_state.cpp
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard.cpp
    ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_factory.cpp
)

# Add mouse sources (mirroring src/mouse/CMakeLists.txt) 
target_sources(cgo_compatible PRIVATE
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse_capture.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse_state.cpp 
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse/mouse_factory.cpp
)

# Add network sources (mirroring src/network/CMakeLists.txt)
target_sources(cgo_compatible PRIVATE
    ${CMAKE_SOURCE_DIR}/src/network/helper.cpp
    ${CMAKE_SOURCE_DIR}/src/network/multicast_client.cpp
    ${CMAKE_SOURCE_DIR}/src/network/multicast_server.cpp
    ${CMAKE_SOURCE_DIR}/src/network/net.cpp
)

# Add system sources (mirroring src/system/CMakeLists.txt)
target_sources(cgo_compatible PRIVATE
    ${CMAKE_SOURCE_DIR}/src/system/system.cpp
)

# Add Windows-specific system sources
if(WIN32)
    target_sources(cgo_compatible PRIVATE
        ${CMAKE_SOURCE_DIR}/src/system/display.cpp
    )
endif()

# Add tools sources (mirroring src/tools/CMakeLists.txt)
target_sources(cgo_compatible PRIVATE
    ${CMAKE_SOURCE_DIR}/src/tools/serializer.cpp
)

# Add platform-specific sources (mirroring what's in src/*/CMakeLists.txt)
if(WIN32)
    target_sources(cgo_compatible PRIVATE
        ${CMAKE_SOURCE_DIR}/src/mouse/mouse_windows.cpp
        ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_windows.cpp
    )
elseif(APPLE)
    target_sources(cgo_compatible PRIVATE
        ${CMAKE_SOURCE_DIR}/src/mouse/mouse_mac.cpp
        ${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_mac.cpp
    )
endif()

# Set up include directories explicitly
target_include_directories(cgo_compatible PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/asio/include
    ${CMAKE_SOURCE_DIR}/extern/catch2/src
    ./
)

# Platform-specific compile definitions and linking
if(WIN32)
    target_compile_definitions(cgo_compatible PRIVATE _WIN32_WINNT=0x0A00)
elseif(APPLE)
    # Add macOS-specific frameworks (these will be needed when linking with Go)
    # Note: Go will need to link these frameworks: -framework CoreGraphics -framework Carbon
    target_link_libraries(cgo_compatible PRIVATE "-framework CoreGraphics" "-framework Carbon")
endif()

set_target_properties(cgo_compatible PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/cgo_compatible"
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Add verbose compilation options for debugging
target_compile_options(cgo_compatible PRIVATE 
    -Wall 
    -Wextra
    -v  # Verbose output to see what's happening
)

# Ensure C++20 features are available
target_compile_features(cgo_compatible PRIVATE cxx_std_20)