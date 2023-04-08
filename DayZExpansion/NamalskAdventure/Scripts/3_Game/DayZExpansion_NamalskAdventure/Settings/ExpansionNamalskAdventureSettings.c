/**
 * ExpansionNamalskAdventureSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static const string EXPANSION_NAMALSKADVENTURE_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "NamalskAdventureSettings.json";

/**@class		ExpansionNamalskAdventureSettingsBase
 * @brief		Spawn settings base class
 **/
class ExpansionNamalskAdventureSettingsBase: ExpansionSettingBase {};

/**@class		ExpansionSpawnSettings
 * @brief		Spawn settings class
 **/
class ExpansionNamalskAdventureSettings: ExpansionNamalskAdventureSettingsBase
{
	[NonSerialized()]
	protected const float DEFAULT_ANOMALY_SQUARE_SIZE = 500;
	
	static const int VERSION = 0;
	
	bool EnableAnomalies;
	ref array<ref ExpansionAnomalyDynamic> DynamicAnomalies;
	ref array<ref ExpansionAnomalyStatic> StaticAnomalies;
	bool EnableDynamic;
	bool EnableStatic;
	bool SpawnDynamicWithEVRStorms;
	bool SpawnStaticWithEVRStorms;
#ifdef EXPANSIONMODMARKET
	bool EnableMerchant;
	ref array<ref ExpansionMerchantPosition> MerchantPositions; //! Server
	ref array<ref ExpansionMerchantItemSet> MerchantItemSets; //! Server
#endif
#ifdef EXPANSIONMODAI
	bool EnableAISpawns;
	ref array<ref ExpansionAISpawnPosition> AISpawnPositions; //! Server
#endif

	[NonSerialized()]
	private bool m_IsLoaded;

	void ExpansionNamalskAdventureSettings()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		DynamicAnomalies = new array<ref ExpansionAnomalyDynamic>;
		StaticAnomalies = new array<ref ExpansionAnomalyStatic>;
	#ifdef EXPANSIONMODMARKET
		MerchantPositions = new array<ref ExpansionMerchantPosition>;
		MerchantItemSets = new array<ref ExpansionMerchantItemSet>;
	#endif
	#ifdef EXPANSIONMODAI
		AISpawnPositions = new array<ref ExpansionAISpawnPosition>;
	#endif
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		/*if (!ctx.Read(BackgroundImagePath))
		{
			Error("ExpansionSpawnSettings::OnRecieve BackgroundImagePath");
			return false;
		}*/

		m_IsLoaded = true;

		return true;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		//ctx.Write( BackgroundImagePath );
	}

	override int Send(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!IsMissionHost())
			return 0;

		auto rpc = ExpansionScriptRPC.Create();
		OnSend(rpc);
		rpc.Send(null, ExpansionSettingsRPC.NamalskAdventure, true, identity);

		return 0;
	}

	private void CopyInternal(ExpansionNamalskAdventureSettingsBase s)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		//! Nothing to do here yet
	}

	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_IsLoaded = true;

		bool save;
		bool settingsExist = FileExist(EXPANSION_NAMALSKADVENTURE_SETTINGS);
		if (settingsExist)
		{
			EXPrint("[ExpansionNamalskAdventureSettings] Load existing setting file:" + EXPANSION_NAMALSKADVENTURE_SETTINGS);

			ExpansionNamalskAdventureSettings settingsDefault = new ExpansionNamalskAdventureSettings;
			settingsDefault.Defaults();

			ExpansionNamalskAdventureSettingsBase settingsBase;
			JsonFileLoader<ExpansionNamalskAdventureSettingsBase>.JsonLoadFile(EXPANSION_NAMALSKADVENTURE_SETTINGS, settingsBase);
			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionNamalskAdventureSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_NAMALSKADVENTURE_SETTINGS + "\" to v" + VERSION);
				CopyInternal(settingsBase); //! Copy over old settings that have not changed.

				//! Nothing else to do here yet

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionNamalskAdventureSettings>.JsonLoadFile(EXPANSION_NAMALSKADVENTURE_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionNamalskAdventureSettings] No existing setting file:" + EXPANSION_NAMALSKADVENTURE_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (save)
			Save();

		return settingsExist;
	}

	override bool OnSave()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Print("[ExpansionNamalskAdventureSettings] Saving settings");
		JsonFileLoader<ExpansionNamalskAdventureSettings>.JsonSaveFile(EXPANSION_NAMALSKADVENTURE_SETTINGS, this);
		return true;
	}

	override void Update(ExpansionSettingBase setting)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.Update( setting );

		ExpansionSettings.SI_NamalskAdventure.Invoke();
	}

	override void Defaults()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_Version = VERSION;
		
		EnableAnomalies = true;
		DefaultNamalskAnomalies();
		
		EnableDynamic = true;
		EnableStatic = true;
		SpawnDynamicWithEVRStorms = true;
		SpawnStaticWithEVRStorms = true;
		
	#ifdef EXPANSIONMODMARKET
		EnableMerchant = true;
		DefaultNamalskMerchantData();
		DefaultNamalskMerchantItemData();
	#endif
	#ifdef EXPANSIONMODAI
		EnableAISpawns = true;
		DefaultAISpawnPositions();
	#endif
	}
	
	protected void DefaultNamalskAnomalies()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		array<string> anomalies = {"Expansion_Anomaly_Singularity", "Expansion_Anomaly_Teleport"};
		array<ref ExpansionLoot> anomaly_loot = new array<ref ExpansionLoot>;
		
		ExpansionLoot loot_1 = ExpansionLoot("Apple", null, 1, 1, NULL, 1);
		anomaly_loot.Insert(loot_1);
		
		//! Dynamic spawns
		ExpansionAnomalyDynamic anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5935.47, 32.1953, 10375.7), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		//anomaly.SetLoot(anomaly_loot, 1, 3, ExpansionAnomalyLootSpawnType.DYNAMIC);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(6851.73, 4.70827, 9946.15), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(6851.73, 4.70827, 9946.15), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(3770.85, 153.138, 8244.01), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		//! Static spawns
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5698.69, 21.3942, 9995.46)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5702.67, 21.3942, 9962.45)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5711.87, 21.3942, 9913.56)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5687.73, 21.3942, 9963.86)));
	}
	
