@echo off
if not exist build mkdir build
cd build

cmake ..
cmake --build . --config Release

echo Build completed. Run ShadowSegmentation.exe to start the application.
pause
