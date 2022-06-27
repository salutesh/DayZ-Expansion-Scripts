class PatrolManager
{
    ExpansionAIPatrolSettings m_AIPatrolSettings;

    eAIDynamicPatrol InitCrashPatrolSpawner(string type, vector position)
    {
        if ( !m_AIPatrolSettings )
            m_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !m_AIPatrolSettings.Enabled )
            return NULL;

        foreach(ExpansionAICrashPatrol group: m_AIPatrolSettings.EventCrashPatrol)
        {
            if (group.EventName != type)
                continue;

            if (group.Chance < Math.RandomFloat(0.0, 1.0))
                continue;

            int aiSum;
            if ( group.NumberOfAI != 0 )
            {
                if ( group.NumberOfAI < 0 )
                {
                    aiSum = Math.RandomInt(1,-group.NumberOfAI);
                } else {
                    aiSum = group.NumberOfAI;
                }
            } else {
                CrashPatrolLog("WARNING: NumberOfAI shouldn't be set to 0, skipping the "+group.EventName+" group");
                continue;
            }

            float mindistradius = 0;
            float maxdistradius = 0;
            int behaviour = group.GetBehaviour();
            vector startpos = group.GetStartPosition(position);
            TVectorArray waypoints = group.GetWaypoints(position, behaviour);

            if ( group.MinDistRadius == -2 )
            {
                mindistradius = m_AIPatrolSettings.MinDistRadius;
            } else {
                mindistradius = group.MinDistRadius;
            }

            if ( group.MaxDistRadius == -2 )
            {
                maxdistradius = m_AIPatrolSettings.MaxDistRadius;
            } else {
                maxdistradius = group.MaxDistRadius;
            }
            
            if (mindistradius > maxdistradius)
            {
                CrashPatrolLog("!!! ERROR !!!");
                CrashPatrolLog("MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
                CrashPatrolLog("!!! ERROR !!!");
            }

            CrashPatrolLog("Spawning "+aiSum+" "+group.Faction+" bots near a "+group.EventName+" at "+startpos);
            return eAIDynamicPatrol.Create(startpos, waypoints, behaviour, group.LoadoutFile, aiSum, -1, eAIFaction.Create(group.Faction), true, mindistradius, maxdistradius, group.GetSpeed(), group.GetThreatSpeed(), group.CanBeLooted, group.UnlimitedReload);
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

        foreach(ExpansionAIPatrol group: m_AIPatrolSettings.Patrol)
        {
		    if (group.Chance < Math.RandomFloat(0.0, 1.0))
                continue;

            int aiSum;
            if ( group.NumberOfAI != 0 )
            {
                if ( group.NumberOfAI < 0 )
                {
                    aiSum = Math.RandomInt(1,-group.NumberOfAI);
                } else {
                    aiSum = group.NumberOfAI;
                }
            } else {
                PatrolLog("WARNING: NumberOfAI shouldn't be set to 0, skipping this group...");
                continue;
            }

            if ( !group.Waypoints )
            {
                PatrolLog("!!! ERROR !!!");
                PatrolLog("Couldn't read the Waypoints (validate your file with a json validator)");
                PatrolLog("!!! ERROR !!!");
                continue;
            }

            vector startpos = group.StartPos;
            if ( !startpos || startpos == "0 0 0" )
            {
                if ( !group.Waypoints[0] || group.Waypoints[0] == "0 0 0" )
                {
                    PatrolLog("!!! ERROR !!!");
                    PatrolLog("Couldn't find a spawn location. StartPos and at least the first Waypoints are both set to 0 0 0 or cannot be read by the system (validate your file with a json validator)");
                    PatrolLog("!!! ERROR !!!");
                    continue;
                }

                startpos = group.Waypoints[0];
            }

            // Safety in case the Y is bellow the ground
            startpos = ExpansionStatic.GetSurfacePosition(startpos);
            if ( startpos[1] < group.StartPos[1] )
                startpos[1] = group.StartPos[1];

            if ( group.RespawnTime == -2 )
            {
                respawntime = m_AIPatrolSettings.RespawnTime;
            } else {
                respawntime = group.RespawnTime;
            }

            if ( group.MinDistRadius == -2 )
            {
                mindistradius = m_AIPatrolSettings.MinDistRadius;
            } else {
                mindistradius = group.MinDistRadius;
            }

            if ( group.MaxDistRadius == -2 )
            {
                maxdistradius = m_AIPatrolSettings.MaxDistRadius;
            } else {
                maxdistradius = group.MaxDistRadius;
            }

            if (mindistradius > maxdistradius)
            {
                PatrolLog("!!! ERROR !!!");
                PatrolLog("MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
                PatrolLog("!!! ERROR !!!");
            }

            PatrolLog("Spawning "+aiSum+" "+group.Faction+" bots at "+startpos);
            eAIDynamicPatrol.Create(startpos, group.Waypoints, group.GetBehaviour(), group.LoadoutFile, aiSum, respawntime, eAIFaction.Create(group.Faction), true, mindistradius, maxdistradius, group.GetSpeed(), group.GetThreatSpeed(), group.CanBeLooted, group.UnlimitedReload);
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