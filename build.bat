@echo off
title BOMBANANA IRL - Build
cd /d D:\hack-club-projects

set PIO=%USERPROFILE%\.platformio\penv\Scripts\python.exe

for /L %%i in (1,1,8) do (
    echo.
    echo ==================================================
    echo   Attempt %%i of 8
    echo ==================================================
    "%PIO%" -m platformio run
    if not errorlevel 1 goto success
    echo.
    echo [FAILED] Connection dropped again. Retrying in 10s...
    timeout /t 10 /nobreak >nul
)

echo.
echo ==================================================
echo   All attempts failed. Double-click this file later.
echo ==================================================
pause
exit /b 1

:success
echo.
echo ==================================================
echo   BUILD SUCCESSFUL - firmware is ready
echo ==================================================
pause
exit /b 0
