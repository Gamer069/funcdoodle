@echo off
mkdir build
if errorlevel 1 exit /b -1

cd build
if errorlevel 1 exit /b -1

cmake ..
if errorlevel 1 exit /b -1

make
if errorlevel 1 exit /b -1

xcopy /E /I ..\assets .
if errorlevel 1 exit /b -1
