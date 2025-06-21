@echo off
cd /d "%~dp0"

echo Cleaning previous build...
if exist build (
    rmdir /s /q build
    echo Previous build folder removed.
)

echo Creating new build folder...
if not exist build mkdir build
cd build

echo Configuring project...
cmake .. -G "Visual Studio 17 2022" -A x64

echo Building project...
cmake --build . --config Release

echo.
echo Build completed successfully!
echo.
echo Usage instructions:
echo   Command line: cd build\Release ^&^& ShadowSegmentation.exe examples\shadow1.jpg
echo   Interactive:  cd build\Release ^&^& ShadowSegmentation.exe
echo   Debug batch:  run_debug.bat
echo.
pause
