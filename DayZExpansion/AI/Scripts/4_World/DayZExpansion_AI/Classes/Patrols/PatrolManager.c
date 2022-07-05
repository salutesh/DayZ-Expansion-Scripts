class ExpansionAIPatrolManager
{
    static ExpansionAIPatrolSettings s_AIPatrolSettings;

    static eAIDynamicPatrol InitObjectPatrol(string type, vector position)
    {
        if ( !s_AIPatrolSettings )
            s_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !s_AIPatrolSettings.Enabled )
            return NULL;

        foreach(ExpansionAIObjectPatrol patrol: s_AIPatrolSettings.ObjectPatrols)
        {
            if (!patrol.ClassName || patrol.ClassName != type)
                continue;

            if (patrol.Chance < Math.RandomFloat(0.0, 1.0))
                continue;

            int aiSum;
            if ( patrol.NumberOfAI != 0 )
            {
                if ( patrol.NumberOfAI < 0 )
                {
                    aiSum = Math.RandomInt(1,-patrol.NumberOfAI);
                } else {
                    aiSum = patrol.NumberOfAI;
                }
            } else {
                ObjectPatrolLog("WARNING: NumberOfAI shouldn't be set to 0, skipping the "+patrol.ClassName+" patrol");
                continue;
            }

            float mindistradius = 0;
            float maxdistradius = 0;
            float despawnradius = 0;

            eAIWaypointBehavior behaviour = patrol.GetBehaviour();
            TVectorArray waypoints = patrol.GetWaypoints(position, behaviour);
            vector startpos = waypoints[0];

            if ( patrol.MinDistRadius <= 0 )
            {
                mindistradius = s_AIPatrolSettings.MinDistRadius;
            } else {
                mindistradius = patrol.MinDistRadius;
            }

            if ( patrol.MaxDistRadius <= 0 )
            {
                maxdistradius = s_AIPatrolSettings.MaxDistRadius;
            } else {
                maxdistradius = patrol.MaxDistRadius;
            }

            if ( patrol.DespawnRadius <= 0 )
            {
                despawnradius = s_AIPatrolSettings.DespawnRadius;
            } else {
                despawnradius = patrol.DespawnRadius;
            }
            
            if (mindistradius > maxdistradius)
            {
                ObjectPatrolLog("!!! ERROR !!!");
                ObjectPatrolLog("MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
                ObjectPatrolLog("!!! ERROR !!!");
            }

            ObjectPatrolLog("Creating trigger for "+aiSum+" "+patrol.Faction+" bots near "+patrol.ClassName+" at "+startpos);
            return eAIDynamicPatrol.CreateEx(startpos, waypoints, behaviour, patrol.LoadoutFile, aiSum, -1, eAIFaction.Create(patrol.Faction), true, mindistradius, maxdistradius, despawnradius, patrol.GetSpeed(), patrol.GetThreatSpeed(), patrol.CanBeLooted, patrol.UnlimitedReload);
        }

        return NULL;
    }

    static void InitPatrols()
    {
        if ( !s_AIPatrolSettings )
            s_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !s_AIPatrolSettings.Enabled )
            return;
            
        PatrolLog("=================== Patrol Spawner START ===================");

        float respawntime = 0;
        float mindistradius = 0;
        float maxdistradius = 0;
        float despawnradius = 0;

        foreach(ExpansionAIPatrol patrol: s_AIPatrolSettings.Patrols)
        {
		    if (patrol.Chance < Math.RandomFloat(0.0, 1.0))
                continue;

            int aiSum;
            if ( patrol.NumberOfAI != 0 )
            {
                if ( patrol.NumberOfAI < 0 )
                {
                    aiSum = Math.RandomInt(1,-patrol.NumberOfAI);
                } else {
                    aiSum = patrol.NumberOfAI;
                }
            } else {
                PatrolLog("WARNING: NumberOfAI shouldn't be set to 0, skipping this patrol...");
                continue;
            }

            if ( !patrol.Waypoints )
            {
                PatrolLog("!!! ERROR !!!");
                PatrolLog("No waypoints (validate your file with a json validator)");
                PatrolLog("!!! ERROR !!!");
                continue;
            }

            vector startpos = patrol.Waypoints[0];
            if ( !startpos || startpos == "0 0 0" )
            {
                PatrolLog("!!! ERROR !!!");
                PatrolLog("Couldn't find a spawn location. First waypoint is set to 0 0 0 or cannot be read by the system (validate your file with a json validator)");
                PatrolLog("!!! ERROR !!!");
                continue;
            }

            // Safety in case the Y is bellow the ground
            startpos = ExpansionStatic.GetSurfacePosition(startpos);
            if ( startpos[1] < patrol.Waypoints[0][1] )
                startpos[1] = patrol.Waypoints[0][1];

            if ( patrol.RespawnTime == -2 )
            {
                respawntime = s_AIPatrolSettings.RespawnTime;
            } else {
                respawntime = patrol.RespawnTime;
            }

            if ( patrol.MinDistRadius <= 0 )
            {
                mindistradius = s_AIPatrolSettings.MinDistRadius;
            } else {
                mindistradius = patrol.MinDistRadius;
            }

            if ( patrol.MaxDistRadius <= 0 )
            {
                maxdistradius = s_AIPatrolSettings.MaxDistRadius;
            } else {
                maxdistradius = patrol.MaxDistRadius;
            }

            if ( patrol.DespawnRadius <= 0 )
            {
                despawnradius = s_AIPatrolSettings.DespawnRadius;
            } else {
                despawnradius = patrol.DespawnRadius;
            }

            if (mindistradius > maxdistradius)
            {
                PatrolLog("!!! ERROR !!!");
                PatrolLog("MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
                PatrolLog("!!! ERROR !!!");
            }

            PatrolLog("Creating trigger for "+aiSum+" "+patrol.Faction+" bots at "+startpos);
            eAIDynamicPatrol.CreateEx(startpos, patrol.GetWaypoints(), patrol.GetBehaviour(), patrol.LoadoutFile, aiSum, respawntime, eAIFaction.Create(patrol.Faction), true, mindistradius, maxdistradius, despawnradius, patrol.GetSpeed(), patrol.GetThreatSpeed(), patrol.CanBeLooted, patrol.UnlimitedReload);
        }
        PatrolLog("=================== Patrol Spawner END ===================");
    }

    private static void ObjectPatrolLog(string msg)
    {
        if ( GetExpansionSettings().GetLog().AIObjectPatrol )
            GetExpansionSettings().GetLog().PrintLog("[AI Object Patrol] " +msg);
    }

    private static void PatrolLog(string msg)
    {
        if ( GetExpansionSettings().GetLog().AIPatrol )
            GetExpansionSettings().GetLog().PrintLog("[AI Patrol] " +msg);
    }
}