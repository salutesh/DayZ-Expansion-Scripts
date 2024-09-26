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
class ExpansionNamalskAdventureSettingsBase: ExpansionSettingBase
{
	bool EnableAnomalies;
	bool EnableDynamic;
	bool EnableStatic;
	bool SpawnDynamicWithEVRStorms;
	bool SpawnStaticWithEVRStorms;
	bool EnableAntennaEvent;
	bool EnableA1BunkerEvent;
	
	#ifdef EXPANSIONMODMARKET
	bool EnableMerchant;
	ref array<ref ExpansionMerchantPosition> MerchantPositions = {}; //! Server
	ref array<ref ExpansionMerchantItemSet> MerchantItemSets = {}; //! Server
	#endif
	
	#ifdef EXPANSIONMODAI
	bool EnableAISpawns;
	ref array<ref ExpansionAISpawnPosition> AISpawnPositions = {}; //! Server
	#endif

	bool EnableSupplyCrates;
	bool ClearPlayerFactions;
};

class ExpansionNamalskAdventureSettingsV7
{
	ref array<ref ExpansionAnomalyDynamicV7> DynamicAnomalies = {};
	ref array<ref ExpansionAnomalyStaticV7> StaticAnomalies = {};
	ref array<ref ExpansionSupplyCrateSetupV7> SupplyCrateSpawns = {};
}

/**@class		ExpansionSpawnSettings
 * @brief		Spawn settings class
 **/
class ExpansionNamalskAdventureSettings: ExpansionNamalskAdventureSettingsBase
{
	static const int VERSION = 8;

	ref array<ref ExpansionAnomalyDynamic> DynamicAnomalies = {};
	ref array<ref ExpansionAnomalyStatic> StaticAnomalies = {};
	ref array<ref ExpansionSupplyCrateSetup> SupplyCrateSpawns = {};

	[NonSerialized()]
	protected const float DEFAULT_ANOMALY_SQUARE_SIZE = 400;
	[NonSerialized()]
	protected const float DEFAULT_ANOMALY_SPAWN_AMOUNT = 7;

	[NonSerialized()]
	private bool m_IsLoaded;

	override bool OnRecieve(ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_IsLoaded = true;

		return true;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	override int Send(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!IsMissionHost())
			return 0;

		auto rpc = CreateRPC();
		OnSend(rpc);
		rpc.Expansion_Send(true, identity);

		return 0;
	}

	protected void CopyInternal(ExpansionNamalskAdventureSettingsBase s)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		EnableAnomalies = s.EnableAnomalies;
		EnableDynamic = s.EnableDynamic;
		EnableStatic = s.EnableStatic;
		SpawnDynamicWithEVRStorms = s.SpawnDynamicWithEVRStorms;
		SpawnStaticWithEVRStorms = s.SpawnStaticWithEVRStorms;
		EnableAntennaEvent = s.EnableAntennaEvent;
		EnableA1BunkerEvent = s.EnableA1BunkerEvent;
		
		#ifdef EXPANSIONMODMARKET
		EnableMerchant = s.EnableMerchant;
		MerchantPositions = s.MerchantPositions;
		MerchantItemSets = s.MerchantItemSets;
		#endif
		
		#ifdef EXPANSIONMODAI
		EnableAISpawns = s.EnableAISpawns;
		AISpawnPositions = s.AISpawnPositions;
		#endif

		EnableSupplyCrates = s.EnableSupplyCrates;
		ClearPlayerFactions = s.ClearPlayerFactions;
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

