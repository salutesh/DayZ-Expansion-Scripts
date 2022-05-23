/**
 * ExpansionAICrashPatrol.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAICrashPatrol
{
    string Faction;             // WEST, EAST, INSURGENT, CIVILIAN, GUARDS
    string EventName;           // Wreck_UH1Y, Wreck_Mi8, Wreck_PoliceCar, ContaminatedArea_Static, ContaminatedArea_Dynamic
    string LoadoutFile;         // a json file containing the loadout of this team - if empty, will use the default loadout of the faction
    int NumberOfAI;             // How many bots, -x will make it random between 0 and x
    string Behaviour;           // HOLD, PATROL
    string Speed;               // WALK, JOG, SPRINT, RANDOM
    string UnderThreatSpeed;    // WALK, JOG, SPRINT, RANDOM
	float MinDistRadius;	    // If the player is closer than MinDistRadius from the spawn point, the group won't spawn, if set to -2, will use the general setting instead
	float MaxDistRadius;	    // Same but if the player is further away than MaxDistRadius, the bots won't spawn, if set to -2, will use the general setting instead
    bool CanBeLooted;           // if enabled, the bots can be looted by the players
    bool UnlimitedReload;       // should bots be able to reload indefinitely (still needs spare mag in inventory!)
    float Chance;               // chance for this patrol to spawn

    void ExpansionAICrashPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "PATROL", string fac = "WEST", string eventtype = "Wreck_UH1Y", string loa = "", float mindistradius = -2, float maxdistradius = -2, bool canbelooted = true, bool unlimitedreload = false, float chance = 1.0)
    {
        NumberOfAI = bod;
        Speed = spd;
        UnderThreatSpeed = threatspd;
        Behaviour = beh;
        Faction = fac;
        EventName = eventtype;
        LoadoutFile = loa;
        MinDistRadius = mindistradius;
        MaxDistRadius = maxdistradius;
        CanBeLooted = canbelooted;
        UnlimitedReload = unlimitedreload;
        Chance = chance;
    }

    //! TODO: Improve the spread code, this is very unoptimised
    vector GetStartPosition(vector position)
    {
        float minspread = 5;
        float maxspread = 10;
        if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
        {
            minspread = 0;
            maxspread = 50;
        }

        position = ExpansionMath.GetRandomPointInRing(position, minspread, maxspread);
        position = ExpansionStatic.GetSurfacePosition(position);

        return position;
    }

    //! TODO: Improve the spread code, this is very unoptimised
    ref TVectorArray GetWaypoints(vector position, int beh = eAIWaypointBehavior.HALT )
    {
        ref TVectorArray waypoints = new ref TVectorArray;
        vector waypoint;
        float minspread;
        float maxspread;
        if ( beh == eAIWaypointBehavior.HALT )
        {
            minspread = 5;
            maxspread = 15;
            if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
            {
                minspread = 0;
                maxspread = 50;
            }

            waypoint = ExpansionMath.GetRandomPointInRing(position, minspread, maxspread);
            waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

            waypoints.Insert(waypoint);
        } else {
            minspread = 5;
            maxspread = 20;
            int amountofpatrols = Math.RandomInt(2,6);
            if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
            {
                minspread = 0;
                maxspread = 150;
                amountofpatrols = Math.RandomInt(4,8);
            }
            
		    for (int i = 0; i < amountofpatrols; i++)
            {
                waypoint = ExpansionMath.GetRandomPointInRing(position, minspread, maxspread);
                waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

                waypoints.Insert(waypoint);
            }
        }

        return waypoints;
    }

    float GetThreatSpeed()
    {
        switch (UnderThreatSpeed)
        {
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
                return Math.RandomInt(1,3);
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
                return Math.RandomInt(1,3);
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
        return eAIWaypointBehavior.HALT;
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
