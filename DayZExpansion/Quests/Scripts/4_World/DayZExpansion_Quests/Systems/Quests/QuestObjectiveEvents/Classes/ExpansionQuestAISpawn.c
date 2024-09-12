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
class ExpansionQuestAISpawn_V27
{
	int NumberOfAI;
	string NPCName;
	ref TVectorArray Waypoints;
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
	ref TStringArray ClassNames;
	float SniperProneDistanceThreshold;
	
	float RespawnTime;
	float DespawnTime;
	float MinDistanceRadius;
	float MaxDistanceRadius;
	float DespawnRadius;
}

class ExpansionQuestAISpawn: ExpansionAIPatrol
{
	[NonSerialized()]
	static ref TStringArray eAI_UNITS = {"eAI_SurvivorF_Eva", "eAI_SurvivorF_Frida", "eAI_SurvivorF_Gabi", "eAI_SurvivorF_Helga", "eAI_SurvivorF_Irena", "eAI_SurvivorF_Judy", "eAI_SurvivorF_Keiko", "eAI_SurvivorF_Linda", "eAI_SurvivorF_Maria", "eAI_SurvivorF_Naomi", "eAI_SurvivorF_Baty", "eAI_SurvivorM_Boris", "eAI_SurvivorM_Cyril", "eAI_SurvivorM_Denis", "eAI_SurvivorM_Elias", "eAI_SurvivorM_Francis", "eAI_SurvivorM_Guo", "eAI_SurvivorM_Hassan", "eAI_SurvivorM_Indar", "eAI_SurvivorM_Jose", "eAI_SurvivorM_Kaito", "eAI_SurvivorM_Lewis", "eAI_SurvivorM_Manua", "eAI_SurvivorM_Mirek", "eAI_SurvivorM_Niki", "eAI_SurvivorM_Oliver", "eAI_SurvivorM_Peter", "eAI_SurvivorM_Quinn", "eAI_SurvivorM_Rolf", "eAI_SurvivorM_Seth", "eAI_SurvivorM_Taiki"};
	
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
		Name = name;
	}
	
	string GetName()
	{
		return Name;
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
		Behaviour = typename.EnumToString(eAIWaypointBehavior, behaviour);
	}
	
	void SetFormation(string formation)
	{
		Formation = formation;
	}
	
	string GetFormation()
	{
		return Formation;
	}
	
	void SetSpeed(int speed)
	{
		Speed = typename.EnumToString(eAIMovementSpeed, speed);
	}
	
	void SetThreatSpeed(int threatSpeed)
	{
		UnderThreatSpeed = typename.EnumToString(eAIMovementSpeed, threatSpeed);
	}
	
	void SetMinAccuracy(float minAccuracy)
	{
		AccuracyMin = minAccuracy;
	}
	
	float GetMinAccuracy()
	{
		return AccuracyMin;
	}
	
	void SetMaxAccuracy(float maxAccuracy)
	{
		AccuracyMax = maxAccuracy;
	}
	
	float GetMaxAccuracy()
	{
		return AccuracyMax;
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
		Units = classNames;
	}
	
	void AddClassName(string className)
	{
		if (!Units)
			Units = new TStringArray;
	
		Units.Insert(className);
	}
	
	TStringArray GetClassNames()
	{
		return Units;
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
		MinDistRadius = distance;
	}
	
	float GetMinDistanceRadius()
	{
		return MinDistRadius;
	}
	
	void SetMaxDistanceRadius(float distance)
	{
		MaxDistRadius = distance;
	}
	
	float GetMaxDistanceRadius()
	{
		return MaxDistRadius;
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		eAIQuestPatrol patrol = eAIQuestPatrol.CreateQuestPatrol(this);

		if (patrol && NumberOfAI >= killCount && killCount > 0)
			patrol.m_NumberOfAI = NumberOfAI - killCount;

		return patrol;
	}

	void CopyFrom(ExpansionQuestAISpawn_V27 aiSpawnV27)
	{
		SetNumberOfAI(aiSpawnV27.NumberOfAI);
		SetName(aiSpawnV27.NPCName);
		SetWaypoints(aiSpawnV27.Waypoints);
		SetLoadout(aiSpawnV27.Loadout);
		SetFaction(aiSpawnV27.Faction);
		SetBehaviour(aiSpawnV27.Behaviour);
		SetFormation(aiSpawnV27.Formation);
		SetSpeed(aiSpawnV27.Speed);
		SetThreatSpeed(aiSpawnV27.ThreatSpeed);
		SetMinAccuracy(aiSpawnV27.MinAccuracy);
		SetMaxAccuracy(aiSpawnV27.MaxAccuracy);
		SetCanBeLooted(aiSpawnV27.CanBeLooted);
		SetUnlimitedReload(aiSpawnV27.UnlimitedReload);
		SetThreatDistanceLimit(aiSpawnV27.ThreatDistanceLimit);
		SetDamageMultiplier(aiSpawnV27.DamageMultiplier);
		SetDamageReceivedMultiplier(aiSpawnV27.DamageReceivedMultiplier);
		SetClassNames(aiSpawnV27.ClassNames);
		SetSniperProneDistanceThreshold(aiSpawnV27.SniperProneDistanceThreshold);

		SetRespawnTime(aiSpawnV27.RespawnTime);
		SetDespawnTime(aiSpawnV27.DespawnTime);
		SetMinDistanceRadius(aiSpawnV27.MinDistanceRadius);
		SetMaxDistanceRadius(aiSpawnV27.MaxDistanceRadius);
		SetDespawnRadius(aiSpawnV27.DespawnRadius);
	}
};

class eAIQuestPatrol: eAIDynamicPatrol
{
	string NPCName;
	
	override void SetupAI(eAIBase ai)
	{
		super.SetupAI(ai);

		if (!ai.m_Expansion_NetsyncData)
			ai.m_Expansion_NetsyncData = new ExpansionNetsyncData(ai);

		ai.m_Expansion_NetsyncData.Set(0, NPCName);
		ai.m_Expansion_NetsyncData.Set(1, "{3364F58EF7F7FBE3}DayZExpansion/Core/GUI/icons/misc/T_Soldier_256x256.edds");
	}

	static eAIQuestPatrol CreateQuestPatrol(ExpansionQuestAISpawn config)
	{
		eAIQuestPatrol patrol = eAIDynamicPatrolT<eAIQuestPatrol>.CreateEx(config, vector.Zero);

		if (patrol && patrol.m_NumberOfAI == 1)
		{
			patrol.NPCName = config.Name;
			config.Name = "";
		}

		return patrol;
	}
};
#endif