			ExpansionNamalskAdventureSettingsBase settingsBase;
			JsonFileLoader<ExpansionNamalskAdventureSettingsBase>.JsonLoadFile(EXPANSION_NAMALSKADVENTURE_SETTINGS, settingsBase);
			if (settingsBase.m_Version < VERSION)
			{
				ExpansionNamalskAdventureSettings settingsDefault = new ExpansionNamalskAdventureSettings;
				settingsDefault.Defaults();

				EXPrint("[ExpansionNamalskAdventureSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_NAMALSKADVENTURE_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 8)
				{
					CopyInternal(settingsBase); //! Copy over old settings that have not changed.

					//! Convert loot
					ExpansionNamalskAdventureSettingsV7 settingsV7;
					JsonFileLoader<ExpansionNamalskAdventureSettingsV7>.JsonLoadFile(EXPANSION_NAMALSKADVENTURE_SETTINGS, settingsV7);

					foreach (ExpansionAnomalyDynamicV7 dynamicAnomalyV7: settingsV7.DynamicAnomalies)
					{
						DynamicAnomalies.Insert(dynamicAnomalyV7.ConvertDynamic());
					}

					foreach (ExpansionAnomalyStaticV7 staticAnomalyV7: settingsV7.StaticAnomalies)
					{
						StaticAnomalies.Insert(staticAnomalyV7.ConvertStatic());
					}

					foreach (ExpansionSupplyCrateSetupV7 supplyCrateV7: settingsV7.SupplyCrateSpawns)
					{
						SupplyCrateSpawns.Insert(supplyCrateV7.Convert());
					}
				}
				else
				{
					JsonFileLoader<ExpansionNamalskAdventureSettings>.JsonLoadFile(EXPANSION_NAMALSKADVENTURE_SETTINGS, this);
				}
				
				if (settingsBase.m_Version < 6)
				{
					EnableAntennaEvent = settingsDefault.EnableAntennaEvent;
					EnableA1BunkerEvent = settingsDefault.EnableA1BunkerEvent;
				}
				
				if (settingsBase.m_Version < 7)
				{
					ClearPlayerFactions = settingsDefault.ClearPlayerFactions;
				}
				
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
			DefaultNamalskAnomalies();
		#ifdef EXPANSIONMODMARKET
			DefaultNamalskMerchantData();
			DefaultNamalskMerchantItemData();
		#endif
		#ifdef EXPANSIONMODAI
			DefaultAISpawnPositions();
		#endif
			DefaultSupplyCrates();
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

		EnableDynamic = true;
		EnableStatic = true;
		SpawnDynamicWithEVRStorms = true;
		SpawnStaticWithEVRStorms = false;
		EnableAntennaEvent = false;
		EnableA1BunkerEvent = true;

		#ifdef EXPANSIONMODMARKET
		EnableMerchant = true;
		#endif
		
		#ifdef EXPANSIONMODAI
		EnableAISpawns = true;
		#endif

		EnableSupplyCrates = true;
		
		ClearPlayerFactions = false;
	}

	protected void DefaultNamalskAnomalies()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		array<string> anomalies = {"Expansion_Anomaly_Singularity", "Expansion_Anomaly_Teleport"};
		array<ref ExpansionLoot> anomaly_loot = new array<ref ExpansionLoot>;

		ExpansionLoot loot_1 = ExpansionLoot("Apple", null, 1, 1, NULL, 1);
		anomaly_loot.Insert(loot_1);

		//! Dynamic spawns
		ExpansionAnomalyDynamic anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(1225.67, 2.00018, 11837.4), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(3774.44, 144.313, 8244.01), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4706.39, 85.6586, 8477.48), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4706.39, 85.6586, 8477.48), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5549.58, 43.0471, 9543.73), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5744.57, 17.2946, 10770.0), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4502.5, 19.5023, 10854), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(6708.11, 15.0, 11202.7), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(7047.19, 40.0, 5817.77), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(7263.92, 238.711, 7058.66), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(7312.8, 84.37, 7994.47), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(6273.98, 21.0682, 9351.2), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4962.06, 50, 7988.59), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(8170.71, 15.1869, 10824.1), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(8170.71, 15.1869, 10824.1), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(3937.58, 5.72824, 10020.3), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5489.87, 101.82, 7892.35), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5015.51, 47.1122, 6555.13), DEFAULT_ANOMALY_SQUARE_SIZE, DEFAULT_ANOMALY_SPAWN_AMOUNT);
		DynamicAnomalies.Insert(anomaly);

		//! Static spawns
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(4320.681152, 77.882248, 8105.860352)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(4238.989746, 77.928940, 8120.389648)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(4150.605469, 78.103477, 8044.797363)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(4380.786621, 77.689484, 8090.837402)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(4298.584961, 78.741501, 8046.419922)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(3785.281250, 153.848358, 8266.510742)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(3762.224365, 158.773376, 8197.483398)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(3738.147949, 153.526825, 8231.050781)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5697.937500, 21.480026, 9973.298828)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5680.349609, 21.919327, 9994.563477)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(5712.300293, 21.977049, 9917.190430)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(6683.45, 15.0424, 11396.9)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(6830.53, 15.1924, 11271.2)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(6629.39, 15.0425, 11300.1)));
		StaticAnomalies.Insert(new ExpansionAnomalyStatic(anomalies, Vector(8166.83, 15.9024, 10791.1)));
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
		ExpansionAISpawnPosition aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8597.78, 14.7325, 10529.7), Vector(215.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8579.5 14.7433 10545.3", "8583.97 14.7324 10542.4"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8619.11, 34.9968, 10479.6));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8619.11, 34.9968, 10479.6), Vector(335.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8634.89, 35.0096, 10512.5));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8634.89, 35.0096, 10512.5), Vector(280.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8566.56, 35.1376, 10554.2));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8566.56, 35.1376, 10554.2), Vector(150.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8539.49, 35.1852, 10512.8));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8539.49, 35.1852, 10512.8), Vector(100.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8609.96 14.7887 10522.3"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8583.78, 14.7504, 10496.2));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8583.78, 14.7504, 10496.2), Vector(35.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8601.2 15.0546 10485.7"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8618.76, 14.7886, 10516.2));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8618.76, 14.7886, 10516.2), Vector(330.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8599.39 15.0545 10489"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8559.18, 15.682, 10528.2));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8559.18, 15.682, 10528.2), Vector(345.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		shelterPositions = {"8563.82 15.7036 10541.3"};
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8578.85, 14.7807, 10489.5));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(8578.85, 14.7807, 10489.5), Vector(230.0, 0, 0), waypoints, "GorkaLoadout", "InvincibleSurvivors", 3.0, 5.0, 1.0, 1.0, false, true);
		aiSpawn.SetShelterPositions(shelterPositions);
		AISpawnPositions.Insert(aiSpawn);

		//! Spawn NAC AI Soldier Units at A1 Bunker
		npcName = "NAC Soldier";
		waypoints = new array<vector>;
		waypoints.Insert(Vector(1927.28, 211.538, 1242.27));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(1927.28, 211.538, 1242.27), Vector(257.0, 0, 0), waypoints, "Namalsk_NAC_Bunker", "NAC", 3.0, 5.0, 0.5, 1.0, true, true, 800.0, 1.0);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(1918.3, 211.528, 1298.24));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(1918.3, 211.528, 1298.24), Vector(263.0, 0, 0), waypoints, "Namalsk_NAC_Bunker", "NAC", 3.0, 5.0, 0.5, 1.0, true, true, 800.0, 1.0);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(1884.75, 196.7, 1315.69));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(1884.75, 196.7, 1315.69), Vector(173.0, 0, 0), waypoints, "Namalsk_NAC_Bunker", "NAC", 3.0, 5.0, 0.5, 1.0, true, true, 800.0, 1.0);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(1850.79, 206.575, 1391.69));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(1850.79, 206.575, 1391.69), Vector(284.0, 0, 0), waypoints, "Namalsk_NAC_Bunker", "NAC", 3.0, 5.0, 0.5, 1.0, true, true, 800.0, 1.0);
		AISpawnPositions.Insert(aiSpawn);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(1836.86, 206.579, 1343.54));
		waypoints.Insert(Vector(1839.01, 206.581, 1354.86));
		waypoints.Insert(Vector(1835.77, 206.591, 1385.99));
		waypoints.Insert(Vector(1831.8, 206.603, 1390.23));
		waypoints.Insert(Vector(1829.13, 206.591, 1384.53));
		waypoints.Insert(Vector(1832.58, 206.581, 1351.4));
		aiSpawn = new ExpansionAISpawnPosition();
		aiSpawn.Set(npcName, Vector(1836.86, 206.579, 1343.54), Vector(359.0, 0, 0), waypoints, "Namalsk_NAC_Bunker", "NAC", 3.0, 5.0, 0.5, 1.0, true, true, 800.0, 1.0);
		AISpawnPositions.Insert(aiSpawn);
	}
	#endif

	protected void DefaultSupplyCrates()
	{
		TStringArray nacPlateCarrierAtt = {"dzn_platecarrierpouches_nac", "dzn_platecarrierholster_nac"};
		TStringArray nvgAtt = {"NVGoggles"};

		//! A1 Bunker supply crates
		ExpansionSupplyCrateSetup supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Bunker_Green", "1898.079956 197.143005 1318.750000", "-4.545284 0.000000 0.000000");
		supplyCrate.AddLoot("ExpansionAnomalyCoreProtectiveCase", NULL, 0.3, -1, NULL, 1, 1);
		//supplyCrate.AddLoot("Expansion_KeyCard_NA_Antenna", NULL, 0.3, -1, NULL, 1, 1); //! Don't spawn it yet.
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Bunker_Green", "1818.742188 207.013809 1393.550903", "71.999947 -0.000000 -0.000000");
		supplyCrate.AddLoot("NVGHeadstrap", nvgAtt, 0.2, -1, NULL, 1);
		supplyCrate.AddLoot("dzn_platecarriervest_nac", nacPlateCarrierAtt, 1.0, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Bunker_Green", "1876.101807 200.092636 1318.457642", "-179.999878 0.000000 -0.000000");
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Bunker_Green", "1874.483887 197.182037 1290.613403", "-89.999901 0.000000 0.000000");
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		//! World military supply crates
		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "4582.287598 43.907669 8819.691406", "53.999985 -0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "5656.101074 40.045158 9421.748047", "-117.000031 0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "6339.882324 66.714439 10603.136719", "-99.000038 0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "3872.280762 160.241196 7141.190918", "89.999985 -0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "5241.186523 84.257492 8339.794922", "27.000008 -0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "4878.279785 68.661240 7905.549805", "-98.999931 0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "5994.645020 37.987732 6632.409668", "81.000008 -0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "4154.082520 74.418991 7756.675293", "125.999924 0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);

		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_World_T1_Green", "3635.906006 99.478661 7498.728027", "0.000000 -0.000000 -0.000000");
		supplyCrate.AddLoot("Expansion_KeyCard_A1_B1", NULL, 0.55, -1, NULL, 1, 1);
		supplyCrate.InsertLoot(ExpansionLootDefaults.SupplyCrate_Military());
		SupplyCrateSpawns.Insert(supplyCrate);
	}

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

	array<ref ExpansionSupplyCrateSetup> GetSupplyCrateSpawns()
	{
		return SupplyCrateSpawns;
	}

	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Namalsk Adventure Settings";
	}
};