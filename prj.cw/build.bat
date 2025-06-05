@echo off
cd /d "%~dp0"

if not exist build mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

echo.
echo Build completed. 
echo Run: cd build\Release && ShadowSegmentation.exe C:\Users\hedge\Downloads\image.jpg
pause
