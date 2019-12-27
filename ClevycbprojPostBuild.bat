@echo off

SET PLATFORM=%1
SET PROJECTDIR=%2
SET OUTPUTDIR=%3

if not "%PLATFORM%"=="Win32" if not "%PLATFORM%"=="Win64" GOTO :end
if not exist "%PROJECTDIR%" GOTO :end
if not exist "%OUTPUTDIR%" GOTO :end

echo Platform: %PLATFORM%
echo Project dir: %PROJECTDIR%
echo Output dir: %OUTPUTDIR%

if "%PLATFORM%"=="Win32" (
	copy %PROJECTDIR%\lib\rstts\platforms\i686-pc-win32\librstts-2.dll %OUTPUTDIR%
) else if "%PLATFORM%"=="Win64" (
	copy %PROJECTDIR%\lib\rstts\platforms\x86_64-pc-win64\librstts-2.dll %OUTPUTDIR%
)

xcopy %PROJECTDIR%\res\language\*.lang.json %OUTPUTDIR%\language\ /y /i
xcopy %PROJECTDIR%\res\data\*.* %OUTPUTDIR%\data\ /s /y /i

:end
