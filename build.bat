@echo off
setlocal

set "ROOT_DIR=%~dp0"
cd /d "%ROOT_DIR%"

where premake5 >nul 2>nul
if errorlevel 1 (
    echo premake5 is required but was not found in PATH.
    exit /b 1
)

set "ACTION=%~1"
if "%ACTION%"=="" set "ACTION=build"

set "PLATFORM=x64"
if /I "%PROCESSOR_ARCHITECTURE%"=="x86" set "PLATFORM=x86"
if /I "%PROCESSOR_ARCHITECTURE%"=="amd64" set "PLATFORM=x64"
if /I "%PROCESSOR_ARCHITECTURE%"=="arm64" set "PLATFORM=arm64"
if /I "%PROCESSOR_ARCHITECTURE%"=="arm" set "PLATFORM=arm32"
if /I "%PROCESSOR_ARCHITEW6432%"=="amd64" set "PLATFORM=x64"
if /I "%PROCESSOR_ARCHITEW6432%"=="arm64" set "PLATFORM=arm64"
if /I "%PROCESSOR_ARCHITEW6432%"=="arm" set "PLATFORM=arm32"

if /I "%ACTION%"=="configure" (
    premake5 vs2022
) else if /I "%ACTION%"=="build" (
    premake5 vs2022
) else if /I "%ACTION%"=="clean" (
    make clean
) else if /I "%ACTION%"=="setup" (
    premake5 vs2022
) else (
    echo Usage: build.bat [configure^|build^|clean^|setup]
    exit /b 2
)
