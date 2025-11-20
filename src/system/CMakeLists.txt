target_sources(mmki_lib PRIVATE
    system.cpp
)

# Windows-specific system sources
if(WIN32)
    target_sources(mmki_lib PRIVATE display.cpp)
endif()