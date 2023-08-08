@echo off

IF NOT DEFINED EXECUTABLE_NAME (
    echo EXECUTABLE_NAME is not defined.
    exit /b 1
)

:: Check if the program is running and terminate it
tasklist | find /I "%EXECUTABLE_NAME%" > nul
if %ERRORLEVEL% NEQ 1 (
    echo Program is running. Terminating it now.
    taskkill /F /IM "%EXECUTABLE_NAME%" > nul
)

exit /b 0
