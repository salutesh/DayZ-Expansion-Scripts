/**
 * ExpansionGuardPosition.c
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
class ExpansionAISpawnPosition
{
	string NPCName = "Unknown";
	vector Position = vector.Zero;
	vector Orientation = vector.Zero;
	ref array<vector> Waypoints = {vector.Zero};
	string Loadout = "GorkaLoadout";
	string Faction = "InvincibleSurvivors";
	float MinSpeed = 1.0;
	float MaxSpeed = 3.0;
	float MinAccuracy = 0.5;
	float MaxAccuracy = 1.0;
	bool CanBeLooted = true;
	bool UnlimtedReload = true;
	float ThreatDistanceLimit = 800.0;
	float DamageMultiplier = 1.0;
	ref array<vector> ShelterPositions;

	void ExpansionAISpawnPosition(string name, vector pos, vector ori, array<vector> waypoints, string loadout, string factionName, float minSpeed = 3.0, float maxSpeed = 5.0, float minAccuracy = 0.5, float maxAccuracy = 1.0, bool canBeLooted = true, bool unlimtedReload = true, float threatDistLimit = 850.0, float damageMultiplier = 1.0)
	{
		NPCName = name;
		Position = pos;
		Orientation = ori;
		Waypoints = waypoints;
		Loadout = loadout;
		Faction = factionName;
		MinSpeed = minSpeed;
		MaxSpeed = maxSpeed;
		MinAccuracy = minAccuracy;
		MaxAccuracy = maxAccuracy;
		CanBeLooted = canBeLooted;
		UnlimtedReload = unlimtedReload;
		ThreatDistanceLimit = threatDistLimit;
		DamageMultiplier = damageMultiplier;
	}
	
	void SetShelterPositions(array<vector> positions)
	{
		if (!ShelterPositions)
			ShelterPositions = new array<vector>;
		
		ShelterPositions.InsertAll(positions);
	}
};
#endif