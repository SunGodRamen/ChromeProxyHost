@echo off

:: Set values for the script
set "EXECUTABLE_NAME=chrome_proxy.exe"
set "DEPENDENCY_LIST=src/logger/logger.c src/stdio_interface/stdio_interface.c src/tcp_server/tcp_server.c src/main.c"
set "APPLICATION_PATH=C:\Program Files\Chrome Proxy"
set "MANIFEST_PATH=%APPLICATION_PATH%\manifest.json"
set "COMPANY_NAME=com.ra.chrome_proxy"

:: Call the termination script.
CALL terminate.bat
IF %ERRORLEVEL% NEQ 0 (
    echo Not compiling files.
    exit /b 1
)

:: Call the compilation script.
CALL compile.bat
IF %ERRORLEVEL% NEQ 0 (
    echo Not copying files.
    exit /b 1
)

:: Check if program directory exists, if not, create it.
IF NOT EXIST "%APPLICATION_PATH%" (
    echo Directory not found, creating...
    mkdir "%APPLICATION_PATH%"
)

:: Copy the manifest and executable to the target directory.
echo Copying files...
copy /Y "src\manifest.json" "%MANIFEST_PATH%"
copy /Y bin\%EXECUTABLE_NAME% "%APPLICATION_PATH%\%EXECUTABLE_NAME%"

:: Check if the copying was successful.
IF %ERRORLEVEL% NEQ 0 (
    echo Failed to copy the files.
    exit /b 1
)

:: Check for the registry entry
set "REGISTRY_KEY=HKCU\Software\Google\Chrome\NativeMessagingHosts\%COMPANY_NAME%"
REG QUERY %REGISTRY_KEY% /ve > nul
IF %ERRORLEVEL% NEQ 0 (
    echo Registry entry not found, adding...

    :: Add the manifest to the registry:
    :: HKCU\Software\Google\Chrome\NativeMessagingHosts\%COMPANY_NAME%
    :: (Default) = %MANIFEST_PATH%

    REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\%COMPANY_NAME%" /ve /t REG_SZ /d "%MANIFEST_PATH%" /f
)

echo Operation successful.
