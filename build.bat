@echo off
mkdir bin
if errorlevel 1 exit /b -1

cd bin
if errorlevel 1 exit /b -1

cmake ..
if errorlevel 1 exit /b -1

"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" .\FuncDoodle.sln
if errorlevel 1 exit /b -1

xcopy /E /I ..\assets .\assets
if errorlevel 1 exit /b -1

xcopy /E /I ..\themes .\themes
if errorlevel 1 exit /b -1