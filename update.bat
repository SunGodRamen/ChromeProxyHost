@echo off

REM Call the compilation script.
CALL compile.bat
REM Check if the compilation was successful.
IF %ERRORLEVEL% NEQ 0 (
    echo Not copying files.
    exit /b 1
)

REM Check if program directory exists, if not, create it.
IF NOT EXIST "C:\Program Files\Chrome Proxy" (
    echo Directory not found, creating...
    mkdir "C:\Program Files\Chrome Proxy"
)

REM Copy the manifest and executable to the target directory.
echo Copying files...
copy /Y "src\manifest.json" "C:\Program Files\Chrome Proxy\manifest.json"
copy /Y "bin\chrome_proxy.exe" "C:\Program Files\Chrome Proxy\chrome_proxy.exe"

REM Check if the copying was successful.
IF %ERRORLEVEL% NEQ 0 (
    echo Failed to copy the files.
    exit /b 1
)

REM Check for the registry entry
REG QUERY "HKCU\Software\Google\Chrome\NativeMessagingHosts\com.ra.chrome_proxy"
IF %ERRORLEVEL% NEQ 0 (
    echo Registry entry not found, adding...

    REM Add the manifest to the registry:
    REM HKCU\Software\Google\Chrome\NativeMessagingHosts\com.ra.chrome_proxy
    REM (Default) = C:\Program Files\Chrome Proxy\manifest.json

    REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\com.ra.chrome_proxy" /ve /t REG_SZ /d "C:\Program Files\Chrome Proxy\manifest.json" /f
)

echo Operation successful.
