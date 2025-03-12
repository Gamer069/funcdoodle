@echo off
if not exist "build" mkdir build
if errorlevel 1 exit /b -1

cd build
if errorlevel 1 exit /b -1

cmake ..
if errorlevel 1 exit /b -1

set /p vspath=Enter MSBuild.exe path:
"%vspath%" .\FuncDoodle.sln
if errorlevel 1 exit /b -1

xcopy /q /E /I ..\assets .\assets
if errorlevel 1 exit /b -1

copy ..\assets\keys.txt .\assets\keys.txt
if errorlevel 1 exit /b -1

copy ..\assets\icon.png .\assets\icon.png
if errorlevel 1 exit /b -1

cd ..
if errorlevel 1 exit /b -1