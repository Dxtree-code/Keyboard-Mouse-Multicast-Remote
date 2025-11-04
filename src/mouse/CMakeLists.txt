target_sources(mmki_lib PRIVATE
    mouse_capture.cpp
    mouse_state.cpp
    mouse.cpp
    mouse_factory.cpp
)

if(WIN32)
    target_sources(mmki_lib PRIVATE mouse_windows.cpp)
elseif(APPLE)
    target_sources(mmki_lib PRIVATE mouse_mac.cpp)
endif()