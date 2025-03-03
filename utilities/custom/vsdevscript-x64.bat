@echo off
@start /b cmd /c %~dp0vsdevcmd-x64.bat
ping 127.0.0.1 -n 21 > nul
@echo on
