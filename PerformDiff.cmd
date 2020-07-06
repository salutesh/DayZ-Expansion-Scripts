@echo off

SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
echo ///////////////////////////////////////////////////////////////////
echo DayZ-Git-Diff - Credits Mikero, Jacob_Mango and Cleetus.
echo ///////////////////////////////////////////////////////////////////

echo Searching for Mikeros Awesome Tools!

for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKCU\SOFTWARE\Mikero\depbo" /v "path" 2^>nul') do (set _MIKEDLL=%%B)
if defined _MIKEDLL goto mikefound
for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKLM\SOFTWARE\Mikero\depbo" /v "path" 2^>nul') do (set _MIKEDLL=%%B)
if defined _MIKEDLL goto mikefound
echo DePBO found!
goto err

:mikefound

set _MIKEDLL=%_MIKEDLL%\bin
echo %_MIKEDLL%
if exist "%_MIKEDLL%\extractpbodos.exe" goto foundextract
echo Extract PBO not found!
goto err

:foundextract
if exist "%_MIKEDLL%\derap.exe" goto finddayz
echo DeRAP not found!
goto err

:finddayz
echo searching registry for the dayz path
for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKLM\SOFTWARE\Wow6432Node\Bohemia Interactive\Dayz" /v "main" 2^>nul') do (set _DAYZPATH=%%B)

if defined _DAYZPATH (
   set _DZ_EXPANSION=%_DAYZPATH%\..\..\workshop\content\221100\2116151222
   set _DZ_EXPANSION_LICENSED=%_DAYZPATH%\..\..\workshop\content\221100\2116157322

   goto findexpansion
)

echo No reg keys found, make sure you have launched your game at least once (via the official launcher).
pause
exit

:findexpansion
echo searching for expansion
if exist "%_DZ_EXPANSION%" goto findexpansionlicensed

echo DayZ Expansion not found in the workshop folder
pause
exit

:findexpansionlicensed
echo searching for expansion licensed
if exist "%_DZ_EXPANSION_LICENSED%" goto extractaddons

echo DayZ Expansion Licensed not found in the workshop folder
pause
exit


:extractaddons

echo DayZ might be installed in %_DAYZPATH%
echo DayZ Expansion might be installed in %_DZ_EXPANSION%
echo DayZ Expansion Licensed might be installed in %_DZ_EXPANSION_LICENSED%

echo removing folders. Expect this to take some time......

call (exit /b 0)

echo removing DZEXPANSION....
if exist "P:\DZEXPANSION" (rmdir /s/q "P:\DZEXPANSION")
if ERRORLEVEL 1 goto err

echo removing DZEXPANSIONLICENSED....
if exist "P:\DZEXPANSIONLICENSED" (rmdir /s/q "P:\DZEXPANSIONLICENSED")
if ERRORLEVEL 1 goto err

echo extracting DayZ Expansion addons....

extractpbodos -p "%_DZ_EXPANSION%\Addons" P:\DZEXPANSION
pause

if ERRORLEVEL 1 goto err

echo extracting DayZ Expansion Licensed addons....

extractpbodos -p "%_DZ_EXPANSION_LICENSED%\Addons" P:\DZEXPANSIONLICENSED
if ERRORLEVEL 1 goto err

echo Removing old folders.

del /S /Q "%0\..\DayZExpansion" >nul

echo Folder Removal Complete...

P:
cd DZEXPANSION
for /R %%D in ( config.cpp ) do (
     IF EXIST "%%D" (
        set _CONFIG=%%~fD
        set _PREFIX=%%~pD

        set _PREFIX=!_PREFIX:\DZEXPANSION=!

        mkdir %0\..\!_PREFIX!

        Powershell -ExecutionPolicy ByPass -command "&'%0\..\HandleConfig.ps1'" "!_CONFIG!"

        xcopy /Y !_CONFIG! %0\..\!_PREFIX!
     )
)
cd ../DZEXPANSIONLICENSED
for /R %%D in ( config.cpp ) do (
     IF EXIST "%%D" (
        set _CONFIG=%%~fD
        set _PREFIX=%%~pD

        set _PREFIX=!_PREFIX:\DZEXPANSIONLICENSED=!

        mkdir %0\..\!_PREFIX!

        Powershell -ExecutionPolicy ByPass -command "&'%0\..\HandleConfig.ps1'" "!_CONFIG!"

        xcopy /Y !_CONFIG! %0\..\!_PREFIX!
     )
)

echo Addons done... Moving onto dta...

xcopy /S /Q "P:\DZEXPANSION\DayZExpansion\Scripts" "%0\..\DayZExpansion\Scripts\"
xcopy /S /Q "P:\DZEXPANSION\DayZExpansion\GUI" "%0\..\DayZExpansion\GUI\"

echo /////////////////////////////////////
echo Files successfully moved... Cleaning up!
echo /////////////////////////////////////

:done
echo All tasks completed successfully
pause

:noprompt
endlocal
@exit /B 0

:err


echo failed
choice /c k /m "This tool WILL NOT WORK until you fix this error!"
endlocal

@exit /B 1

:process_line

for /F usebackq^ tokens^=3*^ delims^=^" %%1 in ('%the_line%') do (

set folder=%%2
set folder=!folder:"=!
set folder=!folder:\\=\!
)
