@echo off
setlocal enabledelayedexpansion

echo ============================================
echo     AUTOMATIC DEBUG PROCESSING
echo ============================================
echo.

:: Check if executable exists
if not exist "build\Release\ShadowSegmentation.exe" (
    echo ERROR: ShadowSegmentation.exe not found!
    echo Please run build.bat first
    pause
    exit /b 1
)

:: Check if examples folder exists
if not exist "examples" (
    echo ERROR: examples folder not found!
    echo Please add example images to examples/ folder
    pause
    exit /b 1
)

:: Clean previous results
echo [1/2] Cleaning previous debug results...
rmdir /s /q debug_output 2>nul
rmdir /s /q build\Release\debug_output 2>nul
echo   ✓ Previous results cleaned

:: Run batch processing
echo [2/2] Starting automatic batch processing...
echo.

cd build\Release
ShadowSegmentation.exe --batch

:: Move debug_output to project root
if exist "debug_output" (
    cd ..\..
    rmdir /s /q debug_output 2>nul
    move "build\Release\debug_output" "debug_output"
    echo   ✓ Debug results moved to project root
) else (
    cd ..\..
)

echo.
echo ============================================
echo        AUTOMATIC PROCESSING COMPLETED
echo ============================================
echo.

:: Check results
if exist "debug_output" (
    echo Results analysis:
    set /a folder_count=0
    for /d %%d in (debug_output\*) do (
        set /a folder_count+=1
        set /a file_count=0
        for %%f in ("%%d\step*.jpg") do set /a file_count+=1
        echo   %%~nd: !file_count! steps
    )
    
    echo.
    echo ✓ Total folders created: !folder_count!
    echo ✓ All results ready for LaTeX report!
    echo.
    echo Opening results folder...
    explorer debug_output
) else (
    echo ERROR: No debug output created!
    echo Check if examples folder contains images
)

pause
