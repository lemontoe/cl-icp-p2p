@echo off
echo Installing required Python packages...

:: Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Python is not installed. Please install Python and add it to your PATH.
    pause
    exit /b
)

:: Install required packages
pip install colorama

echo Requirements installed successfully!
pause