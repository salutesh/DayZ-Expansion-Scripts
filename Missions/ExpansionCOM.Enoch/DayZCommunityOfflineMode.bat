@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -mission=.\Missions\ExpansionCOM.Enoch -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=!Workshop\@DayZ-Expansion;!Workshop\@DayZ-Expansion-Licensed;!Workshop\@CF;!Workshop\@Community-Online-Tools;"