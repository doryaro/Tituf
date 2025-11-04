@echo off
REM Get the directory of this script
set "SCRIPT_DIR=%~dp0"
echo Script is running from: %SCRIPT_DIR%

REM Change to that directory (optional, ensures paths are correct)
cd /d "%SCRIPT_DIR%"

call vendor\bin\premake\premake5.exe vs2022  

REM Remove the imgui.ini file if it exists
if exist "%SCRIPT_DIR%Sandbox\imgui.ini" (
    echo Removing file: %SCRIPT_DIR%Sandbox\imgui.ini
    del /q "%SCRIPT_DIR%Sandbox\imgui.ini"
) else (
    echo File not found: %SCRIPT_DIR%Sandbox\imgui.ini
)

PAUSE
