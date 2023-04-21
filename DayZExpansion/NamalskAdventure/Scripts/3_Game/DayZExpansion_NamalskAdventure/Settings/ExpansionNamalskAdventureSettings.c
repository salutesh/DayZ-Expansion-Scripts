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

class ExpansionSupplyCrateSetup
{
	string ClassName;
	vector Position;
	vector Orientation;
	ref array<ref ExpansionLoot> CrateLoot;
	
	void ExpansionSupplyCrateSetup(string className, vector pos, vector ori)
	{
		ClassName = className;
		Position = pos;
		Orientation = ori;
		CrateLoot = new array<ref ExpansionLoot>;
	}
	
	void AddLoot(string name, TStringArray attachments = NULL, float chance = 1, int quantityPercent = -1, array<ref ExpansionLootVariant> variants = NULL, int max = -1, int min = 0)
	{
		ExpansionLoot loot = new ExpansionLoot(name, attachments, chance, quantityPercent, variants , max, min);
		CrateLoot.Insert(loot);
	}

	array<ref ExpansionLoot> GetLoot()
	{
		return CrateLoot;
	}
};

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
	protected const float DEFAULT_ANOMALY_SQUARE_SIZE = 250;
	
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

	bool EnableSupplyCrates;
	ref array<ref ExpansionSupplyCrateSetup> SupplyCrateSpawns;

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
		
		SupplyCrateSpawns = new array<ref ExpansionSupplyCrateSetup>;
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
		
		EnableSupplyCrates = true;
		DefaultSupplyCrates();
	}
	
	protected void DefaultNamalskAnomalies()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		array<string> anomalies = {"Expansion_Anomaly_Singularity", "Expansion_Anomaly_Teleport"};
		array<ref ExpansionLoot> anomaly_loot = new array<ref ExpansionLoot>;
		
		ExpansionLoot loot_1 = ExpansionLoot("Apple", null, 1, 1, NULL, 1);
		anomaly_loot.Insert(loot_1);
		
		//! Dynamic spawns
		ExpansionAnomalyDynamic anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(1225.67, 2.00018, 11837.4), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(3774.44, 144.313, 8244.01), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4706.39, 85.6586, 8477.48), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4706.39, 85.6586, 8477.48), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5549.58, 43.0471, 9543.73), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(5744.57, 17.2946, 10770.0), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(4502.5, 19.5023, 10854), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(6708.11, 15.0, 11202.7), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(7047.19, 40.0, 5817.77), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(7263.92, 238.711, 7058.66), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(7312.8, 84.37, 7994.47), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
		DynamicAnomalies.Insert(anomaly);
		
		anomaly = new ExpansionAnomalyDynamic(anomalies, Vector(6273.98, 21.0682, 9351.2), DEFAULT_ANOMALY_SQUARE_SIZE, 5);
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

		waypoints.Insert(Vector(3601.81, 145.102, 6661.04));
		
		//! Spawn NAC AI Soldier Units at A3
		npcName = "NAC Soldier";
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
		
		//! Spawn NAC AI Soldier Units at A1 Bunker
		waypoints = new array<vector>;
		waypoints.Insert(Vector(3019.99, 15.2586, 8692.22));
		waypoints.Insert(Vector(3008.74, 15.3229, 8684.97));
		waypoints.Insert(Vector(2991.24, 15.3338, 8664.89));
		waypoints.Insert(Vector(2989.15, 15.3459, 8657.58));
		waypoints.Insert(Vector(2994.59, 15.3337, 8658.69));
		waypoints.Insert(Vector(3016.73, 15.3229, 8683.23));
		AISpawnPositions.Insert(new ExpansionAISpawnPosition(npcName, Vector(3019.99, 15.2586, 8692.22), Vector(105.0, 0, 0), waypoints, "Namalsk_NAC_Bunker", "NAC"));
	}
#endif
	
	protected void DefaultSupplyCrates()
	{
		TStringArray nacPlateCarrierAtt = {"dzn_platecarrierpouches_nac", "dzn_platecarrierholster_nac"};
		TStringArray nvgAtt = {"NVGHeadstrap"};
		
		ExpansionSupplyCrateSetup supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Brown", "4196.722656 2200.373779 8314.196289", "0.000000 -0.000000 -0.000000");
		supplyCrate.AddLoot("ExpansionAnomalyCoreProtectiveCase", NULL, 0.3, -1, NULL, -1);
		supplyCrate.AddLoot("Expansion_KeyCard_NA_Antenna", NULL, 0.3, -1, NULL, -1);
		SupplyCrateSpawns.Insert(supplyCrate);
		
		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Green", "4173.464844 2200.505615 8284.333984", "-107.999939 0.000000 -0.000000");
		supplyCrate.AddLoot("NVGoggles", nvgAtt, 0.2, -1, NULL, -1);
		supplyCrate.AddLoot("dzn_platecarriervest_nac", nacPlateCarrierAtt, 0.1, -1, NULL, -1);
		SupplyCrateSpawns.Insert(supplyCrate);
		
		supplyCrate = new ExpansionSupplyCrateSetup("Expansion_SupplyCrate_Orange", "4117.445801 2210.407715 8388.918945", "62.999973 -0.000000 -0.000000");
		supplyCrate.AddLoot("NVGoggles", nvgAtt, 0.2, -1, NULL, -1);
		supplyCrate.AddLoot("dzn_platecarriervest_nac", nacPlateCarrierAtt, 0.1, -1, NULL, -1);
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