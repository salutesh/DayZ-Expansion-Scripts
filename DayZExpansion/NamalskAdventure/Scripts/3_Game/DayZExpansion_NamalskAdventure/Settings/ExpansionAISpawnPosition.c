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
	string NPCName;
	vector Position;
	vector Orientation;
	ref array<vector> Waypoints = new array<vector>;
	string Loadout;
	string Faction;
	float MinSpeed;
	float MaxSpeed;
	float MinAccuracy;
	float MaxAccuracy;
	bool CanBeLooted;
	bool UnlimtedReload;
	float ThreatDistanceLimit;
	float DamageMultiplier;
	ref array<vector> ShelterPositions = new array<vector>;

	void Set(string name, vector pos, vector ori, array<vector> waypoints, string loadout, string factionName, float minSpeed = 3.0, float maxSpeed = 5.0, float minAccuracy = 0.5, float maxAccuracy = 1.0, bool canBeLooted = true, bool unlimtedReload = true, float threatDistLimit = 850.0, float damageMultiplier = 1.0)
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