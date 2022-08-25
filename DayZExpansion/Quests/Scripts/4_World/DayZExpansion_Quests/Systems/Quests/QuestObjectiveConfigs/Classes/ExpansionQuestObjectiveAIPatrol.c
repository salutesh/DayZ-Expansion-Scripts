/**
 * ExpansionQuestObjectiveTreasureHunt.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIPatrol
{
	private int NPCUnits;
	private ref array<vector> Waypoints = new array<vector>;
	private string NPCSpeed;
	private string NPCMode;
	private string NPCFaction;
	string NPCFormation;
	private string NPCLoadoutFile;
	float NPCAccuracyMin;
	float NPCAccuracyMax;
	private autoptr array<string> ClassNames = new array<string>;
	private bool SpecialWeapon = false;
	private autoptr array<string> AllowedWeapons = new array<string>;

	void SetNPCUnits(int units)
	{
		NPCUnits = units;
	}

	int GetNPCUnits()
	{
		return NPCUnits;
	}

	void SetNPCSpeed(string npcSpeed)
	{
		NPCSpeed = npcSpeed;
	}

	string GetNPCSpeed()
	{
		return NPCSpeed;
	}

	void SetNPCMode(string npcMode)
	{
		NPCMode = npcMode;
	}

	string GetNPCMode()
	{
		return NPCMode;
	}

	void SetNPCFaction(string npcFaction)
	{
		NPCFaction = npcFaction;
	}

	string GetNPCFaction()
	{
		return NPCFaction;
	}

	void SetNPCLoadoutFile(string loadoutFile)
	{
		NPCLoadoutFile = loadoutFile;
	}

	string GetNPCLoadoutFile()
	{
		return NPCLoadoutFile;
	}

	void AddWaypoint(vector waypoint)
	{
		Waypoints.Insert(waypoint);
	}

	array<vector> GetWaypoints()
	{
		return Waypoints;
	}

	void AddClassName(string name)
	{
		ClassNames.Insert(name);
	}

	array<string> GetClassNames()
	{
		return ClassNames;
	}

	void SetNeedSpecialWeapon(bool state)
	{
		SpecialWeapon = state;
	}

	bool NeedSpecialWeapon()
	{
		return SpecialWeapon;
	}

	void AddAllowedWeapon(string name)
	{
		AllowedWeapons.Insert(name);
	}

	array<string> GetAllowedWeapons()
	{
		return AllowedWeapons;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - NPCUnits: " + NPCUnits);
		for (int i = 0; i < Waypoints.Count(); ++i)
		{
			Print(ToString() + "::QuestDebug - Waypoint" + i + " :" + Waypoints[i]);
		}
		Print(ToString() + "::QuestDebug - NPCSpeed: " + NPCSpeed);
		Print(ToString() + "::QuestDebug - NPCSpeed: " + NPCMode);
		Print(ToString() + "::QuestDebug - NPCFaction: " + NPCFaction);
		Print(ToString() + "::QuestDebug - NPCFormation: " + NPCFormation);
		Print(ToString() + "::QuestDebug - NPCLoadoutFile: " + NPCLoadoutFile);
		Print("------------------------------------------------------------");
	#endif
	}
};
#endif