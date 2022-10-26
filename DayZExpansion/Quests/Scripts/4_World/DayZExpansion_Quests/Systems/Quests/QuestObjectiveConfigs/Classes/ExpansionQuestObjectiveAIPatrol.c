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
	protected int NPCUnits;
	protected ref array<vector> Waypoints = new array<vector>;
	protected string NPCSpeed;
	protected string NPCMode;
	protected string NPCFaction;
	string NPCFormation;
	protected string NPCLoadoutFile;
	float NPCAccuracyMin;
	float NPCAccuracyMax;
	protected autoptr array<string> ClassNames = new array<string>;
	protected bool SpecialWeapon = false;
	protected autoptr array<string> AllowedWeapons = new array<string>;

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
		CF_Log.Debug("------------------------------------------------------------");
		CF_Log.Debug(ToString() + "::QuestDebug - NPCUnits: " + NPCUnits);
		for (int i = 0; i < Waypoints.Count(); ++i)
		{
			CF_Log.Debug(ToString() + "::QuestDebug - Waypoint" + i + " :" + Waypoints[i]);
		}
		CF_Log.Debug(ToString() + "::QuestDebug - NPCSpeed: " + NPCSpeed);
		CF_Log.Debug(ToString() + "::QuestDebug - NPCSpeed: " + NPCMode);
		CF_Log.Debug(ToString() + "::QuestDebug - NPCFaction: " + NPCFaction);
		CF_Log.Debug(ToString() + "::QuestDebug - NPCFormation: " + NPCFormation);
		CF_Log.Debug(ToString() + "::QuestDebug - NPCLoadoutFile: " + NPCLoadoutFile);
		CF_Log.Debug("------------------------------------------------------------");
	#endif
	}
};
#endif