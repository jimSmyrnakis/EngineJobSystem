@echo off
setlocal EnableDelayedExpansion

set "ROOT_DIR=%~dp0"
cd /d "%ROOT_DIR%"

where premake5 >nul 2>nul
if errorlevel 1 (
    echo premake5 is required but was not found in PATH.
    exit /b 1
)

set "ACTION=%~1"
if "%ACTION%"=="" set "ACTION=build"

set "MODE=%~2"
if "%MODE%"=="" set "MODE=unit"

rem ===================================
rem Detect platform
rem ===================================

set "PLATFORM=x64"

if /I "%PROCESSOR_ARCHITECTURE%"=="x86" set "PLATFORM=x86"
if /I "%PROCESSOR_ARCHITECTURE%"=="amd64" set "PLATFORM=x64"
if /I "%PROCESSOR_ARCHITECTURE%"=="arm" set "PLATFORM=arm32"
if /I "%PROCESSOR_ARCHITECTURE%"=="arm64" set "PLATFORM=arm64"

if /I "%PROCESSOR_ARCHITEW6432%"=="amd64" set "PLATFORM=x64"
if /I "%PROCESSOR_ARCHITEW6432%"=="arm64" set "PLATFORM=arm64"
if /I "%PROCESSOR_ARCHITEW6432%"=="arm" set "PLATFORM=arm32"


rem ===================================
rem Map names
rem ===================================

set "PLAT=%PLATFORM%"
set "VSPLATFORM=%PLATFORM%"


if /I "%PLATFORM%"=="x86" (
    set "PLAT=x86"
    set "VSPLATFORM=Win32"
)

if /I "%PLATFORM%"=="x64" (
    set "PLAT=x64"
    set "VSPLATFORM=x64"
)

if /I "%PLATFORM%"=="arm32" (
    set "PLAT=ARM32"
    set "VSPLATFORM=ARM"
)

if /I "%PLATFORM%"=="arm64" (
    set "PLAT=ARM64"
    set "VSPLATFORM=ARM64"
)


rem ===================================
rem Actions
rem ===================================


if /I "%ACTION%"=="configure" (

    call :call_premake
    goto :eof

)


if /I "%ACTION%"=="build" (

    call :call_premake

    msbuild Engine.sln /p:Configuration=Debug /p:Platform=%VSPLATFORM%

    msbuild Engine.sln /p:Configuration=Release /p:Platform=%VSPLATFORM%

    msbuild Engine.sln /p:Configuration=Dist /p:Platform=%VSPLATFORM%

    goto :eof

)


if /I "%ACTION%"=="clean" (

    msbuild Engine.sln /t:Clean /p:Platform=%VSPLATFORM%

    goto :eof

)


if /I "%ACTION%"=="setup" (

    call :call_premake
    msbuild Engine.sln /p:Configuration=Debug /p:Platform=%VSPLATFORM%

    msbuild Engine.sln /p:Configuration=Release /p:Platform=%VSPLATFORM%

    msbuild Engine.sln /p:Configuration=Dist /p:Platform=%VSPLATFORM%

    goto :eof

)


if /I "%ACTION%"=="tests" (

    for /r "bin\tests" %%E in (*Tests.exe) do (

        echo ====================================
        echo Running %%E
        echo ====================================

        "%%E"

        if errorlevel 1 (
            echo Test failed: %%E
            exit /b 1
        )

    )

    goto :eof

)


if /I "%ACTION%"=="coverage" (

    call :coverage
    goto :eof

)

if /I "%ACTION%"=="performance" (

    call :performance
    goto :eof

)


echo Usage:
echo build.bat [configure^|build^|clean^|setup^|tests^|coverage^|performance]

exit /b 2



rem ===================================
rem Coverage
rem ===================================

:coverage

set CONFIGS=Debug Release Dist


for %%C in (%CONFIGS%) do (

    echo ====================================
    echo Coverage Windows%PLAT% %%C
    echo ====================================


    if not exist "coverage\Windows\%PLAT%\%%C" (
        mkdir "coverage\Windows\%PLAT%\%%C"
    )


    for /r "bin\tests\Windows\%PLAT%\%%C" %%E in (*Tests.exe) do (

        echo Running coverage for %%E


        OpenCppCoverage ^
        --export_type=cobertura:coverage\Windows\%PLAT%\%%C\%%~nE.xml ^
        -- %%E


    )

)

goto :eof


rem ===================================
rem Performance
rem ===================================

:performance

set CONFIGS=Release Dist


for %%C in (%CONFIGS%) do (

    echo ====================================
    echo Performance Windows%PLAT% %%C
    echo ====================================


    if not exist "reports\perf\%PLATFORM%\%%C" (
        mkdir "reports\perf\%PLATFORM%\%%C"
    )


    for /r "bin\perf\%PLATFORM%\%%C" %%E in (*.exe) do (

        echo Running %%E


        set "NAME=%%~nE"


        "%%E" ^
        --benchmark_format=json ^
        "--benchmark_out=reports\perf\%PLATFORM%\%%C\!NAME!.json"


        python perf.py ^
        "reports\perf\%PLATFORM%\%%C\!NAME!.json"

    )

)

goto :eof

rem ===================================
rem call_premake
rem ===================================

:call_premake

set CONFIGS=Release Dist


    echo ====================================
    echo Call premake
    echo ====================================

    if /I "%MODE%"=="unit"  (
        premake5 --file=premakeWorkspace.lua vs2022
    )



goto :eof
