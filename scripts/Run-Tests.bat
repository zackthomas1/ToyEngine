@echo off
echo ====================================
echo    ToyEngine Test Runner
echo ====================================
echo.

REM Check if Tests executable exists
if not exist "bin\Debug-windows-x86_64\Tests\Tests.exe" (
    echo ERROR: Tests executable not found!
    echo Please build the Tests project first.
    echo.
    echo To build:
    echo 1. Run scripts\Win-GenProjects.bat
    echo 2. Open ToyEngine.sln in Visual Studio
    echo 3. Build the Tests project
    pause
    exit /b 1
)

echo Running ToyEngine Tests...
echo.

REM Run all tests
"bin\Debug-windows-x86_64\Tests\Tests.exe" --gtest_color=yes

echo.
echo ====================================
echo Test execution completed.
echo ====================================
pause