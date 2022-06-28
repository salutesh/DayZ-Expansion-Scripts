class PatrolManager
{
    ExpansionAIPatrolSettings m_AIPatrolSettings;

    eAIDynamicPatrol InitCrashPatrolSpawner(string type, vector position)
    {
        if ( !m_AIPatrolSettings )
            m_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !m_AIPatrolSettings.Enabled )
            return NULL;

        foreach(ExpansionAICrashPatrol patrol: m_AIPatrolSettings.EventCrashPatrol)
        {
            if (patrol.EventName != type)
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
                CrashPatrolLog("WARNING: NumberOfAI shouldn't be set to 0, skipping the "+patrol.EventName+" patrol");
                continue;
            }

            float mindistradius = 0;
            float maxdistradius = 0;
            eAIWaypointBehavior behaviour = patrol.GetBehaviour();
            TVectorArray waypoints = patrol.GetWaypoints(position, behaviour);
            vector startpos = waypoints[0];

            if ( patrol.MinDistRadius == -2 )
            {
                mindistradius = m_AIPatrolSettings.MinDistRadius;
            } else {
                mindistradius = patrol.MinDistRadius;
            }

            if ( patrol.MaxDistRadius == -2 )
            {
                maxdistradius = m_AIPatrolSettings.MaxDistRadius;
            } else {
                maxdistradius = patrol.MaxDistRadius;
            }
            
            if (mindistradius > maxdistradius)
            {
                CrashPatrolLog("!!! ERROR !!!");
                CrashPatrolLog("MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
                CrashPatrolLog("!!! ERROR !!!");
            }

            CrashPatrolLog("Spawning "+aiSum+" "+patrol.Faction+" bots near a "+patrol.EventName+" at "+startpos);
            return eAIDynamicPatrol.Create(startpos, waypoints, behaviour, patrol.LoadoutFile, aiSum, -1, eAIFaction.Create(patrol.Faction), true, mindistradius, maxdistradius, patrol.GetSpeed(), patrol.GetThreatSpeed(), patrol.CanBeLooted, patrol.UnlimitedReload);
        }

        return NULL;
    }

    void InitPatrolSpawner()
    {
        if ( !m_AIPatrolSettings )
            m_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !m_AIPatrolSettings.Enabled )
            return;
            
        PatrolLog("=================== Patrol Spawner START ===================");

        float respawntime = 0;
        float mindistradius = 0;
        float maxdistradius = 0;

        foreach(ExpansionAIPatrol patrol: m_AIPatrolSettings.Patrol)
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
                respawntime = m_AIPatrolSettings.RespawnTime;
            } else {
                respawntime = patrol.RespawnTime;
            }

            if ( patrol.MinDistRadius == -2 )
            {
                mindistradius = m_AIPatrolSettings.MinDistRadius;
            } else {
                mindistradius = patrol.MinDistRadius;
            }

            if ( patrol.MaxDistRadius == -2 )
            {
                maxdistradius = m_AIPatrolSettings.MaxDistRadius;
            } else {
                maxdistradius = patrol.MaxDistRadius;
            }

            if (mindistradius > maxdistradius)
            {
                PatrolLog("!!! ERROR !!!");
                PatrolLog("MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
                PatrolLog("!!! ERROR !!!");
            }

            PatrolLog("Spawning "+aiSum+" "+patrol.Faction+" bots at "+startpos);
            eAIDynamicPatrol.Create(startpos, patrol.Waypoints, patrol.GetBehaviour(), patrol.LoadoutFile, aiSum, respawntime, eAIFaction.Create(patrol.Faction), true, mindistradius, maxdistradius, patrol.GetSpeed(), patrol.GetThreatSpeed(), patrol.CanBeLooted, patrol.UnlimitedReload);
        }
        PatrolLog("=================== Patrol Spawner END ===================");
    }

    private void CrashPatrolLog(string msg)
    {
        if ( GetExpansionSettings().GetLog().AICrashPatrol )
            GetExpansionSettings().GetLog().PrintLog("[AI Crash Patrol] " +msg);
    }

    private void PatrolLog(string msg)
    {
        if ( GetExpansionSettings().GetLog().AIPatrol )
            GetExpansionSettings().GetLog().PrintLog("[AI Patrol] " +msg);
    }
}