@echo off
REM -- Compile all cpp files with g++
g++ -std=c++17 -Wall -O2 mouse.cpp MouseCapture.cpp windowsMouseTracker.cpp -o MouseCapture.exe

REM -- Check if compilation succeeded
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)

echo Compilation succeeded.
pause
