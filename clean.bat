@echo off
setlocal

set "ROOT_DIR=%~dp0"
cd /d "%ROOT_DIR%"

if exist lib rmdir /s /q lib
if exist bin rmdir /s /q bin
if exist obj rmdir /s /q obj
if exist coverage rmdir /s /q coverage

del /q Makefile 2>nul
del /q *.make 2>nul
del /q *.workspace 2>nul
del /q *.sln 2>nul
del /q *.vcxproj* 2>nul
del /q *.csproj 2>nul

echo Cleaned generated build files and directories.
