@echo off

REM Build 32-bit and 64-bit versions of the app in Debug mode.
REM msbuild /t:Build /p:platform=Win32 /p:config=Debug %~dp0\..\Clevy.cbproj
REM msbuild /t:Build /p:platform=Win64 /p:config=Debug %~dp0\..\Clevy.cbproj

REM Build 32-bit and 64-bit versions of the app in Release mode.
msbuild /t:Build /p:platform=Win32 /p:config=Release %~dp0\..\Clevy.cbproj
msbuild /t:Build /p:platform=Win64 /p:config=Release %~dp0\..\Clevy.cbproj

REM Create installers for both 32-bit and 64-bit version of the app.
iscc %~dp0\Setup_Win32.iss
iscc %~dp0\Setup_Win64.iss
