
target_sources(
    mmki_lib PRIVATE
    keyboard_capture.cpp
    keyboard_state.cpp
    keyboard.cpp
    keyboard_factory.cpp
)

if(WIN32)
    target_sources(mmki_lib PRIVATE keyboard_windows.cpp)
elseif(APPLE)
    target_sources(mmki_lib PRIVATE keyboard_mac.cpp)
endif()

