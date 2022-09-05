class ExpansionAIPatrolManager
{
    static ExpansionAIPatrolSettings s_AIPatrolSettings;

    static eAIDynamicPatrol InitObjectPatrol(string type, vector position)
    {
        if ( !s_AIPatrolSettings )
            s_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !s_AIPatrolSettings.Enabled )
            return NULL;

        //! Init matching object patrol
        foreach(ExpansionAIObjectPatrol patrol: s_AIPatrolSettings.ObjectPatrols)
        {
            if (!patrol.ClassName || !CF_String.EqualsIgnoreCase(patrol.ClassName, type))
                continue;

            auto dynPatrol = InitPatrol(patrol, position);
            if (!dynPatrol)
                continue;

            //! Return dynamic patrol
            return dynPatrol;
        }

        return NULL;
    }

    protected static eAIDynamicPatrol InitPatrol(ExpansionAIDynamicSpawnBase patrol, vector position = vector.Zero)
    {
        if (patrol.Chance < Math.RandomFloat(0.0, 1.0))
            return NULL;

        int aiSum;
        if ( patrol.NumberOfAI != 0 )
        {
            if ( patrol.NumberOfAI < 0 )
            {
                aiSum = Math.RandomInt(1,-patrol.NumberOfAI);
            } else {
                aiSum = patrol.NumberOfAI;
            }
        }
        else {
            PatrolLog(patrol, "WARNING: NumberOfAI shouldn't be set to 0, skipping this patrol...");
            return NULL;
        }

        eAIWaypointBehavior behaviour = patrol.GetBehaviour();
        TVectorArray waypoints = patrol.GetWaypoints(position, behaviour);
        vector startpos = waypoints[0];

        if (startpos == vector.Zero)
        {
            PatrolLog(patrol, "!!! ERROR !!!");
            PatrolLog(patrol, "Couldn't find a spawn location. First waypoint is set to 0 0 0 or could not be read by the system (validate your file with a json validator)");
            PatrolLog(patrol, "!!! ERROR !!!");
            return NULL;
        }

        float respawntime = 0;
        float despawntime = 0;
        float mindistradius = 0;
        float maxdistradius = 0;
        float despawnradius = 0;

        if ( patrol.RespawnTime == -2 )
        {
            respawntime = s_AIPatrolSettings.RespawnTime;
        } else {
            respawntime = patrol.RespawnTime;
        }

        if ( patrol.DespawnTime < 0 )
        {
            despawntime = s_AIPatrolSettings.DespawnTime;
        } else {
            despawntime = patrol.DespawnTime;
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
            PatrolLog(patrol, "!!! ERROR !!!");
            PatrolLog(patrol, "MinDistRadius has a larger radius than MaxDistRadius (MinDistRadius should be smaller than MaxDistRadius)");
            PatrolLog(patrol, "!!! ERROR !!!");
        }

        auto dynPatrol = eAIDynamicPatrol.CreateEx(startpos, waypoints, behaviour, patrol.LoadoutFile, aiSum, respawntime, despawntime, eAIFaction.Create(patrol.Faction), eAIFormation.Create(patrol.Formation), true, mindistradius, maxdistradius, despawnradius, patrol.GetSpeed(), patrol.GetThreatSpeed(), patrol.CanBeLooted, patrol.UnlimitedReload);
        dynPatrol.SetAccuracy(patrol.AccuracyMin, patrol.AccuracyMax);
        return dynPatrol;
    }

    static void InitPatrols()
    {
        if ( !s_AIPatrolSettings )
            s_AIPatrolSettings = GetExpansionSettings().GetAIPatrol();

        if ( !s_AIPatrolSettings.Enabled )
            return;
            
        PatrolLog(null, "=================== Patrol Spawner START ===================");

        //! Init all roaming patrols
        foreach(ExpansionAIPatrol patrol: s_AIPatrolSettings.Patrols)
        {
            if ( !patrol.Waypoints || !patrol.Waypoints.Count() )
            {
                PatrolLog(patrol, "!!! ERROR !!!");
                PatrolLog(patrol, "No waypoints (validate your file with a json validator)");
                PatrolLog(patrol, "!!! ERROR !!!");
                continue;
            }

            if (!InitPatrol(patrol))
                continue;
        }
        PatrolLog(null, "=================== Patrol Spawner END ===================");
    }

    private static void PatrolLog(ExpansionAIDynamicSpawnBase patrol, string msg)
    {
        if ( patrol && patrol.IsInherited(ExpansionAIObjectPatrol) )
        {
            if (GetExpansionSettings().GetLog().AIObjectPatrol )
                GetExpansionSettings().GetLog().PrintLog("[AI Object Patrol] " +msg);
        }
        else if ( GetExpansionSettings().GetLog().AIPatrol )
        {
            GetExpansionSettings().GetLog().PrintLog("[AI Patrol] " +msg);
        }
    }
}
