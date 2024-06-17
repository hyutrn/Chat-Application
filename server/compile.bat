@echo off

REM Compile the source files
g++ server.cpp -o server.exe -lws2_32

REM Check if compilation was successful
if %errorlevel% equ 0 (
    echo Compilation successful.
) else (
    echo Compilation failed.
)

