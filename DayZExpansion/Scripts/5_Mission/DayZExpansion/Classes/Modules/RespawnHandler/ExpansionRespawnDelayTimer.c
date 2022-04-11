/**
 * ExpansionRespawnDelayTimer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRespawnDelayTimer
{
	protected const int RESPAWN_COOLDOWN = GetExpansionSettings().GetSpawn().RespawnCooldown;
	string PlayerUID;
	int Index;
	int Hour;
	int Minute;
	int Second;
	int Punishment;
	bool IsTerritory;
	
	void ExpansionRespawnDelayTimer(string playerUID, int index, bool isTerritory = false)
	{
		PlayerUID = playerUID;
		Index = index;
		IsTerritory = isTerritory;
		
		SetTime();
	}
	
	void SetTime()
	{
		if (!GetExpansionSettings().GetSpawn().RespawnUTCTime)
			GetHourMinuteSecond(Hour, Minute, Second);
		else
			GetHourMinuteSecondUTC(Hour, Minute, Second);
	}
	
	int GetTime()
	{
		int time;
		time += Hour * 3600;
		time += Minute * 60;
		time += Second;
		
		return time;
	}
	
	bool HasCooldown()
	{
		int currentTime;
		int oldTime;
		int hours;
		int minutes;
		int seconds;
		
		if (!GetExpansionSettings().GetSpawn().RespawnUTCTime)
			GetHourMinuteSecond(hours, minutes, seconds);
		else
			GetHourMinuteSecondUTC(hours, minutes, seconds);
		
		currentTime += hours * 3600;
		currentTime += minutes * 60;
		currentTime += seconds;
		
		oldTime += Hour * 3600;
		oldTime += Minute * 60;
		oldTime += Second;
				
		if ((currentTime - oldTime) < (GetExpansionSettings().GetSpawn().RespawnCooldown + Punishment))
			return true;
		
		return false;
	}
	
	int GetTimeDiff()
	{
		int currentTime;
		int oldTime;
		int hours;
		int minutes;
		int seconds;
		
		if (!GetExpansionSettings().GetSpawn().RespawnUTCTime)
			GetHourMinuteSecond(hours, minutes, seconds);
		else
			GetHourMinuteSecondUTC(hours, minutes, seconds);
		
		currentTime += hours * 3600;
		currentTime += minutes * 60;
		currentTime += seconds;
		
		oldTime += Hour * 3600;
		oldTime += Minute * 60;
		oldTime += Second;
		
		return (currentTime - oldTime);
	}
	
	void SetPunishment(int sec)
	{
		Punishment = sec;
	}
	
	void AddPunishment(int sec)
	{
		Punishment += sec;
	}
	
	int GetPunishment()
	{
		return Punishment;
	}
		
	bool IsTerritory()
	{
		return IsTerritory;
	}
}