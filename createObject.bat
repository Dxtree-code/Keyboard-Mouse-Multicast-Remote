@echo off
REM Set object directory
set OBJDIR=object

REM Create object directory if it doesn’t exist
if not exist %OBJDIR% mkdir %OBJDIR%

REM Compile C++ source files into objects
g++ -std=c++17 -c mouse.cpp -o %OBJDIR%\mouse.o
g++ -std=c++17 -c MouseCapture.cpp -o %OBJDIR%\MouseCapture.o
g++ -std=c++17 -c mouse_tracker.cpp -o %OBJDIR%\mouse_tracker.o

REM Archive into a static library
ar rcs %OBJDIR%\libMouseTracker.a %OBJDIR%\mouse.o %OBJDIR%\MouseCapture.o %OBJDIR%\mouse_tracker.o

echo.
echo Static library created: %OBJDIR%\libMouseTracker.a
