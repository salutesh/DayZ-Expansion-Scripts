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
	string PlayerUID;
	int Index;
	int Timestamp;
	int Now;
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
		Timestamp = CF_Date.Now(true).GetTimestamp();
	}
	
	bool HasCooldown()
	{
		if (GetTimeDiff() < (GetExpansionSettings().GetSpawn().GetCooldown(IsTerritory) + Punishment))
			return true;
		
		return false;
	}
	
	int GetTimeDiff()
	{
		int currentTime = CF_Date.Now(true).GetTimestamp();
		
		return (currentTime - Timestamp);
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
