@echo off
title BOMBANANA IRL - local server
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0tools\serve.ps1"
echo.
echo Server stopped. Press any key to close.
pause >nul
