/**
 * ExpansionAIPatrol.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAIPatrol
{
    string Faction;                     // WEST, EAST, INSURGENT, CIVILIAN
    string LoadoutFile;                 // a json file containing the loadout of this team - if empty, will use the default loadout of the faction
    int NumberOfAI;                     // How many bots, -x will make it random between 0 and x
    string Behaviour;                   // HALT, LOOP, REVERSE, HOLD OR PATROL
    string Speed;                       // WALK, JOG, SPRINT, RANDOM
    string UnderThreatSpeed;            // WALK, JOG, SPRINT, RANDOM
	float RespawnTime;	                // Time in seconds before the dead group will respawn. If set to -1, they won't respawn, if set to -2, will use the general setting instead
	float MinDistRadius;	            // If the player is closer than MinDistRadius from the spawn point, the group won't spawn, if set to -2, will use the general setting instead
	float MaxDistRadius;	            // Same but if the player is further away than MaxDistRadius, the bots won't spawn, if set to -2, will use the general setting instead
	float WaypointsSpreadRadius;        // if set to 0, the bots will go exactly on the position of each waypoints. Beyond 0, they will take a random waypoint location around the original position in the area. If bellow 0 (negative), each waypoints will have a different radius size between the -value choosen and -1
    vector StartPos;                    // where the group is going to spawn
    autoptr TVectorArray Waypoints;     // a list of positions to create a path to follow
    bool CanBeLooted;                   // if enabled, the bots can be looted by the players
    bool UnlimitedReload;               // should bots be able to reload indefinitely (still needs spare mag in inventory!)
    float Chance;                       // chance for this patrol to spawn

    void ExpansionAIPatrol(int bod, string spd, string threatspd, string beh, string fac, string loa, float respawntime, float mindistradius, float maxdistradius, float wprnd, vector startpos, TVectorArray way, bool canbelooted, bool unlimitedreload, float chance)
    {
        NumberOfAI = bod;
        Speed = spd;
        UnderThreatSpeed = threatspd;
        Behaviour = beh;
        Faction = fac;
        LoadoutFile = loa;
        RespawnTime = respawntime;
        MinDistRadius = mindistradius;
        MaxDistRadius = maxdistradius;
        WaypointsSpreadRadius = wprnd;
        StartPos = startpos;
        Waypoints = way;
        CanBeLooted = canbelooted;
        UnlimitedReload = unlimitedreload;
        Chance = chance;
    }

    TVectorArray GetWaypoints()
    {
        if ( WaypointsSpreadRadius != 0.0 )
        {
            float randomdist;
            TVectorArray RndWaypoints = new TVectorArray;
            foreach(vector waypoint: Waypoints)
            {
                if ( WaypointsSpreadRadius < 0.0 )
                {
                    randomdist = Math.RandomIntInclusive(1,-WaypointsSpreadRadius);
                } else {
                    randomdist = WaypointsSpreadRadius;
                }

                waypoint[0] = Math.RandomFloat(waypoint[0]-randomdist, waypoint[0]+randomdist);
                waypoint[2] = Math.RandomFloat(waypoint[2]-randomdist, waypoint[2]+randomdist);
                waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

                RndWaypoints.Insert(waypoint);
            }

            return RndWaypoints;
        }

        //! Unknown speed, sending default speed
        return Waypoints;
    }

    float GetThreatSpeed()
    {
        switch (UnderThreatSpeed)
        {
            case "STATIC":
            {
                return 0.0;
                break;
            }
            case "WALK":
            {
                return 1.0;
                break;
            }
            case "JOG":
            {
                return 2.0;
                break;
            }
            case "SPRINT":
            {
                return 3.0;
                break;
            }
            case "RANDOM":
            {
                return Math.RandomIntInclusive(1,3);
                break;
            }
        }

        //! Unknown speed, sending default speed
        return 2.0;
    }

    float GetSpeed()
    {
        switch (Speed)
        {
            case "STATIC":
            {
                return 0.0;
                break;
            }
            case "WALK":
            {
                return 1.0;
                break;
            }
            case "JOG":
            {
                return 2.0;
                break;
            }
            case "SPRINT":
            {
                return 3.0;
                break;
            }
            case "RANDOM":
            {
                return Math.RandomIntInclusive(1,3);
                break;
            }
        }

        //! Unknown speed, sending default speed
        return 2.0;
    }

    int GetBehaviour()
    {
        switch (Behaviour)
        {
            case "REVERSE":
            {
                return eAIWaypointBehavior.REVERSE; // Follow the waypoints in reverse (from end to start)
                break;
            }
            case "HOLD":
            case "HALT":
            {
                return eAIWaypointBehavior.HALT; // They just don't move, they stay at their position
                break;
            }
            case "PATROL":
            case "LOOP":
            {
                return eAIWaypointBehavior.LOOP; // Follow the waypoint's in the normal order (from start to finish)
                break;
            }
            case "HOLD OR PATROL":
            {
                if ( Math.RandomIntInclusive(0,1) )
                    return eAIWaypointBehavior.HALT; // They just don't move, they stay at their position

                return eAIWaypointBehavior.LOOP; // Follow the waypoint's in the normal order (from start to finish)
                break;
            }
        }

        //! Unknown Behaviour, sending default behaviour
        return eAIWaypointBehavior.REVERSE;
    }

    eAIFaction GetFaction()
    {
        switch (Faction)
        {
            case "WEST":
            {
                return new eAIFactionWest(); // Friendly toward WEST and CIVILIANS
                break;
            }
            case "EAST":
            {
                return new eAIFactionEast(); // Friendly toward EAST and CIVILIANS
                break;
            }
            case "RAIDERS":
            case "INSURGENT":
            {
                return new eAIFactionRaiders(); // Hostile toward everyone
                break;
            }
            case "CIVILIAN":
            {
                return new eAIFactionCivilian(); // They like everyone
                break;
            }
            case "GUARDS":
            {
                return new eAIFactionGuards(); // They like everyone until the other party raises their weapon (and is not a guard themselves)
                break;
            }
        }

        //! Unknown Faction, sending default faction
        return new eAIFactionCivilian();
    }
};
