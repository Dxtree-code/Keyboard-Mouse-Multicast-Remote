# Debug script to see what CMake is doing
# Run this from build directory: cmake -P ../debug_build.cmake

message(STATUS "=== CMAKE DEBUG INFO ===")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# Check if source files exist
set(TEST_FILES
    "${CMAKE_SOURCE_DIR}/src/tools/serializer.cpp"
    "${CMAKE_SOURCE_DIR}/src/system/system.cpp"
    "${CMAKE_SOURCE_DIR}/src/mouse/mouse_capture.cpp"
    "${CMAKE_SOURCE_DIR}/src/keyboard/keyboard_capture.cpp"
)

foreach(file ${TEST_FILES})
    if(EXISTS ${file})
        message(STATUS "✓ Found: ${file}")
    else()
        message(WARNING "✗ Missing: ${file}")
    endif()
endforeach()

# Check platform detection
if(WIN32)
    message(STATUS "Platform: Windows")
elseif(APPLE)
    message(STATUS "Platform: macOS")
else()
    message(STATUS "Platform: Other/Linux")
endif()