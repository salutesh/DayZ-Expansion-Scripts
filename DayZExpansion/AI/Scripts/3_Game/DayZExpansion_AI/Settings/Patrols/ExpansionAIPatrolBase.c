/**
 * ExpansionAIPatrolBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAIPatrolBase
{
	string Faction;                     // Raiders, Mercenaries, West, East, Guards, Civilian, Passive
	string LoadoutFile;                 // a json file containing the loadout of this team - if empty, will use the default loadout of the faction
	int NumberOfAI;                     // How many bots, -x will make it random between 0 and x
	string Behaviour;                   // HALT, LOOP, ALTERNATE, HOLD OR PATROL
	string Speed;                       // WALK, JOG, SPRINT, RANDOM
	string UnderThreatSpeed;            // WALK, JOG, SPRINT, RANDOM
	float MinDistRadius;	            // If the player is closer than MinDistRadius from the spawn point, the group won't spawn, if set to -2, will use the general setting instead
	float MaxDistRadius;	            // Same but if the player is further away than MaxDistRadius, the bots won't spawn, if set to -2, will use the general setting instead
	float MinSpreadRadius;
	float MaxSpreadRadius;
	bool CanBeLooted;                   // if enabled, the bots can be looted by the players
	bool UnlimitedReload;               // should bots be able to reload indefinitely (still needs spare mag in inventory!)
	float Chance;                       // chance for this patrol to spawn

	void ExpansionAIPatrolBase(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "PATROL", string fac = "WEST")
	{
		NumberOfAI = bod;
		Speed = spd;
		UnderThreatSpeed = threatspd;
		Behaviour = beh;
		Faction = fac;
	}

	float _GetSpeed(string speed)
	{
		switch (speed)
		{
			case "STATIC":
			{
				return 0.0;
			}
			case "WALK":
			{
				return 1.0;
			}
			case "JOG":
			{
				return 2.0;
			}
			case "SPRINT":
			{
				return 3.0;
			}
			case "RANDOM":
			{
				return Math.RandomIntInclusive(1, 3);
			}
		}

		//! Unknown speed, sending default speed
		return 2.0;
	}

	float GetSpeed()
	{
		return _GetSpeed(Speed);
	}

	float GetThreatSpeed()
	{
		return _GetSpeed(UnderThreatSpeed);
	}

	int GetBehaviour()
	{
		switch (Behaviour)
		{
			case "HOLD":
			case "HALT":
			{
				return eAIWaypointBehavior.HALT; // They just don't move, they stay at their position
			}
			case "LOOP":
			{
				return eAIWaypointBehavior.LOOP; // Follow the waypoints from start to finish, then go straight to start, repeat
			}
			case "PATROL":
			case "REVERSE":
			case "ALTERNATE":
			{
				return eAIWaypointBehavior.ALTERNATE; // Follow the waypoints from start to finish, then from finish to start, repeat
			}
			case "HOLD OR PATROL":
			{
				if ( Math.RandomIntInclusive(0, 1) )
					return eAIWaypointBehavior.HALT; // They just don't move, they stay at their position

				return eAIWaypointBehavior.ALTERNATE; // Follow the waypoints from start to finish, then from finish to start, repeat
			}
		}

		//! Unknown Behaviour, sending default behaviour
		return eAIWaypointBehavior.ALTERNATE;
	}

	//! Update faction string to v3
	void UpdateFaction()
	{
		switch (Faction)
		{
			case "INSURGENT":
				Faction = "Raiders";
				break;
			case "WEST":
			case "EAST":
			case "RAIDERS":
			case "CIVILIAN":
			case "GUARDS":
			case "PASSIVE":
				string lowerpart = Faction.Substring(1, Faction.Length() - 1);
				lowerpart.ToLower();
				Faction = Faction[0] + lowerpart;
				break;
		}
	}
};
