@echo off

IF NOT DEFINED EXECUTABLE_NAME (
    echo EXECUTABLE_NAME is not defined.
    exit /b 1
)

:: Check if the program is running and terminate it
tasklist | find /I "%EXECUTABLE_NAME%" > nul
if %ERRORLEVEL% NEQ 1 (
    echo Program is running. Terminating it now.
    taskkill /F /IM "%EXECUTABLE_NAME%"
    timeout /T 3 /NOBREAK > nul
    :: Wait to see if the program is restarting
    tasklist | find /I "%EXECUTABLE_NAME%" > nul
    if %ERRORLEVEL% NEQ 0 (
        echo Something is restarting the program. Disable the extension and try again.
        exit /b 1
    )
)

exit /b 0
