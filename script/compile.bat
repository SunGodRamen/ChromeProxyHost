@echo off

REM Set directories and filenames
IF NOT DEFINED EXECUTABLE_NAME (
    echo EXECUTABLE_NAME is not defined.
    exit /b 1
)

IF NOT DEFINED DEPENDENCY_LIST (
    echo DEPENDENCY_LIST is not defined.
    exit /b 1
)

REM Remove old executable
if exist bin\%EXECUTABLE_NAME% (
    echo Removing old executable...
    del bin\%EXECUTABLE_NAME%
)

REM Compile and link all source files directly into the final executable
echo Compiling and linking source files...
wsl x86_64-w64-mingw32-gcc %DEPENDENCY_LIST% -Isrc -lws2_32 -o bin/%EXECUTABLE_NAME%

REM Check if the compilation was successful.
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b 1
)

echo Compilation successful.
exit /b 0