#ifdef EXPANSIONMODMARKET
	protected void DefaultNamalskMerchantData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Merchant positions
		MerchantPositions.Insert(new ExpansionMerchantPosition(1, Vector(6831.73, 16.4077, 11346.5), Vector(310.0, 0, 0)));
		MerchantPositions.Insert(new ExpansionMerchantPosition(2, Vector(8355.28, 15.5027, 10777.1), Vector(230.0, 0, 0)));
		MerchantPositions.Insert(new ExpansionMerchantPosition(3, Vector(4425.47, 3.26632, 11278.7), Vector(300.0, 0, 0)));
		MerchantPositions.Insert(new ExpansionMerchantPosition(4, Vector(7835.72, 10.0497, 7667.68), Vector(215.0, 0, 0)));
		MerchantPositions.Insert(new ExpansionMerchantPosition(5, Vector(4010.66, 53.0584, 7587.21), Vector(320.0, 0, 0)));
	}
		
	protected void DefaultNamalskMerchantItemData()
	{
		//! Merchant item sets
		ExpansionMerchantItemSet itemSet = new ExpansionMerchantItemSet();
		itemSet.AddItem("nvgoggles", 5, ExpansionMarketTraderBuySell.CanOnlyBuy);
		MerchantItemSets.Insert(itemSet);
	}
#endif

#ifdef EXPANSIONMODAI
	protected void DefaultAISpawnPositions()
	{
		array<vector> shelterPositions = {"8551.87 15.7033 10530.6", "8553.74 15.7033 10533.4"};
		array<vector> waypoints = new array<vector>;
		string npcName = "Survivor Guard";
		//! AI guards - Jalovisco - Survivors Camp
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8597.78, 14.7325, 10529.7));
		ExpansionAISpawnPosition aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8597.78, 14.7325, 10529.7), Vector(215.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8579.5 14.7433 10545.3", "8583.97 14.7324 10542.4"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8619.11, 34.9968, 10479.6));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8619.11, 34.9968, 10479.6), Vector(335.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8634.89, 35.0096, 10512.5));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8634.89, 35.0096, 10512.5), Vector(280.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true); 
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8566.56, 35.1376, 10554.2));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8566.56, 35.1376, 10554.2), Vector(150.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8539.49, 35.1852, 10512.8));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8539.49, 35.1852, 10512.8), Vector(100.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8609.96 14.7887 10522.3"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8583.78, 14.7504, 10496.2));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8583.78, 14.7504, 10496.2), Vector(35.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);
		
		shelterPositions = {"8601.2 15.0546 10485.7"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8618.76, 14.7886, 10516.2));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8618.76, 14.7886, 10516.2), Vector(330.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8599.39 15.0545 10489"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8559.18, 15.682, 10528.2));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8559.18, 15.682, 10528.2), Vector(345.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8563.82 15.7036 10541.3"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8578.85, 14.7807, 10489.5));
		aiSpawn = new ExpansionAISpawnPosition(npcName, Vector(8578.85, 14.7807, 10489.5), Vector(230.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 1.0, 1.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);
	
		//! AI guards - Sebjan Reservoir - Resistance Camp
		waypoints = new array<vector>;
		waypoints.Insert(Vector(5999.97, 5.74131, 10082.5));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(5999.97, 5.74131, 10082.5), Vector(320.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleResistance", 1.0, 1.0, 1.0, 1.0, false, true));
		
		waypoints = new array<vector>;
		waypoints.Insert(Vector(6001.02, 5.78099, 10068.3));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(6001.02, 5.78099, 10068.3), Vector(195.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleResistance", 1.0, 1.0, 1.0, 1.0, false, true));
		
		waypoints = new array<vector>;
		waypoints.Insert(Vector(5980.95, 5.79031, 10040.9));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(5980.95, 5.79031, 10040.9), Vector(105.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleResistance", 1.0, 1.0, 1.0, 1.0, false, true));
		
		waypoints = new array<vector>;
		waypoints.Insert(Vector(6019.13, 5.80883, 10026.7));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(6019.13, 5.80883, 10026.7), Vector(105.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleResistance", 1.0, 1.0, 1.0, 1.0, false, true));

		//! Spawn NAC AI Soldier Units at A3
		waypoints = new array<vector>;
		waypoints.Insert(Vector(3601.81, 145.102, 6661.04));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(3601.81, 145.102, 6661.04), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC"));

		waypoints = new array<vector>;
		waypoints.Insert(Vector(3596.59, 170.731, 6659.7));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(3596.59, 170.731, 6659.7), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC"));

		waypoints = new array<vector>;
		waypoints.Insert(Vector(3605.11, 155.144, 6659.77));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(3605.11, 155.144, 6659.77), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC"));

		waypoints = new array<vector>;
		waypoints.Insert(Vector(3591.66, 143.823, 6718.72));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(3591.66, 143.823, 6718.72), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC"));
	}
#endif
	
#ifdef EXPANSIONMODMARKET
	ExpansionMerchantPosition GetMerchantPosition()
	{
		return MerchantPositions.GetRandomElement();
	}

	ExpansionMerchantItemSet GetMerchantItemSet()
	{
		return MerchantItemSets.GetRandomElement();
	}
#endif

	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Namalsk Adventure Settings";
	}
};