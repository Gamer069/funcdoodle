@echo off
mkdir bin
if errorlevel 1 exit /b -1

cd bin
if errorlevel 1 exit /b -1

cmake ..
if errorlevel 1 exit /b -1

where msbuild >nul 2>nul
if %errorlevel% neq 0 (
    echo "Run this from developer command prompt, instead of regular command prompt, or just put msbuild.exe in your path"
    pause
    exit /b 1
)

msbuild .\FuncDoodle.sln
if errorlevel 1 exit /b -1

xcopy /E /I ..\assets .\assets
if errorlevel 1 exit /b -1

xcopy /E /I ..\themes .\themes
if errorlevel 1 exit /b -1
