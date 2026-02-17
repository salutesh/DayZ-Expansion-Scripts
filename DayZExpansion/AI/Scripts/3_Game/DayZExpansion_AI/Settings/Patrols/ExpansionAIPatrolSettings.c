/**
 * ExpansionAIPatrolSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAIPatrolSettingsBase: ExpansionSettingBase
{
    bool Enabled;

	float FormationScale;

	float DespawnTime;				    // If all players outside despawn radius, ticks up time. When despawn time reached, patrol is deleted.
	float RespawnTime;				    // Time in seconds before the dead patrol will respawn. If set to -1, they won't respawn
	float MinDistRadius;			    // If the player is closer than MinDistRadius from the spawn point, the patrol won't spawn
	float MaxDistRadius;			    // Same but if the player is further away than MaxDistRadius, the bots won't spawn

	float DespawnRadius;

	float AccuracyMin;
	float AccuracyMax;

	float ThreatDistanceLimit;
	float NoiseInvestigationDistanceLimit;
	float MaxFlankingDistance;
	int EnableFlankingOutsideCombat;
	float DamageMultiplier;
	float DamageReceivedMultiplier;
	float ShoryukenChance;
	float ShoryukenDamageMultiplier;
}

class ExpansionAIPatrolSettingsV4
{
	ref array< ref ExpansionAICrashPatrol > EventCrashPatrol;
	ref array< ref ExpansionAIPatrol_V4 > Patrol;
}

class ExpansionAIPatrolSettingsV19
{
	ref array< ref ExpansionAIObjectPatrol > ObjectPatrols;
	ref array< ref ExpansionAIPatrol_V4 > Patrols;
}

class ExpansionAIPatrolSettingsV24
{
	ref array< ref ExpansionAIObjectPatrol > ObjectPatrols;
}

/**@class		ExpansionAIPatrolSettings
 * @brief		Spawn settings class
 **/
class ExpansionAIPatrolSettings: ExpansionAIPatrolSettingsBase
{
	static const int VERSION = 32;

	ref map<string, ref array<ref ExpansionAIPatrolLoadBalancing>> LoadBalancingCategories = new map<string, ref array<ref ExpansionAIPatrolLoadBalancing>>;

	ref array< ref ExpansionAIDynamicSpawnBase > Patrols = {};
	
	[NonSerialized()]
	ref map<string,  ref array<ref ExpansionAIDynamicSpawnBase>> m_ObjectPatrols = new map<string, ref array<ref ExpansionAIDynamicSpawnBase>>;

	[NonSerialized()]
	ref array< ref ExpansionAIDynamicSpawnBase > m_Patrols = {};

