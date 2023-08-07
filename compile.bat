REM Set directories and filenames
set SRC_DIR=src
set BIN_DIR=bin
set EXECUTABLE_NAME=chrome_proxy.exe

REM Remove old executable
if exist %BIN_DIR%\%EXECUTABLE_NAME% (
    echo Removing old executable...
    del %BIN_DIR%\%EXECUTABLE_NAME%
)

REM Compile individual C source files into object files
echo Compiling individual source files...
wsl x86_64-w64-mingw32-gcc -c %SRC_DIR%/stdio_interface/stdio_interface.c -o %SRC_DIR%/stdio_interface/stdio_interface.o -I%SRC_DIR%
wsl x86_64-w64-mingw32-gcc -c %SRC_DIR%/tcp_server/tcp_server.c -o %SRC_DIR%/tcp_server/tcp_server.o -I%SRC_DIR%
wsl x86_64-w64-mingw32-gcc -c %SRC_DIR%/main.c -o %SRC_DIR%/main.o -I%SRC_DIR%/stdio_interface -I%SRC_DIR%/tcp_server

REM Link all object files together to create the final executable
echo Linking object files...
wsl x86_64-w64-mingw32-gcc %SRC_DIR%/main.o %SRC_DIR%/stdio_interface/stdio_interface.o %SRC_DIR%/tcp_server/tcp_server.o -lws2_32 -o %BIN_DIR%/%EXECUTABLE_NAME%

REM Clean up intermediate object files
echo Cleaning up...
del %SRC_DIR%\main.o
del %SRC_DIR%\stdio_interface\stdio_interface.o
del %SRC_DIR%\tcp_server\tcp_server.o

REM Check if the compilation was successful.
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b 1
)

echo Compilation successful.
