/**
 * ExpansionQuestAISpawn.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestAISpawn
{
	[NonSerialized()]
	static ref TStringArray eAI_UNITS = {"eAI_SurvivorF_Eva", "eAI_SurvivorF_Frida", "eAI_SurvivorF_Gabi", "eAI_SurvivorF_Helga", "eAI_SurvivorF_Irena", "eAI_SurvivorF_Judy", "eAI_SurvivorF_Keiko", "eAI_SurvivorF_Linda", "eAI_SurvivorF_Maria", "eAI_SurvivorF_Naomi", "eAI_SurvivorF_Baty", "eAI_SurvivorM_Boris", "eAI_SurvivorM_Cyril", "eAI_SurvivorM_Denis", "eAI_SurvivorM_Elias", "eAI_SurvivorM_Francis", "eAI_SurvivorM_Guo", "eAI_SurvivorM_Hassan", "eAI_SurvivorM_Indar", "eAI_SurvivorM_Jose", "eAI_SurvivorM_Kaito", "eAI_SurvivorM_Lewis", "eAI_SurvivorM_Manua", "eAI_SurvivorM_Mirek", "eAI_SurvivorM_Niki", "eAI_SurvivorM_Oliver", "eAI_SurvivorM_Peter", "eAI_SurvivorM_Quinn", "eAI_SurvivorM_Rolf", "eAI_SurvivorM_Seth", "eAI_SurvivorM_Taiki"};
	
	int NumberOfAI;
	string NPCName;
	ref TVectorArray Waypoints = new TVectorArray;
	eAIWaypointBehavior Behaviour;
	string Formation;
	string Loadout;
	string Faction;
	float Speed;
	float ThreatSpeed;
	float MinAccuracy;
	float MaxAccuracy;
	bool CanBeLooted;
	bool UnlimitedReload;
	float ThreatDistanceLimit;
	float DamageMultiplier;
	float DamageReceivedMultiplier;
	ref TStringArray ClassNames = new TStringArray;
	float SniperProneDistanceThreshold;
	
	float RespawnTime;
	float DespawnTime;
	float MinDistanceRadius;
	float MaxDistanceRadius;
	float DespawnRadius;

	void SetNumberOfAI(int amount)
	{
	    NumberOfAI = amount;
	}
	
	int GetNumberOfAI()
	{
	    return NumberOfAI;
	}
	
	void SetName(string name)
	{
	    NPCName = name;
	}
	
	string GetName()
	{
	    return NPCName;
	}
	
	void AddWaypoint(vector waypoint)
	{
	    if (!Waypoints)
	        Waypoints = new TVectorArray;
	
	    Waypoints.Insert(waypoint);
	}
	
	void SetWaypoints(TVectorArray waypoints)
	{
		Waypoints = waypoints;
	}
	
	TVectorArray GetWaypoints()
	{
	    return Waypoints;
	}
	
	void SetLoadout(string loadout)
	{
	    Loadout = loadout;
	}
	
	string GetLoadout()
	{
	    return Loadout;
	}
	
	void SetFaction(string faction)
	{
	    Faction = faction;
	}
	
	string GetFaction()
	{
	    return Faction;
	}
	
	void SetBehaviour(int behaviour)
	{
	    Behaviour = behaviour;
	}
	
	eAIWaypointBehavior GetBehaviour()
	{
	    return Behaviour;
	}
	
	void SetFormation(string formation)
	{
	    Formation = formation;
	}
	
	string GetFormation()
	{
	    return Formation;
	}
	
	void SetSpeed(float speed)
	{
	    Speed = speed;
	}
	
	float GetSpeed()
	{
	    return Speed;
	}
	
	void SetThreatSpeed(float threatSpeed)
	{
	    ThreatSpeed = threatSpeed;
	}
	
	float GetThreatSpeed()
	{
	    return ThreatSpeed;
	}
	
	void SetMinAccuracy(float minAccuracy)
	{
	    MinAccuracy = minAccuracy;
	}
	
	float GetMinAccuracy()
	{
	    return MinAccuracy;
	}
	
	void SetMaxAccuracy(float maxAccuracy)
	{
	    MaxAccuracy = maxAccuracy;
	}
	
	float GetMaxAccuracy()
	{
	    return MaxAccuracy;
	}
	
	void SetCanBeLooted(bool state)
	{
	    CanBeLooted = state;
	}
	
	bool CanBeLooted()
	{
	    return CanBeLooted;
	}
	
	void SetUnlimitedReload(bool state)
	{
	    UnlimitedReload = state;
	}
	
	bool UnlimitedReload()
	{
	    return UnlimitedReload;
	}
	
	void SetThreatDistanceLimit(float limit)
	{
	    ThreatDistanceLimit = limit;
	}
	
	float GetThreatDistanceLimit()
	{
	    return ThreatDistanceLimit;
	}
	
	void SetDamageMultiplier(float mp)
	{
	    DamageMultiplier = mp;
	}
	
	float GetDamageMultiplier()
	{
	    return DamageMultiplier;
	}
	
	void SetDamageReceivedMultiplier(float mp)
	{
	    DamageReceivedMultiplier = mp;
	}
	
	float GetDamageReceivedMultiplier()
	{
	    return DamageReceivedMultiplier;
	}
	
	void SetClassNames(TStringArray classNames)
	{
	    ClassNames = classNames;
	}
	
	void AddClassName(string className)
	{
	    if (!ClassNames)
	        ClassNames = new TStringArray;
	
	    ClassNames.Insert(className);
	}
	
	TStringArray GetClassNames()
	{
	    return ClassNames;
	}
	
	void SetSniperProneDistanceThreshold(float dist)
	{
	    SniperProneDistanceThreshold = dist;
	}
	
	float GetSniperProneDistanceThreshold()
	{
	    return SniperProneDistanceThreshold;
	}
	
	void SetRespawnTime(float time)
	{
	    RespawnTime = time;
	}
	
	float GetRespawnTime()
	{
	    return RespawnTime;
	}
	
	void SetDespawnTime(float time)
	{
	    DespawnTime = time;
	}
	
	float GetDespawnTime()
	{
	    return DespawnTime;
	}
	
	void SetMinDistanceRadius(float distance)
	{
	    MinDistanceRadius = distance;
	}
	
	float GetMinDistanceRadius()
	{
	    return MinDistanceRadius;
	}
	
	void SetMaxDistanceRadius(float distance)
	{
	    MaxDistanceRadius = distance;
	}
	
	float GetMaxDistanceRadius()
	{
	    return MaxDistanceRadius;
	}
	
	void SetDespawnRadius(float distance)
	{
	    DespawnRadius = distance;
	}
	
	float GetDespawnRadius()
	{
	    return DespawnRadius;
	}
	
	eAIQuestPatrol CreateAIQuestPatrol(int killCount = 0)
	{
	    auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	
	    int aiSum;
	    if (NumberOfAI != 0)
		{
			if (NumberOfAI < 0)
			{
				aiSum = Math.RandomInt(1, -NumberOfAI);
			}
			else
			{
				aiSum = NumberOfAI - killCount;
			}
		}
		else
		{
	        Error("[QUESTS] WARNING: NumberOfAI shouldn't be set to 0, skipping this group...");
			return NULL;
		}
	
	    if (!Waypoints)
	    {
	    	Error("[QUESTS] WARNING: No waypoints (validate your file with a json validator or make sure at least the main position is also added as a waypoint), skipping this group...");
	       	return NULL;
	    }
	
		vector startpos = Waypoints[0];
		if (!startpos || startpos == "0 0 0")
		{
			Error("[QUESTS] WARNING: Couldn't find a spawn location. First waypoint is set to 0 0 0 or cannot be read by the system (validate your file with a json validator), skipping this group...");
			return NULL;
		}
	
		//! Safety in case the Y is bellow the ground
		startpos = ExpansionStatic.GetSurfacePosition(startpos);
		if (startpos[1] < Waypoints[0][1])
		startpos[1] = Waypoints[0][1];
		
		eAIQuestPatrol patrol = eAIQuestPatrol.CreateQuestPatrol(startpos, Waypoints, Behaviour, Loadout, aiSum, RespawnTime, DespawnTime, eAIFaction.Create(Faction), eAIFormation.Create(Formation), true, MinDistanceRadius, MaxDistanceRadius, DespawnRadius, Speed, ThreatSpeed, CanBeLooted, UnlimitedReload);
		patrol.SetAccuracy(MinAccuracy, MaxAccuracy);
		patrol.SetDamageReceivedMultiplier(DamageReceivedMultiplier);
		patrol.SetUnits(ClassNames);
		patrol.SetNPCName(NPCName);
	
	    return patrol;
	}
};

class eAIQuestPatrol : eAIDynamicPatrol
{
	string NPCName;

	void SetNPCName(string name)
	{
	    NPCName = name;
	}
	
	override private eAIBase CreateAI(vector pos)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif
	
		string unit;
	
		if (m_Units)
			unit = m_Units.GetQuasiRandomElementAvoidRepetition();
		else
			unit = eAISurvivor.GetQuasiRandom();
		
		eAIBase eAIUnit = eAIBase.Cast(GetGame().CreateObject(unit, pos));
		if (eAIUnit && !eAIUnit.m_Expansion_NetsyncData)
			eAIUnit.m_Expansion_NetsyncData = new ExpansionNetsyncData(eAIUnit);
	
       int iconIndex = 0;
       if (NPCName != "")
       {
           eAIUnit.m_Expansion_NetsyncData.Set(0, NPCName);
           iconIndex++;
       }
	
		eAIUnit.m_Expansion_NetsyncData.Set(iconIndex, "{3364F58EF7F7FBE3}DayZExpansion/Core/GUI/icons/misc/T_Soldier_256x256.edds");

		return eAIUnit;
	}

	static eAIQuestPatrol CreateQuestPatrol(vector pos, array<vector> waypoints, eAIWaypointBehavior behaviour, string loadout = "", int count = 1, int respawnTime = 600, int despawnTime = 600, eAIFaction faction = null, eAIFormation formation = null, bool autoStart = true, float minR = 300, float maxR = 800, float despawnR = 880, float speedLimit = 3.0, float threatspeedLimit = 3.0, bool canBeLooted = true, bool unlimitedReload = false)
	{
	#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAIQuestPatrol", "Create");
	#endif

		eAIQuestPatrol patrol;
		Class.CastTo(patrol, ((typename)eAIQuestPatrol).Spawn());
		patrol.m_Position = pos;
		patrol.m_Waypoints = waypoints;
		patrol.m_WaypointBehaviour = behaviour;
		patrol.m_NumberOfAI = count;
		patrol.m_Loadout = loadout;
		patrol.m_RespawnTime = respawnTime;
		patrol.m_DespawnTime = despawnTime;
		patrol.m_MinimumRadius = minR;
		patrol.m_MaximumRadius = maxR;
		patrol.m_DespawnRadius = despawnR;
		patrol.m_MovementSpeedLimit = speedLimit;
		patrol.m_MovementThreatSpeedLimit = threatspeedLimit;
		patrol.m_Faction = faction;
		patrol.m_Formation = formation;
		patrol.m_CanBeLooted = canBeLooted;
		patrol.m_UnlimitedReload = unlimitedReload;
		patrol.m_CanSpawn = true;
		if (patrol.m_Faction == null) patrol.m_Faction = new eAIFactionCivilian();
		if (patrol.m_Formation == null) patrol.m_Formation = new eAIFormationVee();
		if (autoStart) patrol.Start();
		return patrol;
	}
};
#endif