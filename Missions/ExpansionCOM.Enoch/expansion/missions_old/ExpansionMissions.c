/**
 * ExpansionMissions.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#include "$CurrentDir:\\Missions\\DayZExpansion.Enoch\\missions\\ExpansionLoot.c"

#include "$CurrentDir:\\Missions\\DayZExpansion.Enoch\\missions\\spawns\\ExpansionMissionOne.c"
#include "$CurrentDir:\\Missions\\DayZExpansion.Enoch\\missions\\spawns\\ExpansionMissionTwo.c"
#include "$CurrentDir:\\Missions\\DayZExpansion.Enoch\\missions\\spawns\\ExpansionMissionThree.c"
#include "$CurrentDir:\\Missions\\DayZExpansion.Enoch\\missions\\spawns\\ExpansionMissionFour.c"
#include "$CurrentDir:\\Missions\\DayZExpansion.Enoch\\missions\\spawns\\ExpansionMissionFive.c"

void SpawnMissions()
{
	GetDynamicMissions().SpawnMission("MissionOne", 1, 10, 70);
	GetDynamicMissions().SpawnMission("MissionTwo", 1, 20, 70);  
	GetDynamicMissions().SpawnMission("MissionThree", 1, 30, 70);  
	GetDynamicMissions().SpawnMission("MissionFour", 1, 40, 70);  
	GetDynamicMissions().SpawnMission("MissionFive", 1, 50, 70); 
}

class ExpansionDynamicMissions
{
	static void MissionOne()
	{
		ExpansionMissionOne();
	}

	static void MissionTwo()
	{
		ExpansionMissionTwo();
	}

	static void MissionThree()
	{
		ExpansionMissionThree();
	}

	static void MissionFour()
	{
		ExpansionMissionFour();
	}

	static void MissionFive()
	{
		ExpansionMissionFive();
	}

	void SpawnMission(string function, float chance, float time, float lifetime)
	{
		if ( Math.RandomFloatInclusive(0, 1) <= chance )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( SpawnMissionLatter, time * 60 * 1000, false, function );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( SpawnMission, lifetime * 60 * 1000, false, function, chance, time, lifetime );
		}
	}

	void SpawnMissionLatter(string function)
	{
		GetGame().GameScript.Call( this, function, null );
	}

	void SpawnMissionLoot(array< ref ExpansionMissionEventSoldierLoot> loot, ItemBase item, int items)
	{
		for (int p = 0; p < items; p++)
		{
			item.GetInventory().CreateInInventory(loot[Math.RandomInt(0, loot.Count())].Item);
		}
	}
}

static ref ExpansionDynamicMissions ex_DynamicMissions;
static ref ExpansionDynamicMissions GetDynamicMissions()
{
	if ( !ex_DynamicMissions )
	{
		 ex_DynamicMissions = new ExpansionDynamicMissions();
	}

	return ex_DynamicMissions;
}