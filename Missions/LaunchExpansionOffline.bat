@echo off

taskkill /F /IM DayZDiag_x64.exe /T

start DayZDiag_x64.exe -mission=.\Missions\ExpansionCOM.ChernarusPlus -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=!Workshop\@DayZ-Expansion;!Workshop\@DayZ-Expansion-Licensed;!Workshop\@CF;!Workshop\@Community-Online-Tools;"