	[NonSerialized()]
	private ref TStringArray m_UniquePersistentPatrolNames;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		return true;
	}
	
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
#endif
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionAIPatrolSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionAIPatrolSettings s )
	{
		ExpansionArray<ExpansionAIDynamicSpawnBase>.RefCopy(s.Patrols, Patrols);
		
		m_ObjectPatrols.Clear();
		foreach (string type, array<ref ExpansionAIDynamicSpawnBase> patrols: s.m_ObjectPatrols)
		{
			array<ref ExpansionAIDynamicSpawnBase> to = {};
			ExpansionArray<ExpansionAIDynamicSpawnBase>.RefCopy(patrols, to);
			m_ObjectPatrols[type] = to;
		}
		
		ExpansionArray<ExpansionAIDynamicSpawnBase>.RefCopy(s.m_Patrols, m_Patrols);

		ExpansionAIPatrolSettingsBase sb = s;
		CopyInternal( sb );
	}

	private void CopyInternal(  ExpansionAIPatrolSettingsBase s )
	{
		Enabled = s.Enabled;
		FormationScale = s.FormationScale;
		RespawnTime = s.RespawnTime;
		DespawnTime = s.DespawnTime;
		MinDistRadius = s.MinDistRadius;
		MaxDistRadius = s.MaxDistRadius;
		DespawnRadius = s.DespawnRadius;
		AccuracyMin = s.AccuracyMin;
		AccuracyMax = s.AccuracyMax;
		ThreatDistanceLimit = s.ThreatDistanceLimit;
		NoiseInvestigationDistanceLimit = s.NoiseInvestigationDistanceLimit;
		DamageMultiplier = s.DamageMultiplier;
		DamageReceivedMultiplier = s.DamageReceivedMultiplier;
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}
	
	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool settingsExist = FileExist(EXPANSION_AIPATROL_SETTINGS);

		int version;

		if (settingsExist)
		{
			EXPrint("[ExpansionAIPatrolSettings] Load existing setting file:" + EXPANSION_AIPATROL_SETTINGS);

			//! Try loading settings base
			ExpansionAIPatrolSettingsBase settingsBase;
			if (!ExpansionJsonFileParser<ExpansionAIPatrolSettingsBase>.Load(EXPANSION_AIPATROL_SETTINGS, settingsBase))
			{
				//! Use defaults, but DON'T save them
				Defaults();
				DefaultPatrols();

				version = VERSION;
			}
			else if (settingsBase.m_Version < 20)
			{
				CopyInternal(settingsBase);

				version = settingsBase.m_Version;
			}
			//! Try loading full setttings
			else if (!ExpansionJsonFileParser<ExpansionAIPatrolSettings>.Load(EXPANSION_AIPATROL_SETTINGS, this))
			{
				//! Use defaults, but DON'T save them
				Defaults();
				DefaultPatrols();

				version = VERSION;
			}
			else
			{
				version = m_Version;
			}

			if (version < VERSION)
			{
				EXPrint("[ExpansionAIPatrolSettings] Load - Converting v" + version + " \"" + EXPANSION_AIPATROL_SETTINGS + "\" to v" + VERSION);

				ExpansionAIPatrolSettings settingsDefault = new ExpansionAIPatrolSettings;
				settingsDefault.Defaults();

				if (version < 5)
				{
					//! Version 5: Renamed EventCrashPatrol to ObjectPatrols, renamed Patrol to Patrols
					ExpansionAIPatrolSettingsV4 settingsV4 = new ExpansionAIPatrolSettingsV4;
					if (ExpansionJsonFileParser<ExpansionAIPatrolSettingsV4>.Load(EXPANSION_AIPATROL_SETTINGS, settingsV4))
					{
						foreach (ExpansionAICrashPatrol crashPatrol: settingsV4.EventCrashPatrol)
						{
							if (!crashPatrol.Loadout)
								crashPatrol.Loadout = crashPatrol.LoadoutFile;
							crashPatrol.ObjectClassName = crashPatrol.EventName;
							Patrols.Insert(crashPatrol);
						}

						foreach (ExpansionAIPatrol_V4 patrolV4: settingsV4.Patrol)
						{
							if (!patrolV4.Loadout)
								patrolV4.Loadout = patrolV4.LoadoutFile;
							Patrols.Insert(patrolV4);
						}
					}
				}
				else if (version < 20)
				{
					//! Version 20: Renamed LoadoutFile to Loadout
					ExpansionAIPatrolSettingsV19 settingsV19;
					if (ExpansionJsonFileParser<ExpansionAIPatrolSettingsV19>.Load(EXPANSION_AIPATROL_SETTINGS, settingsV19))
					{
						foreach (ExpansionAIObjectPatrol objPatrolV19: settingsV19.ObjectPatrols)
						{
							if (!objPatrolV19.Loadout)
								objPatrolV19.Loadout = objPatrolV19.LoadoutFile;
							objPatrolV19.ObjectClassName = objPatrolV19.ClassName;
							Patrols.Insert(objPatrolV19);
						}

						foreach (ExpansionAIPatrol_V4 patrolV19: settingsV19.Patrols)
						{
							if (!patrolV19.Loadout)
								patrolV19.Loadout = patrolV19.LoadoutFile;
							Patrols.Insert(patrolV19);
						}
					}
				}
				else if (version < 25)
				{
					//! Version 25: Merged ObjectPatrols into Patrols, renamed ClassName to ObjectClassName
					ExpansionAIPatrolSettingsV24 settingsV24;
					if (ExpansionJsonFileParser<ExpansionAIPatrolSettingsV24>.Load(EXPANSION_AIPATROL_SETTINGS, settingsV24))
					{
						array<ref ExpansionAIDynamicSpawnBase> patrolsTmp = {};
						ExpansionArray<ExpansionAIDynamicSpawnBase>.RefCopy(Patrols, patrolsTmp);
						Patrols.Clear();

						foreach (ExpansionAIObjectPatrol objPatrolV24: settingsV24.ObjectPatrols)
						{
							objPatrolV24.ObjectClassName = objPatrolV24.ClassName;
							Patrols.Insert(objPatrolV24);
						}

						foreach (ExpansionAIDynamicSpawnBase patrolTmp: patrolsTmp)
						{
							Patrols.Insert(patrolTmp);
						}
					}
				}

				if (version < 6)
					DespawnRadius = MaxDistRadius * 1.1;

				if (version < 8)
				{
					DespawnTime = settingsDefault.DespawnTime;
					AccuracyMin = settingsDefault.AccuracyMin;
					AccuracyMax = settingsDefault.AccuracyMax;
				}

				if (version < 16)
					NoiseInvestigationDistanceLimit = settingsDefault.NoiseInvestigationDistanceLimit;

				if (version < 17 && !DamageReceivedMultiplier)
					DamageReceivedMultiplier = settingsDefault.DamageReceivedMultiplier;

				if (version < 23 && !LoadBalancingCategories.Count())
					LoadBalancingCategories = settingsDefault.LoadBalancingCategories;

				if (version < 24)
					FormationScale = settingsDefault.FormationScale;

				if (!MaxFlankingDistance)
					MaxFlankingDistance = settingsDefault.MaxFlankingDistance;

				if (version < 28 && !EnableFlankingOutsideCombat)
					EnableFlankingOutsideCombat = settingsDefault.EnableFlankingOutsideCombat;

				if (version < 32)
				{
					if (!ShoryukenChance)
						ShoryukenChance = settingsDefault.ShoryukenChance;

					if (!ShoryukenDamageMultiplier)
						ShoryukenDamageMultiplier = settingsDefault.ShoryukenDamageMultiplier;
				}

				m_Version = VERSION;
				save = true;
			}

			m_UniquePersistentPatrolNames = {};
		
			ExpansionAIDynamicSpawnBase policeCrashPatrol;

			foreach (ExpansionAIDynamicSpawnBase patrol: Patrols)
			{
				if (version < 4)
					patrol.UpdateSettings();

				if (version < 6)
				{
					if (patrol.MaxDistRadius <= 0)
						patrol.DespawnRadius = -2;
					else
						patrol.DespawnRadius = patrol.MaxDistRadius * 1.1;
				}

				if (version < 7)
					patrol.Formation = "RANDOM";

				if (version < 8)
				{
					patrol.DespawnTime = -1;
					patrol.AccuracyMin = -1;
					patrol.AccuracyMax = -1;
				}

				if (version < 11)
				{
					patrol.ThreatDistanceLimit = -1.0;
					patrol.DamageMultiplier = -1.0;
				}

				if (version < 16)
					patrol.NoiseInvestigationDistanceLimit = -1.0;

				if (version < 17 && !patrol.DamageReceivedMultiplier)
					patrol.DamageReceivedMultiplier = -1.0;

				if (version < 22 && !patrol.LootingBehaviour)
					patrol.SetDefaultLootingBehaviour();

				if (version < 24)
					patrol.FormationScale = -1;

				if (!patrol.DefaultStance)
					patrol.DefaultStance = typename.EnumToString(eAIStance, eAIStance.STANDING);

				if (!patrol.MaxFlankingDistance)
					patrol.MaxFlankingDistance = -1;

				if (version < 28 && !patrol.EnableFlankingOutsideCombat)
					patrol.EnableFlankingOutsideCombat = -1;

				if (version < 29 && patrol.DefaultLookAngle == -1)
					patrol.DefaultLookAngle = 0;

				if (version < 30 && patrol.NumberOfAI < 0)
				{
					patrol.NumberOfAIMax = -patrol.NumberOfAI;
					patrol.NumberOfAI = 1;
				}

				if (version < 32)
				{
					if (!patrol.ShoryukenChance)
						patrol.ShoryukenChance = -1;

					if (!patrol.ShoryukenDamageMultiplier)
						patrol.ShoryukenDamageMultiplier = -1;
				}

				if (patrol.ObjectClassName)
				{
					if (version < 2)
					{
						patrol.DefaultSpread();

						switch (patrol.ObjectClassName)
						{
							case "Wreck_Mi8":
								patrol.ObjectClassName = "Wreck_Mi8_Crashed";
								break;
							case "Wreck_PoliceCar":
								patrol.ObjectClassName = "Land_Wreck_sed01_aban1_police";
								policeCrashPatrol = patrol;
								break;
						}
					}

					if (version < 9)
					{
						patrol.RespawnTime = -2;
					}

					if (version < 23 && patrol.LoadBalancingCategory == "")
					{
						switch (patrol.ObjectClassName)
						{
							case "Wreck_UH1Y":
							case "Wreck_Mi8_Crashed":
								patrol.LoadBalancingCategory = "HelicopterWreck";
								break;

							case "ContaminatedArea_Static":
							case "ContaminatedArea_Dynamic":
								patrol.LoadBalancingCategory = "ContaminatedArea";
								break;
						}
					}

					switch (patrol.ObjectClassName)
					{
						case "ContaminatedArea_Static":
						case "ContaminatedArea_Dynamic":
							patrol.CanSpawnInContaminatedArea = true;
							break;
					}

					if (patrol.Persist)
					{
						EXError.Error(this, "Object patrol '" + patrol.ObjectClassName + "': Persistence is not supported for object patrols", {});
						patrol.Persist = false;
					}

					if (!ExpansionStatic.IsAnyOf(patrol.ObjectClassName, eAIRegisterDynamicPatrolSpawner.s_RegisteredTypes))
					{
						EXError.Error(this, "Ignoring object patrol classname '" + patrol.ObjectClassName + "' because it is not registered for dynamic patrol spawning", {});
					}
					else
					{
						AddObjectPatrol_Internal(patrol);
					}
				}
				else
				{
					if (version < 2)
					{
						patrol.MinSpreadRadius = 1;
						patrol.MaxSpreadRadius = 100;
					}

					if (patrol.Persist)
					{
						if (patrol.Name && m_UniquePersistentPatrolNames.Find(patrol.Name) == -1)
						{
							m_UniquePersistentPatrolNames.Insert(patrol.Name);
							patrol.GenerateBaseName();
						}
						else
						{
							EXError.Error(this, "Not an unique persistent patrol name: '" + patrol.Name + "'", {});
						}
					}

					m_Patrols.Insert(patrol);
				}
			}

			if (policeCrashPatrol)
			{
				TStringArray policeWrecks = {"Land_Wreck_sed01_aban2_police", "Land_Wreck_hb01_aban1_police", "Land_Wreck_hb01_aban2_police"};
				foreach (string policeWreck: policeWrecks)
				{
					AddObjectPatrol(policeCrashPatrol.NumberOfAI, policeCrashPatrol.Speed, policeCrashPatrol.UnderThreatSpeed, policeCrashPatrol.Behaviour, policeCrashPatrol.Faction, policeCrashPatrol.Loadout, policeCrashPatrol.CanBeLooted, policeCrashPatrol.UnlimitedReload, policeCrashPatrol.Chance, policeCrashPatrol.MinDistRadius, policeCrashPatrol.MaxDistRadius, policeWreck);
				}
			}
		}
		else
		{
			EXPrint("[ExpansionAIPatrolSettings] No existing setting file:" + EXPANSION_AIPATROL_SETTINGS + ". Creating defaults!");
			Defaults();
			DefaultPatrols();
			save = true;
		}

		foreach (auto categories: LoadBalancingCategories)
		{
			auto tracker = new ExpansionAIPatrolLoadBalancingTracker;

			foreach (auto category: categories)
			{
				category.m_PatrolCountTracker = tracker;
			}
		}

		if (save)
			Save();
		
		return settingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionAIPatrolSettings] Saving settings");
		
		JsonFileLoader<ExpansionAIPatrolSettings>.JsonSaveFile( EXPANSION_AIPATROL_SETTINGS, this );
		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_AI.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

        Enabled = true;

		FormationScale = -1;

        RespawnTime = 600;
        DespawnTime = 600;
        #ifdef DIAG_DEVELOPER
        MinDistRadius = 1;
        #else
        MinDistRadius = 400;
        #endif
        MaxDistRadius = 1000;
        DespawnRadius = 1100;
		AccuracyMin = -1;
		AccuracyMax = -1;

		ThreatDistanceLimit = -1;
		NoiseInvestigationDistanceLimit = -1;
		MaxFlankingDistance = -1;
		EnableFlankingOutsideCombat = -1;
		DamageMultiplier = -1;
		DamageReceivedMultiplier = -1;
		ShoryukenChance = 0.01;
		ShoryukenDamageMultiplier = 3.0;

		//! maxplayers = 60
		//! aicount_per_patrol = 3
		//! cost_per_ai = 1.5
		//! maxpatrols = (maxplayers - curplayers) / (aicount_per_patrol * cost_per_ai)
		LoadBalancingCategories["Global"] = {
			new ExpansionAIPatrolLoadBalancing(0, 10, 20),
			new ExpansionAIPatrolLoadBalancing(11, 20, 16),
			new ExpansionAIPatrolLoadBalancing(21, 30, 12),
			new ExpansionAIPatrolLoadBalancing(31, 40, 8),
			new ExpansionAIPatrolLoadBalancing(41, 50, 4),
			new ExpansionAIPatrolLoadBalancing(51, 255, 0)
		};
		LoadBalancingCategories["ObjectPatrol"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 5)};
		LoadBalancingCategories["Patrol"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 5)};
		LoadBalancingCategories["Quest"] = {new ExpansionAIPatrolLoadBalancing(0, 255, -1)};
		LoadBalancingCategories["HelicopterWreck"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 3)};
		LoadBalancingCategories["ContaminatedArea"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 2)};
		LoadBalancingCategories["MilitaryRoaming"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 5)};
		LoadBalancingCategories["MilitaryStatic"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 5)};
		LoadBalancingCategories["Survivor"] = {new ExpansionAIPatrolLoadBalancing(0, 255, 15)};
		LoadBalancingCategories["Example"] = {new ExpansionAIPatrolLoadBalancing(0, 255, -1)};
	}

	ExpansionAIDynamicSpawnBase AddObjectPatrol(int numberOfAI = 1, string speed = "JOG", string threatSpeed = "SPRINT", string bhv = "ALTERNATE", string faction = "West", string loadout = "", bool canBeLooted = true, int unlimitedReload = 0, float chance = 1.0, float minDistRadius = -1, float maxDistRadius = -1, string objClassName = "Wreck_UH1Y", TVectorArray waypoints = null)
	{
		ExpansionAIDynamicSpawnBase patrol = new ExpansionAIDynamicSpawnBase(numberOfAI, speed, threatSpeed, bhv, faction, loadout, canBeLooted, unlimitedReload, chance, minDistRadius, maxDistRadius);

		patrol.ObjectClassName = objClassName;
		if (!waypoints)
			patrol.DefaultSpread();
		patrol.Waypoints = waypoints;

		Patrols.Insert(patrol);

		AddObjectPatrol_Internal(patrol);

		return patrol;
	}

	private void AddObjectPatrol_Internal(ExpansionAIDynamicSpawnBase patrol)
	{
		string type = patrol.ObjectClassName;

		type.ToLower();

		array<ref ExpansionAIDynamicSpawnBase> objectPatrols;
		if (!m_ObjectPatrols.Find(type, objectPatrols))
		{
			objectPatrols = {};
			m_ObjectPatrols[type] = objectPatrols;
		}

		objectPatrols.Insert(patrol);
	}

	ExpansionAIPatrol AddPatrol(int numberOfAI = 1, string speed = "JOG", string threatSpeed = "SPRINT", string bhv = "ALTERNATE", string faction = "West", string loadout = "HumanLoadout", bool canBeLooted = true, int unlimitedReload = 0, float chance = 1.0, float minDistRadius = -1, float maxDistRadius = -1, float respawnTime = -2, float maxSpreadRadius = 0, TVectorArray waypoints = null)
	{
		ExpansionAIPatrol patrol = new ExpansionAIPatrol(numberOfAI, speed, threatSpeed, bhv, faction, loadout, canBeLooted, unlimitedReload, chance, minDistRadius, maxDistRadius, respawnTime, maxSpreadRadius, waypoints);

		Patrols.Insert(patrol);

		m_Patrols.Insert(patrol);

		return patrol;
	}

	void DefaultPatrols()
	{
        string worldName = ExpansionStatic.GetCanonicalWorldName();

        DefaultObjectPatrols(worldName);
        DefaultPatrols(worldName);
	}

    void DefaultObjectPatrols(string worldName)
    {
		if (GetExpansionSettings().GetLog().AIObjectPatrol)
			GetExpansionSettings().GetLog().PrintLog("[AI Object Patrol] Generating default object patrols for \"%1\"", worldName);

		ExpansionAIDynamicSpawnBase patrol;
		int unlimitedReload = eAITargetType.ANIMAL | eAITargetType.INFECTED;
		string militaryFaction;

		switch (worldName)
		{
			case "sakhal":
				militaryFaction = "East";
				break;

			default:
				militaryFaction = "West";
				break;
		}

		//! -------------------------------------------------------------------
		//! Heli crash patrols
		//! -------------------------------------------------------------------

		patrol = AddObjectPatrol(-2, "JOG", "SPRINT", "LOOP_OR_ALTERNATE", "West", "", true, unlimitedReload, 0.5, -1, -1, "Wreck_UH1Y");
		patrol.DefaultStance = "CROUCHED";
		patrol.LoadBalancingCategory = "HelicopterWreck";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_GLOVES | CLOTHING_FEET | CLOTHING_SIMILAR | UPGRADE";

		patrol = AddObjectPatrol(-2, "JOG", "SPRINT", "LOOP_OR_ALTERNATE", "East", "", true, unlimitedReload, 0.5, -1, -1, "Wreck_Mi8_Crashed");
		patrol.DefaultStance = "CROUCHED";
		patrol.LoadBalancingCategory = "HelicopterWreck";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_GLOVES | CLOTHING_FEET | CLOTHING_SIMILAR | UPGRADE";

		//! -------------------------------------------------------------------
		//! Police patrols
		//! -------------------------------------------------------------------

		patrol = AddObjectPatrol(-2, "WALK", "SPRINT", "LOOP_OR_ALTERNATE", "Guards", "PoliceLoadout", true, unlimitedReload, 0.1, -1, -1, "Land_City_PoliceStation");
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";
		patrol.WaypointInterpolation = "UniformCubic";

		patrol = AddObjectPatrol(-2, "WALK", "SPRINT", "LOOP_OR_ALTERNATE", "Guards", "PoliceLoadout", true, unlimitedReload, 0.1, -1, -1, "Land_Village_PoliceStation");
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";
		patrol.WaypointInterpolation = "UniformCubic";

		//! -------------------------------------------------------------------
		//! Contaminated area patrols
		//! -------------------------------------------------------------------

		patrol = AddObjectPatrol(-2, "JOG", "SPRINT", "LOOP_OR_ALTERNATE", militaryFaction, "NBCLoadout", true, unlimitedReload, 0.5, -1, -1, "ContaminatedArea_Static");
		patrol.CanSpawnInContaminatedArea = true;
		patrol.LoadBalancingCategory = "ContaminatedArea";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_HEADGEAR | CLOTHING_MASK | CLOTHING_GLOVES | CLOTHING_FEET | CLOTHING_SIMILAR";
		patrol.WaypointInterpolation = "UniformCubic";

		patrol = AddObjectPatrol(-2, "JOG", "SPRINT", "LOOP_OR_ALTERNATE", militaryFaction, "NBCLoadout", true, unlimitedReload, 0.5, -1, -1, "ContaminatedArea_Dynamic");
		patrol.CanSpawnInContaminatedArea = true;
		patrol.LoadBalancingCategory = "ContaminatedArea";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_HEADGEAR | CLOTHING_MASK | CLOTHING_GLOVES | CLOTHING_FEET | CLOTHING_SIMILAR";
		patrol.WaypointInterpolation = "UniformCubic";

		//! -------------------------------------------------------------------
		//! Military patrols - static
		//! -------------------------------------------------------------------

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.1, -1, -1, "Land_Mil_Tower_Small", {"-0.0151265 1.32325 1.19653"});
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.2, -1, -1, "Land_Mil_GuardTower", {"0.0688021 3.62166 -3.98126"});
		patrol.DefaultLookAngle = 180;
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.1, -1, -1, "Land_Mil_GuardBox_Brown", {"0.161377 -0.789551 -0.782226"});
		patrol.DefaultLookAngle = 180;
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.1, -1, -1, "Land_Mil_GuardBox_Green", {"0.161377 -0.789551 -0.782226"});
		patrol.DefaultLookAngle = 180;
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.1, -1, -1, "Land_Mil_GuardBox_Smooth", {"0.161377 -0.789551 -0.782226"});
		patrol.DefaultLookAngle = 180;
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.1, -1, -1, "Land_Mil_Fortified_Nest_Small", {"0 -0.965636 0"});
		patrol.DefaultLookAngle = 180;
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "HALT", militaryFaction, "", true, unlimitedReload, 0.1, -1, -1, "Land_Mil_GuardShed", {"0 -0.685577 0"});
		patrol.DefaultLookAngle = 180;
		patrol.LoadBalancingCategory = "MilitaryStatic";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		//! -------------------------------------------------------------------
		//! Military patrols - roaming (local)
		//! -------------------------------------------------------------------

		patrol = AddObjectPatrol(-3, "WALK", "SPRINT", "ROAMING_LOCAL", militaryFaction, "", true, unlimitedReload, 0.2, -1, -1, "Land_Mil_Barracks1", {"0 -1.94586 0"});
		patrol.LoadBalancingCategory = "MilitaryRoaming";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "ROAMING_LOCAL", militaryFaction, "", true, unlimitedReload, 0.2, -1, -1, "Land_Mil_Barracks_Round", {"-1.0607 -0.691666 2.30131"});
		patrol.LoadBalancingCategory = "MilitaryRoaming";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(1, "WALK", "SPRINT", "ROAMING_LOCAL", militaryFaction, "", true, unlimitedReload, 0.2, -1, -1, "Land_Mil_Tent_Big1_2", {"0 0 0"});
		patrol.LoadBalancingCategory = "MilitaryRoaming";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";

		patrol = AddObjectPatrol(-2, "WALK", "SPRINT", "ROAMING_LOCAL", militaryFaction, "", true, unlimitedReload, 0.2, -1, -1, "Land_Mil_Tent_Big1_4", {"0 0 0"});
		patrol.LoadBalancingCategory = "MilitaryRoaming";
		patrol.LootingBehaviour = "DEFAULT | CLOTHING_BODY | CLOTHING_LEGS | CLOTHING_FEET | CLOTHING_SIMILAR";
	}

    void DefaultPatrols(string worldName)
    {
        if (GetExpansionSettings().GetLog().AIPatrol)
            GetExpansionSettings().GetLog().PrintLog("[AI Patrol] Generating default patrols for \"%1\"", worldName);

		DefaultRoamingPatrols(worldName);
	}

	void DefaultRoamingPatrols(string worldName)
	{
		TVectorArray positions = GetSpawnPointPositions();
		array<ref TVectorArray> clusters = GetClusters(positions, 350);

		if (clusters.Count() > 0)
			DefaultRoamingPatrols(clusters);
		else if (GetExpansionSettings().GetLog().AIPatrol)
			GetExpansionSettings().GetLog().PrintLog("[AI Patrol] WARNING: No spawn points found for \"%1\"", worldName);
    }

	//! Extract unique positions from all freshspawn groups
	TVectorArray GetSpawnPointPositions()
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(true, this);
	#endif

		auto ce = ExpansionCE.GetInstance();
		ce.LoadPlayerSpawnpoints();

		TVectorArray positions = {};

		if (ce.PlayerSpawnPoints.Fresh)
		{
			foreach (auto group: ce.PlayerSpawnPoints.Fresh.Groups)
			{
				foreach (vector position: group.Positions)
				{
					if (positions.Find(position) == -1)
						positions.Insert(position);
				}
			}
		}

		return positions;
	}

	array<ref TVectorArray> GetClusters(TVectorArray positions, float maxDistance)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(true, this);
	#endif

		array<ref TVectorArray> clusters = {};
		map<int, bool> seen = new map<int, bool>;

		for (int i = positions.Count() - 1; i >= 0; --i)
		{
			if (seen[i])
				continue;

			vector point1 = positions[i];

			TVectorArray cluster = {};

			seen[i] = true;
			cluster.Insert(point1);

			bool update = true;
			while (update)
			{
				update = false;

				for (int j = i - 1; j >= 0; --j)
				{
					if (seen[j])
						continue;

					vector candidate = positions[j];

					foreach (vector point: cluster)
					{
						if (vector.DistanceSq(candidate, point) <= maxDistance * maxDistance)
						{
							seen[j] = true;
							cluster.Insert(candidate);
							//! Since we've added a new point to the cluster, we need to check
							//! remaining unadded points against the newly added point as well
							update = true;  
							break;
						}
					}

					//! Limit max points per cluster
					if (update && cluster.Count() == 5)
					{
						update = false;
						break;
					}
				}
			}

			clusters.Insert(cluster);
		}

		return clusters;
	}

	void DefaultRoamingPatrols(array<ref TVectorArray> clusters)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(true, this);
	#endif

		int include;
		include |= ExpansionLocationType.CAMP;
		include |= ExpansionLocationType.RUIN;
		include |= ExpansionLocationType.SETTLEMENT;

		auto locations = ExpansionLocationT<ExpansionLocation>.GetWorldLocations(include);

		ExpansionAIDynamicSpawnBase patrol;
		int unlimitedReload = eAITargetType.ANIMAL | eAITargetType.INFECTED;

		map<string, int> seen = new map<string, int>;

		TStringArray phonetics = {"Alpha",
								  "Bravo",
								  "Charlie",
								  "Delta",
								  "Echo",
								  "Foxtrot",
								  "Golf",
								  "Hotel",
								  "India",
								  "Juliett",
								  "Kilo",
								  "Lima",
								  "Mike",
								  "November",
								  "Oscar",
								  "Papa",
								  "Quebec",
								  "Romeo",
								  "Sierra",
								  "Tango",
								  "Uniform",
								  "Victor",
								  "Whiskey",
								  "X-ray",
								  "Yankee",
								  "Zulu"};

		int n;

		foreach (TVectorArray points: clusters)
		{
			patrol = AddPatrol(1, "JOG", "SPRINT", "ROAMING", "RANDOM", "FreshSpawnLoadout", true, unlimitedReload, 0.8, -1, 21722, -2, 0, points);
			patrol.DespawnRadius = 21722;
			patrol.LoadBalancingCategory = "Survivor";
			patrol.LootingBehaviour = "ALL";
			patrol.Persist = true;

			ExpansionLocation closestLocation = null;
			float closestDistSq = 4000000;  //! 2000 m

			//! Determine center of cluster
			vector center = vector.Zero;

			foreach (vector point: points)
			{
				center[0] = center[0] + point[0];
				center[2] = center[2] + point[2];
			}

			int count = points.Count();

			center[0] = center[0] / count;
			center[2] = center[2] / count;
			
			//! Find closest named location
			foreach (ExpansionLocation location: locations)
			{
				if (!location.Name)
					continue;

				//! We divide by radius so that larger settlements are preferred over smaller ones
				float distSq = ExpansionMath.Distance2DSq(center, location.Position) / location.Radius;

				if (distSq < closestDistSq)
				{
					closestDistSq = distSq;
					closestLocation = location;
				}
			}

			string name;

			if (closestLocation)
			{
				name = closestLocation.Name;

				if (seen[name] || closestDistSq * closestLocation.Radius > closestLocation.Radius * closestLocation.Radius)
				{
					string cardinalDir = GetCardinalDirection(closestLocation.Position, center);

					if (cardinalDir)
						name = string.Format("%1 %2", name, cardinalDir);
				}
			}
			else
			{
				//! Fall back to generic name using phonetics
				n = seen[""];

				if (n >= phonetics.Count())
					n = 0;

				name = string.Format("Spawnpoint %1", phonetics[n]);

				seen[""] = ++n;
			}

			n = seen[name];
			seen[name] = ++n;

			if (n > 1)
				name = string.Format("%1 %2", name, n);

			patrol.Name = name;
		}
	}

	string GetCardinalDirection(vector a, vector b)
	{
		float dx = b[0] - a[0];  //! East / West
		float dz = b[2] - a[2];  //! North / South

		//! If positions are basically identical
		if (Math.AbsFloat(dx) < 0.01 && Math.AbsFloat(dz) < 0.01)
			return "";

		float angle = Math.Atan2(dx, dz) * Math.RAD2DEG;

		if (angle < 0)
			angle += 360;

		if (angle >= 337.5 || angle < 22.5)
			return "North";
		else if (angle < 67.5)
			return "North-East";
		else if (angle < 112.5)
			return "East";
		else if (angle < 157.5)
			return "South-East";
		else if (angle < 202.5)
			return "South";
		else if (angle < 247.5)
			return "South-West";
		else if (angle < 292.5)
			return "West";

		return "North-West";
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "AI Patrol Settings";
	}
};
