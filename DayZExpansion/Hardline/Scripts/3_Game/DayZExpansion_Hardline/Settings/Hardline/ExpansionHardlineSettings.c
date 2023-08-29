/**
 * ExpansionHardlineSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/



/**@class		ExpansionHardlineSettingsV5 (legacy)
 **/
class ExpansionHardlineSettingsV5: ExpansionSettingBase
{
	//! Rank requirements
	int RankBambi;
	int RankSurvivor;

	//! Hero ranks
	int RankScout;
	int RankPathfinder;
	int RankHero;
	int RankSuperhero;
	int RankLegend;

	//! Bandit ranks
	int RankKleptomaniac;
	int RankBully;
	int RankBandit;
	int RankKiller;
	int RankMadman;

	//! Actions
	int HumanityBandageTarget;
	int HumanityDestroySafe;
	int HumanityDestroyBarbedWire;
	int HumanityDestroyBaseBuilding;
	int HumanityDestroyLock;

	//! Targets
	int HumanityOnKillInfected;
	int HumanityOnKillAI;
	int HumanityOnKillAnimal;
	int HumanityOnKillBambi;
	int HumanityOnKillHero;
	int HumanityOnKillBandit;

	//! Events
	int HumanityLossOnDeath;

	bool UseHumanity;
}

/**@class		ExpansionHardlineSettings
 * @brief		Hardline settings class
 **/
class ExpansionHardlineSettings: ExpansionSettingBase
{
	static const int VERSION = 8;
	
	int ReputationOnKillInfected;
	int ReputationOnKillPlayer;
	int ReputationOnKillAnimal;
	int ReputationOnKillAI;
	
	int ReputationLossOnDeath;
	
	int PoorItemRequirement;
	int CommonItemRequirement;
	int UncommonItemRequirement;
	int RareItemRequirement;
	int EpicItemRequirement;
	int LegendaryItemRequirement;
	int MythicItemRequirement;
	int ExoticItemRequirement;
	
	bool ShowHardlineHUD;
	bool UseReputation;
#ifdef EXPANSIONMODAI
	bool UseFactionReputation;
	bool EnableFactionPersistence;
#endif
	
	bool EnableItemRarity;
	bool UseItemRarityForMarketPurchase;
	bool UseItemRarityForMarketSell;
	
	int MaxReputation;

	//! Needs to be always last
	ref map<string, ExpansionHardlineItemRarity> ItemRarity;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionHardlineSettings()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this);
	#endif

		ItemRarity = new map<string, ExpansionHardlineItemRarity>;
	}

	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(ctx);
	#endif

		if (!ctx.Read(ShowHardlineHUD))
		{
			Error(ToString() + "::OnRecieve ShowHardlineHUD");
			return false;
		}

		if (!ctx.Read(UseReputation))
		{
			Error(ToString() + "::OnRecieve UseReputation");
			return false;
		}

		if (!ctx.Read(EnableItemRarity))
		{
			Error(ToString() + "::OnRecieve EnableItemRarity");
			return false;
		}
		
	#ifdef EXPANSIONMOD
		if (!ctx.Read(MaxReputation))
		{
			Error(ToString() + "::OnRecieve MaxReputation");
			return false;
		}
	#endif

		m_IsLoaded = true;

		return true;
	}

	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(ctx);
	#endif

		ctx.Write(ShowHardlineHUD);
		ctx.Write(UseReputation);
		ctx.Write(EnableItemRarity);
	#ifdef EXPANSIONMOD
		ctx.Write(MaxReputation);
	#endif
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
	#endif
		if ( !IsMissionHost() )
		{
			return 0;
		}

		auto rpc = ExpansionScriptRPC.Create();
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Hardline, true, identity );

		return 0;
	}

	private void CopyInternal( ExpansionHardlineSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(s);
#endif
		
		ReputationOnKillInfected = s.ReputationOnKillInfected;
		ReputationOnKillPlayer = s.ReputationOnKillPlayer;
		ReputationOnKillAnimal = s.ReputationOnKillAnimal;
		ReputationOnKillAI = s.ReputationOnKillAI;

		ReputationLossOnDeath = s.ReputationLossOnDeath;
		
		PoorItemRequirement = s.PoorItemRequirement;
		CommonItemRequirement = s.CommonItemRequirement;
		UncommonItemRequirement = s.UncommonItemRequirement;
		RareItemRequirement = s.RareItemRequirement;
		EpicItemRequirement = s.EpicItemRequirement;
		LegendaryItemRequirement = s.LegendaryItemRequirement;
		MythicItemRequirement = s.MythicItemRequirement;
		ExoticItemRequirement = s.ExoticItemRequirement;

		ShowHardlineHUD = s.ShowHardlineHUD;
		UseReputation = s.UseReputation;
		
		EnableItemRarity = s.EnableItemRarity;
		UseItemRarityForMarketPurchase = s.UseItemRarityForMarketPurchase;
		UseItemRarityForMarketSell = s.UseItemRarityForMarketSell;
		
		MaxReputation = s.MaxReputation;
		
		ItemRarity = s.ItemRarity;
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
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this);
	#endif

		m_IsLoaded = true;

		bool save;
		bool hardlineSettingsExist = FileExist(EXPANSION_HARDLINE_SETTINGS);

		if (hardlineSettingsExist)
		{
			EXPrint("[ExpansionHardlineSettings] Load existing setting file:" + EXPANSION_HARDLINE_SETTINGS);

			ExpansionHardlineSettings settingsDefault = new ExpansionHardlineSettings;
			settingsDefault.Defaults();

			ExpansionJsonFileParser<ExpansionHardlineSettings>.Load(EXPANSION_HARDLINE_SETTINGS, this);
			if (m_Version < VERSION)
			{
				EXPrint("[ExpansionHardlineSettings] Load - Converting v" + m_Version + " \"" + EXPANSION_HARDLINE_SETTINGS + "\" to v" + VERSION);

				if (m_Version < 6)
				{
					ExpansionHardlineSettingsV5 settingsV5;
					if (ExpansionJsonFileParser<ExpansionHardlineSettingsV5>.Load(EXPANSION_HARDLINE_SETTINGS, settingsV5))
					{
						ReputationOnKillInfected = settingsV5.HumanityOnKillInfected;
						ReputationOnKillPlayer = (settingsV5.HumanityOnKillBambi + settingsV5.HumanityOnKillBandit + settingsV5.HumanityOnKillHero) / 3;
						ReputationOnKillAnimal = settingsV5.HumanityOnKillAnimal;
						ReputationOnKillAI = settingsV5.HumanityOnKillAI;
						UseReputation = settingsV5.UseHumanity;
					}
				}

			#ifdef EXPANSIONMODAI
				if (m_Version < 7)
				{
					UseFactionReputation = settingsDefault.UseFactionReputation;
					EnableFactionPersistence = settingsDefault.EnableFactionPersistence;
				}
			#endif
				
				if (m_Version < 8)
					MaxReputation = settingsDefault.MaxReputation;

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionHardlineSettings] No existing setting file:" + EXPANSION_HARDLINE_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		//! Make sure item classnames are lowercase
		map<string, ExpansionHardlineItemRarity> itemRarity();
		itemRarity.Copy(ItemRarity);
		ItemRarity.Clear();
		foreach (string className, ExpansionHardlineItemRarity rarity: itemRarity)
		{
			AddItem(className, rarity);
		}

		if (save)
			Save();

		return hardlineSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionHardlineSettings] Saving settings");

		JsonFileLoader<ExpansionHardlineSettings>.JsonSaveFile( EXPANSION_HARDLINE_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_Hardline.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

		ReputationOnKillInfected = 5;
		ReputationOnKillPlayer = 10;
		ReputationOnKillAnimal = 5;
		ReputationOnKillAI = 10;
		
		ReputationLossOnDeath = 100;
		
		//! Item rarity rank requirements
		PoorItemRequirement = 0;
		CommonItemRequirement = 0;
		UncommonItemRequirement = 1000;
		RareItemRequirement = 2000;
		EpicItemRequirement = 3000;
		LegendaryItemRequirement = 4000;
		MythicItemRequirement = 5000;
		ExoticItemRequirement = 10000;

		ShowHardlineHUD = true;
		UseReputation = true;
	#ifdef EXPANSIONMODAI
		UseFactionReputation = false;
		EnableFactionPersistence = false;
	#endif
		EnableItemRarity = true;
		UseItemRarityForMarketPurchase = true;
		UseItemRarityForMarketSell = true;
		
		MaxReputation = 0;
		
		DefaultItemRarity();
	}

	// -----------------------------------------------------------
	// ExpansionHardlineModule DefaultItemRarity
	// -----------------------------------------------------------
	void DefaultItemRarity()
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

	#ifdef NAMALSK_SURVIVAL
		AddItem("dzn_module_card", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_module_lantia", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_module_surge", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_module_ext", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_module_ext2", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_lehs", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_lehs_helmet", ExpansionHardlineItemRarity.Exotic);
		
		AddItem("dzn_printer_filament_abs", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_printer_filament_tpc", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_printer_filament_nylon", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_blueprint_lehs", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_lehs_battery", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_lehs_o2tank", ExpansionHardlineItemRarity.Mythic);
		
		AddItem("dzn_athena_planning", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_athena_action1", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_athena_action2", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_netanyas_log", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_athena3_august", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_athena3_exp1", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_athena3_exp2", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_athena3_october", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_phoenix_log_1", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_phoenix_log_2", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_phoenix_log_3", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_phoenix_log_4", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_phoenix_log_5", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_lantia_december", ExpansionHardlineItemRarity.Mythic);
		AddItem("dzn_lantia_vega7", ExpansionHardlineItemRarity.Mythic);
		
		AddItem("dzn_apsi", ExpansionHardlineItemRarity.Legendary);
		AddItem("dzn_vinyl_bubson", ExpansionHardlineItemRarity.Legendary);

		AddItem("dzn_detector", ExpansionHardlineItemRarity.Epic);
		
		AddItem("dzn_tool_watch", ExpansionHardlineItemRarity.Uncommon);
	#endif

	#ifdef EXPANSIONMODMARKET
		AddItem("ExpansionGoldBar", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionGoldNugget", ExpansionHardlineItemRarity.Epic);
	#endif

		//! Ammo
		AddItem("Ammo_12gaPellets", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_12gaRubberSlug", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_12gaSlug", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_22", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_308Win", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_308WinTracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_357", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_380", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_45ACP", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_545x39", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_545x39Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_556x45", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_556x45Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_762x39", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_762x39Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_762x54", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_762x54Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_9x19", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_9x39", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_9x39AP", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_Flare", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Ammo_ArrowBolt", ExpansionHardlineItemRarity.Common);

		AddItem("ExpansionAmmoLAW", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionAmmoRPG", ExpansionHardlineItemRarity.Legendary);

		AddItem("Ammo_Expansion_M203_Smoke_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Smoke_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Smoke_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Smoke_Yellow", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Smoke_Purple", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_Expansion_M203_Smoke_Teargas", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Yellow", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Purple", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_Expansion_M203_HE", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_Expansion_46x30", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_Expansion_338", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Ammo_8mm", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Ammo boxes
		AddItem("AmmoBox_00buck_10rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_12gaSlug_10Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_12gaRubberSlug_10Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_22_50Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_308Win_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_308WinTracer_20Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_357_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_380_35rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_45ACP_25rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_545x39_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_545x39Tracer_20Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_556x45_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_556x45Tracer_20Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_762x39_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_762x39Tracer_20Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_762x54_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_762x54Tracer_20Rnd", ExpansionHardlineItemRarity.Rare);

		AddItem("AmmoBox_9x19_25rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_9x39_20rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_9x39AP_20rnd", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("AmmoBox_Expansion_46x30_25rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox_Expansion_338_15rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_AmmoBox_8mm_15rnd", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Assault Rifles
		AddItem("FAL", ExpansionHardlineItemRarity.Epic);
		AddItem("AKM", ExpansionHardlineItemRarity.Epic);
		AddItem("AK101", ExpansionHardlineItemRarity.Epic);
		AddItem("AK74", ExpansionHardlineItemRarity.Epic);
		AddItem("M4A1", ExpansionHardlineItemRarity.Epic);
		AddItem("M16A2", ExpansionHardlineItemRarity.Epic);
		AddItem("FAMAS", ExpansionHardlineItemRarity.Epic);

		AddItem("Aug", ExpansionHardlineItemRarity.Epic);
		AddItem("AugShort", ExpansionHardlineItemRarity.Epic);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_M16", ExpansionHardlineItemRarity.Epic);
		AddItem("Expansion_G36", ExpansionHardlineItemRarity.Epic);
	#endif

		//! Backbacks
		AddItem("ChildBag_Red", ExpansionHardlineItemRarity.Common);
		AddItem("ChildBag_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("ChildBag_Green", ExpansionHardlineItemRarity.Common);

		AddItem("DryBag_Orange", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DryBag_Yellow", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DryBag_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DryBag_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DryBag_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DryBag_Green", ExpansionHardlineItemRarity.Uncommon);

		AddItem("TaloonBag_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TaloonBag_Orange", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TaloonBag_Violet", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TaloonBag_Green", ExpansionHardlineItemRarity.Uncommon);

		AddItem("SmershBag", ExpansionHardlineItemRarity.Rare);

		AddItem("AssaultBag_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("AssaultBag_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("AssaultBag_Ttsko", ExpansionHardlineItemRarity.Rare);

		AddItem("HuntingBag", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMOD
		AddItem("TortillaBag", ExpansionHardlineItemRarity.Rare);
	#else
		AddItem("TortillaBag", ExpansionHardlineItemRarity.Rare);
	#endif

	#ifdef EXPANSIONMOD
		AddItem("CoyoteBag_Brown", ExpansionHardlineItemRarity.Rare);
		AddItem("CoyoteBag_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionCoyoteBlack", ExpansionHardlineItemRarity.Rare);
	#else
		AddItem("CoyoteBag_Brown", ExpansionHardlineItemRarity.Rare);
	#endif

		AddItem("MountainBag_Red", ExpansionHardlineItemRarity.Epic);
		AddItem("MountainBag_Blue", ExpansionHardlineItemRarity.Epic);
		AddItem("MountainBag_Orange", ExpansionHardlineItemRarity.Epic);
		AddItem("MountainBag_Green", ExpansionHardlineItemRarity.Epic);

		AddItem("AliceBag_Green", ExpansionHardlineItemRarity.Epic);
		AddItem("AliceBag_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("AliceBag_Camo", ExpansionHardlineItemRarity.Epic);

	#ifdef EXPANSIONMODVEHICLE
		//! Boats
		AddItem("ExpansionUtilityBoat", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionZodiacBoat", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionLHD", ExpansionHardlineItemRarity.Exotic);
	#endif

		//! Boots and Shoes
		AddItem("AthleticShoes_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("AthleticShoes_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("AthleticShoes_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("AthleticShoes_Black", ExpansionHardlineItemRarity.Common);
		AddItem("AthleticShoes_Green", ExpansionHardlineItemRarity.Common);

		AddItem("JoggingShoes_Black", ExpansionHardlineItemRarity.Common);
		AddItem("JoggingShoes_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("JoggingShoes_Red", ExpansionHardlineItemRarity.Common);
		AddItem("JoggingShoes_Violet", ExpansionHardlineItemRarity.Common);
		AddItem("JoggingShoes_White", ExpansionHardlineItemRarity.Common);

		AddItem("Sneakers_Green", ExpansionHardlineItemRarity.Common);
		AddItem("Sneakers_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Sneakers_White", ExpansionHardlineItemRarity.Common);
		AddItem("Sneakers_Black", ExpansionHardlineItemRarity.Common);
		AddItem("Sneakers_Gray", ExpansionHardlineItemRarity.Common);

		AddItem("Ballerinas_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Ballerinas_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Ballerinas_White", ExpansionHardlineItemRarity.Common);
		AddItem("Ballerinas_Yellow", ExpansionHardlineItemRarity.Common);

		AddItem("DressShoes_White", ExpansionHardlineItemRarity.Common);
		AddItem("DressShoes_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("DressShoes_Black", ExpansionHardlineItemRarity.Common);
		AddItem("DressShoes_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("DressShoes_Sunburst", ExpansionHardlineItemRarity.Common);

		AddItem("HikingBootsLow_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HikingBootsLow_Grey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HikingBootsLow_Beige", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HikingBootsLow_Black", ExpansionHardlineItemRarity.Uncommon);

		AddItem("WorkingBoots_Yellow", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingBoots_Grey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingBoots_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingBoots_Beige", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingBoots_Green", ExpansionHardlineItemRarity.Uncommon);

		AddItem("HikingBoots_Brown", ExpansionHardlineItemRarity.Rare);
		AddItem("HikingBoots_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CombatBoots_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("CombatBoots_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("CombatBoots_Brown", ExpansionHardlineItemRarity.Rare);
		AddItem("CombatBoots_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("CombatBoots_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("JungleBoots_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("JungleBoots_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("JungleBoots_Brown", ExpansionHardlineItemRarity.Rare);
		AddItem("JungleBoots_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("JungleBoots_Olive", ExpansionHardlineItemRarity.Rare);

		AddItem("Wellies_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Wellies_Brown", ExpansionHardlineItemRarity.Rare);
		AddItem("Wellies_Grey", ExpansionHardlineItemRarity.Rare);
		AddItem("Wellies_Green", ExpansionHardlineItemRarity.Rare);

		AddItem("TTSKOBoots", ExpansionHardlineItemRarity.Epic);

		AddItem("MilitaryBoots_Redpunk", ExpansionHardlineItemRarity.Epic);
		AddItem("MilitaryBoots_Bluerock", ExpansionHardlineItemRarity.Epic);
		AddItem("MilitaryBoots_Beige", ExpansionHardlineItemRarity.Epic);
		AddItem("MilitaryBoots_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("MilitaryBoots_Brown", ExpansionHardlineItemRarity.Epic);

		AddItem("NBCBootsGray", ExpansionHardlineItemRarity.Legendary);
		AddItem("NBCBootsYellow", ExpansionHardlineItemRarity.Legendary);

		//! Buttstocks
		AddItem("MP5k_StockBttstck", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Fal_OeBttstck", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Fal_FoldingBttstck", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Saiga_Bttstck", ExpansionHardlineItemRarity.Rare);

		AddItem("AKS74U_Bttstck", ExpansionHardlineItemRarity.Rare);

		AddItem("AK74_WoodBttstck", ExpansionHardlineItemRarity.Rare);

		AddItem("AK_PlasticBttstck", ExpansionHardlineItemRarity.Rare);
		AddItem("AK_WoodBttstck", ExpansionHardlineItemRarity.Rare);
		AddItem("AK_FoldingBttstck", ExpansionHardlineItemRarity.Rare);

		AddItem("M4_OEBttstck", ExpansionHardlineItemRarity.Epic);
		AddItem("M4_MPBttstck", ExpansionHardlineItemRarity.Epic);
		AddItem("M4_CQBBttstck", ExpansionHardlineItemRarity.Epic);

		AddItem("PP19_Bttstck", ExpansionHardlineItemRarity.Epic);

		AddItem("GhillieAtt_Tan", ExpansionHardlineItemRarity.Epic);
		AddItem("GhillieAtt_Woodland", ExpansionHardlineItemRarity.Epic);
		AddItem("GhillieAtt_Mossy", ExpansionHardlineItemRarity.Epic);

		AddItem("OffroadHatchback", ExpansionHardlineItemRarity.Epic);
		AddItem("Hatchback_02", ExpansionHardlineItemRarity.Epic);
		AddItem("Sedan_02", ExpansionHardlineItemRarity.Epic);
		AddItem("CivilianSedan", ExpansionHardlineItemRarity.Epic);
		AddItem("Truck_01_Covered", ExpansionHardlineItemRarity.Epic);
		//! Vanilla - Not released yet
		//AddItem("Truck_02", ExpansionHardlineItemRarity.Epic);
		//AddItem("Van_01", ExpansionHardlineItemRarity.Epic);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractor", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUAZ", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionUAZRoofless", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionUAZCargo", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionUAZCargoRoofless", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionBus", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionVodnik", ExpansionHardlineItemRarity.Legendary);
		AddItem("Expansion_Landrover", ExpansionHardlineItemRarity.Legendary);
	#endif

		//! Coats and Jackets
		AddItem("LabCoat", ExpansionHardlineItemRarity.Common);

		AddItem("TrackSuitJacket_Black", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitJacket_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitJacket_Green", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitJacket_LightBlue", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitJacket_Red", ExpansionHardlineItemRarity.Common);

		AddItem("DenimJacket", ExpansionHardlineItemRarity.Common);

		AddItem("WoolCoat_Red", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_RedCheck", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_BlueCheck", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_GreyCheck", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_BrownCheck", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_Black", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_BlackCheck", ExpansionHardlineItemRarity.Common);
		AddItem("WoolCoat_Green", ExpansionHardlineItemRarity.Common);

	#ifdef EXPANSIONMOD
		AddItem("RidersJacket_Black", ExpansionHardlineItemRarity.Common);
		AddItem("ExpansionRidersJacketBrown", ExpansionHardlineItemRarity.Common);
		AddItem("ExpansionRidersJacketDarkBlue", ExpansionHardlineItemRarity.Common);
	#else
		AddItem("RidersJacket_Black", ExpansionHardlineItemRarity.Common);
	#endif

		AddItem("FirefighterJacket_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("FirefighterJacket_Black", ExpansionHardlineItemRarity.Common);

		AddItem("JumpsuitJacket_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("JumpsuitJacket_Gray", ExpansionHardlineItemRarity.Common);
		AddItem("JumpsuitJacket_Green", ExpansionHardlineItemRarity.Common);
		AddItem("JumpsuitJacket_Red", ExpansionHardlineItemRarity.Common);

		AddItem("BomberJacket_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("BomberJacket_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("BomberJacket_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("BomberJacket_Maroon", ExpansionHardlineItemRarity.Common);
		AddItem("BomberJacket_SkyBlue", ExpansionHardlineItemRarity.Common);
		AddItem("BomberJacket_Black", ExpansionHardlineItemRarity.Common);
		AddItem("BomberJacket_Olive", ExpansionHardlineItemRarity.Common);

		AddItem("QuiltedJacket_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Red", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Orange", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Yellow", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Violet", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Black", ExpansionHardlineItemRarity.Common);
		AddItem("QuiltedJacket_Green", ExpansionHardlineItemRarity.Common);

		AddItem("PrisonUniformJacket", ExpansionHardlineItemRarity.Common);

		AddItem("PoliceJacketOrel", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PoliceJacket", ExpansionHardlineItemRarity.Uncommon);

		AddItem("ParamedicJacket_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ParamedicJacket_Crimson", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ParamedicJacket_Green", ExpansionHardlineItemRarity.Uncommon);

		AddItem("HikingJacket_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HikingJacket_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HikingJacket_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HikingJacket_Green", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Raincoat_Pink", ExpansionHardlineItemRarity.Rare);
		AddItem("Raincoat_Orange", ExpansionHardlineItemRarity.Rare);
		AddItem("Raincoat_Yellow", ExpansionHardlineItemRarity.Rare);
		AddItem("Raincoat_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Raincoat_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("Raincoat_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Raincoat_Green", ExpansionHardlineItemRarity.Rare);

		AddItem("TTsKOJacket_Camo", ExpansionHardlineItemRarity.Epic);

		AddItem("BDUJacket", ExpansionHardlineItemRarity.Epic);

		AddItem("HuntingJacket_Autumn", ExpansionHardlineItemRarity.Epic);
		AddItem("HuntingJacket_Brown", ExpansionHardlineItemRarity.Epic);
		AddItem("HuntingJacket_Spring", ExpansionHardlineItemRarity.Epic);
		AddItem("HuntingJacket_Summer", ExpansionHardlineItemRarity.Epic);
		AddItem("HuntingJacket_Winter", ExpansionHardlineItemRarity.Epic);

		AddItem("M65Jacket_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("M65Jacket_Khaki", ExpansionHardlineItemRarity.Epic);
		AddItem("M65Jacket_Tan", ExpansionHardlineItemRarity.Epic);
		AddItem("M65Jacket_Olive", ExpansionHardlineItemRarity.Epic);

		AddItem("GorkaEJacket_Summer", ExpansionHardlineItemRarity.Epic);
		AddItem("GorkaEJacket_Flat", ExpansionHardlineItemRarity.Epic);
		AddItem("GorkaEJacket_Autumn", ExpansionHardlineItemRarity.Epic);
		AddItem("GorkaEJacket_PautRev", ExpansionHardlineItemRarity.Epic);

		AddItem("USMCJacket_Desert", ExpansionHardlineItemRarity.Epic);
		AddItem("USMCJacket_Woodland", ExpansionHardlineItemRarity.Epic);

		AddItem("NBCJacketGray", ExpansionHardlineItemRarity.Legendary);
		AddItem("NBCJacketYellow", ExpansionHardlineItemRarity.Legendary);

		//! Containers
		AddItem("SmallProtectorCase", ExpansionHardlineItemRarity.Rare);
		AddItem("AmmoBox", ExpansionHardlineItemRarity.Epic);
		AddItem("BarrelHoles_Blue", ExpansionHardlineItemRarity.Epic);
		AddItem("BarrelHoles_Green", ExpansionHardlineItemRarity.Epic);
		AddItem("BarrelHoles_Red", ExpansionHardlineItemRarity.Epic);
		AddItem("BarrelHoles_Yellow", ExpansionHardlineItemRarity.Epic);
		AddItem("Barrel_Blue", ExpansionHardlineItemRarity.Epic);
		AddItem("Barrel_Green", ExpansionHardlineItemRarity.Epic);
		AddItem("Barrel_Red", ExpansionHardlineItemRarity.Epic);
		AddItem("Barrel_Yellow", ExpansionHardlineItemRarity.Epic);
		AddItem("SeaChest", ExpansionHardlineItemRarity.Legendary);
		AddItem("WoodenCrate", ExpansionHardlineItemRarity.Legendary);

	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionToolBox", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionSafeMini", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionSafeMedium", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionSafeLarge", ExpansionHardlineItemRarity.Legendary);
	#endif

		//! Crossbows
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Crossbow", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Electronics
		AddItem("PersonalRadio", ExpansionHardlineItemRarity.Rare);
		AddItem("Megaphone", ExpansionHardlineItemRarity.Rare);
		AddItem("ElectronicRepairKit", ExpansionHardlineItemRarity.Rare);
		AddItem("CableReel", ExpansionHardlineItemRarity.Rare);
		AddItem("BatteryCharger", ExpansionHardlineItemRarity.Rare);
		AddItem("BaseRadio", ExpansionHardlineItemRarity.Epic);
		AddItem("Rangefinder", ExpansionHardlineItemRarity.Epic);
		AddItem("NVGoggles", ExpansionHardlineItemRarity.Epic);

		AddItem("AlarmClock_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("KitchenTimer", ExpansionHardlineItemRarity.Uncommon);

		//! EXPLOSIVE GRANADES
		AddItem("RGD5Grenade", ExpansionHardlineItemRarity.Epic);
		AddItem("M67Grenade", ExpansionHardlineItemRarity.Epic);

		//! FLASHBANGS
		AddItem("FlashGrenade", ExpansionHardlineItemRarity.Rare);

		//! SMOKE GRANADES
	#ifdef EXPANSIONMODWEAPONS
		AddItem("M18SmokeGrenade_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_M18SmokeGrenade_Teargas", ExpansionHardlineItemRarity.Rare);
	#else
		AddItem("M18SmokeGrenade_Red", ExpansionHardlineItemRarity.Rare);
	#endif
		AddItem("M18SmokeGrenade_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("M18SmokeGrenade_Yellow", ExpansionHardlineItemRarity.Rare);
		AddItem("M18SmokeGrenade_Purple", ExpansionHardlineItemRarity.Rare);
		AddItem("M18SmokeGrenade_White", ExpansionHardlineItemRarity.Rare);
		AddItem("RDG2SmokeGrenade_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("RDG2SmokeGrenade_White", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODMISSIONS
		//! Airdrop smoke
		AddItem("ExpansionSupplySignal", ExpansionHardlineItemRarity.Exotic);
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
		//! EXPLOSIVE CHARGES
		AddItem("ExpansionSatchel", ExpansionHardlineItemRarity.Legendary);
	#endif

		//! Ghillies
		AddItem("GhillieHood_Tan", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieHood_Woodland", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieHood_Mossy", ExpansionHardlineItemRarity.Legendary);

		AddItem("GhillieBushrag_Tan", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieBushrag_Woodland", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieBushrag_Mossy", ExpansionHardlineItemRarity.Legendary);

		AddItem("GhillieTop_Tan", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieTop_Woodland", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieTop_Mossy", ExpansionHardlineItemRarity.Legendary);

		AddItem("GhillieSuit_Tan", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieSuit_Woodland", ExpansionHardlineItemRarity.Legendary);
		AddItem("GhillieSuit_Mossy", ExpansionHardlineItemRarity.Legendary);

		//! Gloves
		AddItem("SurgicalGloves_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("SurgicalGloves_LightBlue", ExpansionHardlineItemRarity.Common);
		AddItem("SurgicalGloves_Green", ExpansionHardlineItemRarity.Common);
		AddItem("SurgicalGloves_White", ExpansionHardlineItemRarity.Common);

		AddItem("WorkingGloves_Yellow", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingGloves_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingGloves_Beige", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WorkingGloves_Brown", ExpansionHardlineItemRarity.Uncommon);

	#ifdef EXPANSIONMOD
		AddItem("TacticalGloves_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionTacticalGlovesDesert", ExpansionHardlineItemRarity.Rare);
	#else
		AddItem("TacticalGloves_Black", ExpansionHardlineItemRarity.Rare);
	#endif
		AddItem("TacticalGloves_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("TacticalGloves_Green", ExpansionHardlineItemRarity.Rare);

		AddItem("OMNOGloves_Gray", ExpansionHardlineItemRarity.Epic);
		AddItem("OMNOGloves_Brown", ExpansionHardlineItemRarity.Epic);

		AddItem("NBCGlovesGray", ExpansionHardlineItemRarity.Legendary);
		AddItem("NBCGlovesYellow", ExpansionHardlineItemRarity.Legendary);

		//! Handguards
		AddItem("MP5_PlasticHndgrd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MP5_RailHndgrd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("AK74_Hndgrd", ExpansionHardlineItemRarity.Rare);

		AddItem("AK_WoodHndgrd", ExpansionHardlineItemRarity.Epic);
		AddItem("AK_RailHndgrd", ExpansionHardlineItemRarity.Epic);
		AddItem("AK_PlasticHndgrd", ExpansionHardlineItemRarity.Epic);

		AddItem("M4_PlasticHndgrd", ExpansionHardlineItemRarity.Rare);
		AddItem("M4_RISHndgrd", ExpansionHardlineItemRarity.Epic);
		AddItem("M4_MPHndgrd", ExpansionHardlineItemRarity.Epic);

		//! Hats
		AddItem("MedicalScrubsHat_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("MedicalScrubsHat_White", ExpansionHardlineItemRarity.Common);
		AddItem("MedicalScrubsHat_Green", ExpansionHardlineItemRarity.Common);

	#ifdef EXPANSIONMOD
		AddItem("CowboyHat_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCowboyHatGator", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCowboyHatSnake", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCowboyHatRattleSnake", ExpansionHardlineItemRarity.Uncommon);
	#else
		AddItem("CowboyHat_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CowboyHat_black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CowboyHat_darkBrown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CowboyHat_green", ExpansionHardlineItemRarity.Uncommon);
	#endif

		AddItem("MilitaryBeret_ChDKZ", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MilitaryBeret_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MilitaryBeret_UN", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MilitaryBeret_CDF", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MilitaryBeret_NZ", ExpansionHardlineItemRarity.Uncommon);

		AddItem("BeanieHat_Pink", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Beige", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Grey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BeanieHat_Green", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ushanka_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("Ushanka_Blue", ExpansionHardlineItemRarity.Epic);
		AddItem("Ushanka_Green", ExpansionHardlineItemRarity.Epic);

		AddItem("BoonieHat_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_NavyBlue", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Orange", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Tan", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Olive", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_DPM", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Dubok", ExpansionHardlineItemRarity.Rare);
		AddItem("BoonieHat_Flecktran", ExpansionHardlineItemRarity.Rare);

		AddItem("OfficerHat", ExpansionHardlineItemRarity.Epic);

		AddItem("NBCHoodGray", ExpansionHardlineItemRarity.Legendary);
		AddItem("NBCHoodYellow", ExpansionHardlineItemRarity.Legendary);

		//! Helis
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionGyrocopter", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionUh1h", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionMerlin", ExpansionHardlineItemRarity.Legendary);
	#endif

		//! Helmets
		AddItem("ConstructionHelmet_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("ConstructionHelmet_Orange", ExpansionHardlineItemRarity.Rare);
		AddItem("ConstructionHelmet_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("ConstructionHelmet_White", ExpansionHardlineItemRarity.Rare);
		AddItem("ConstructionHelmet_Yellow", ExpansionHardlineItemRarity.Rare);
		AddItem("ConstructionHelmet_Lime", ExpansionHardlineItemRarity.Rare);

		AddItem("SkateHelmet_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("SkateHelmet_Gray", ExpansionHardlineItemRarity.Rare);
		AddItem("SkateHelmet_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("SkateHelmet_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("SkateHelmet_Green", ExpansionHardlineItemRarity.Rare);

		AddItem("HockeyHelmet_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("HockeyHelmet_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("HockeyHelmet_White", ExpansionHardlineItemRarity.Rare);
		AddItem("HockeyHelmet_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("DirtBikeHelmet_Mouthguard", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Visor", ExpansionHardlineItemRarity.Rare);

		AddItem("DirtBikeHelmet_Chernarus", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Police", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("DirtBikeHelmet_Khaki", ExpansionHardlineItemRarity.Rare);

		AddItem("MotoHelmet_Lime", ExpansionHardlineItemRarity.Rare);
		AddItem("MotoHelmet_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("MotoHelmet_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("MotoHelmet_White", ExpansionHardlineItemRarity.Rare);
		AddItem("MotoHelmet_Grey", ExpansionHardlineItemRarity.Rare);
		AddItem("MotoHelmet_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("MotoHelmet_Green", ExpansionHardlineItemRarity.Rare);

		AddItem("DarkMotoHelmet_Grey", ExpansionHardlineItemRarity.Epic);
		AddItem("DarkMotoHelmet_Lime", ExpansionHardlineItemRarity.Epic);
		AddItem("DarkMotoHelmet_Blue", ExpansionHardlineItemRarity.Epic);
		AddItem("DarkMotoHelmet_Red", ExpansionHardlineItemRarity.Epic);
		AddItem("DarkMotoHelmet_White", ExpansionHardlineItemRarity.Epic);
		AddItem("DarkMotoHelmet_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("DarkMotoHelmet_Green", ExpansionHardlineItemRarity.Epic);

		AddItem("TankerHelmet", ExpansionHardlineItemRarity.Epic);

		AddItem("GreatHelm", ExpansionHardlineItemRarity.Epic);

		AddItem("ZSh3PilotHelmet", ExpansionHardlineItemRarity.Epic);
		AddItem("ZSh3PilotHelmet_Green", ExpansionHardlineItemRarity.Epic);
		AddItem("ZSh3PilotHelmet_Black", ExpansionHardlineItemRarity.Epic);

		AddItem("FirefightersHelmet_Red", ExpansionHardlineItemRarity.Epic);
		AddItem("FirefightersHelmet_White", ExpansionHardlineItemRarity.Epic);
		AddItem("FirefightersHelmet_Yellow", ExpansionHardlineItemRarity.Epic);

	#ifdef EXPANSIONMOD
		AddItem("Mich2001Helmet", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionMich2001Desert", ExpansionHardlineItemRarity.Epic);
	#else
		AddItem("Mich2001Helmet", ExpansionHardlineItemRarity.Epic);
	#endif

		AddItem("GorkaHelmetVisor", ExpansionHardlineItemRarity.Epic);
		AddItem("GorkaHelmet", ExpansionHardlineItemRarity.Epic);
		AddItem("GorkaHelmet_Black", ExpansionHardlineItemRarity.Epic);

		AddItem("Ssh68Helmet", ExpansionHardlineItemRarity.Epic);

		AddItem("BallisticHelmet_UN", ExpansionHardlineItemRarity.Epic);
		AddItem("BallisticHelmet_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("BallisticHelmet_Green", ExpansionHardlineItemRarity.Epic);

		// Holsters/Pouches
		AddItem("ChestHolster", ExpansionHardlineItemRarity.Rare);
		AddItem("PlateCarrierHolster", ExpansionHardlineItemRarity.Rare);
		AddItem("PlateCarrierPouches", ExpansionHardlineItemRarity.Rare);
		AddItem("NylonKnifeSheath", ExpansionHardlineItemRarity.Rare);

		//! Kits
		AddItem("SewingKit", ExpansionHardlineItemRarity.Rare);
		AddItem("LeatherSewingKit", ExpansionHardlineItemRarity.Rare);
		AddItem("WeaponCleaningKit", ExpansionHardlineItemRarity.Rare);

		//! Knifes
		AddItem("KitchenKnife", ExpansionHardlineItemRarity.Rare);
		AddItem("SteakKnife", ExpansionHardlineItemRarity.Common);
		AddItem("HuntingKnife", ExpansionHardlineItemRarity.Rare);
		AddItem("CombatKnife", ExpansionHardlineItemRarity.Epic);
		AddItem("KukriKnife", ExpansionHardlineItemRarity.Rare);
		AddItem("FangeKnife", ExpansionHardlineItemRarity.Common);
		AddItem("Machete", ExpansionHardlineItemRarity.Rare);
		AddItem("CrudeMachete", ExpansionHardlineItemRarity.Rare);
		AddItem("OrientalMachete", ExpansionHardlineItemRarity.Rare);
		AddItem("Cleaver", ExpansionHardlineItemRarity.Rare);

		//! Launchers
		AddItem("Flaregun", ExpansionHardlineItemRarity.Rare);
		AddItem("M79", ExpansionHardlineItemRarity.Legendary);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("ExpansionLAW", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionRPG7", ExpansionHardlineItemRarity.Legendary);
	#endif

		//! Lights
		AddItem("Chemlight_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Chemlight_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Chemlight_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Chemlight_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Chemlight_Yellow", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Roadflare", ExpansionHardlineItemRarity.Rare);
		AddItem("Matchbox", ExpansionHardlineItemRarity.Rare);
		AddItem("PetrolLighter", ExpansionHardlineItemRarity.Rare);
		AddItem("Flashlight", ExpansionHardlineItemRarity.Rare);
		AddItem("XmasLights", ExpansionHardlineItemRarity.Rare);
		AddItem("PortableGasLamp", ExpansionHardlineItemRarity.Rare);
		AddItem("PortableGasStove", ExpansionHardlineItemRarity.Rare);
		AddItem("Headtorch_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Headtorch_Grey", ExpansionHardlineItemRarity.Rare);
		AddItem("Spotlight", ExpansionHardlineItemRarity.Rare);

		AddItem("UniversalLight", ExpansionHardlineItemRarity.Rare);
		AddItem("TLRLight", ExpansionHardlineItemRarity.Rare);

		//! Liquids
		AddItem("EngineOil", ExpansionHardlineItemRarity.Epic);
		AddItem("CanisterGasoline", ExpansionHardlineItemRarity.Epic);

		//! Locks
		AddItem("CombinationLock", ExpansionHardlineItemRarity.Rare);
		AddItem("CombinationLock4", ExpansionHardlineItemRarity.Epic);
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionCodeLock", ExpansionHardlineItemRarity.Epic);
	#endif

		AddItem("Mag_IJ70_8Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_CZ75_15Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_Glock_15Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_MKII_10Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_1911_7Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_FNX45_15Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_Deagle_9Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_CZ527_5rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_CZ61_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_PP19_64Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_UMP_25Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_MP5_15Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_MP5_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_FAL_20Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_Saiga_5Rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_Saiga_8Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Saiga_Drum20Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_AKM_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_AKM_Palm30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_AKM_Drum75Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_AK101_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_AK74_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_AK74_45Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_STANAG_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_STANAGCoupled_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_STANAG_60Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_CMAG_10Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_CMAG_20Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_CMAG_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_CMAG_40Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_VSS_10Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_VAL_20Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Ruger1022_15Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Ruger1022_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_SVD_10Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Scout_5Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_FAMAS_25Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Aug_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_P1_8Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_CZ550_4rnd", ExpansionHardlineItemRarity.Rare);
		AddItem("Mag_CZ550_10rnd", ExpansionHardlineItemRarity.Epic);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Mag_Expansion_Taser", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_Expansion_M9_15Rnd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mag_Expansion_Kedr_20Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_MP7_40Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_Vityaz_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_MPX_50Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_M14_10Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_M14_20Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_G36_30Rnd", ExpansionHardlineItemRarity.Epic);
		AddItem("Mag_Expansion_AWM_5Rnd", ExpansionHardlineItemRarity.Legendary);
	#endif

		//! Masks
		AddItem("SurgicalMask", ExpansionHardlineItemRarity.Common);
		AddItem("NioshFaceMask", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HockeyMask", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BalaclavaMask_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("BalaclavaMask_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("BalaclavaMask_Pink", ExpansionHardlineItemRarity.Rare);
		AddItem("BalaclavaMask_White", ExpansionHardlineItemRarity.Rare);
		AddItem("BalaclavaMask_Blackskull", ExpansionHardlineItemRarity.Rare);
		AddItem("BalaclavaMask_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("BalaclavaMask_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("Balaclava3Holes_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("Balaclava3Holes_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("Balaclava3Holes_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Balaclava3Holes_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("WeldingMask", ExpansionHardlineItemRarity.Rare);
		AddItem("GasMask", ExpansionHardlineItemRarity.Epic);
		AddItem("GP5GasMask", ExpansionHardlineItemRarity.Epic);
		AddItem("AirborneMask", ExpansionHardlineItemRarity.Epic);

		//! Meat
		AddItem("BearSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("GoatSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BoarSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PigSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DeerSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WolfSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CowSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SheepSteakMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ChickenBreastMeat", ExpansionHardlineItemRarity.Uncommon);
	#ifdef EXPANSIONMOD
		AddItem("RabbitLegMeat", ExpansionHardlineItemRarity.Uncommon);
	#endif

		//! Medical
		AddItem("CharcoalTablets", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BandageDressing", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DisinfectantAlcohol", ExpansionHardlineItemRarity.Rare);
		AddItem("PurificationTablets", ExpansionHardlineItemRarity.Rare);
		AddItem("BloodTestKit", ExpansionHardlineItemRarity.Rare);
		AddItem("Thermometer", ExpansionHardlineItemRarity.Uncommon);
		AddItem("VitaminBottle", ExpansionHardlineItemRarity.Rare);
		AddItem("DisinfectantSpray", ExpansionHardlineItemRarity.Rare);
		AddItem("TetracyclineAntibiotics", ExpansionHardlineItemRarity.Rare);
		AddItem("PainkillerTablets", ExpansionHardlineItemRarity.Rare);
		AddItem("StartKitIV", ExpansionHardlineItemRarity.Rare);
		AddItem("Heatpack", ExpansionHardlineItemRarity.Rare);
		AddItem("SalineBag", ExpansionHardlineItemRarity.Epic);
		AddItem("BloodBagEmpty", ExpansionHardlineItemRarity.Epic);
		AddItem("FirstAidKit", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Epinephrine", ExpansionHardlineItemRarity.Epic);
		AddItem("Morphine", ExpansionHardlineItemRarity.Epic);
		AddItem("IodineTincture", ExpansionHardlineItemRarity.Epic);
		AddItem("AntiChemInjector", ExpansionHardlineItemRarity.Epic);

		//! Melee
		AddItem("BrassKnuckles_Dull", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BrassKnuckles_Shiny", ExpansionHardlineItemRarity.Uncommon);
		AddItem("StunBaton", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Broom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Broom_Birch", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pipe", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CattleProd", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BaseballBat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("NailedBaseballBat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BarbedBaseballBat", ExpansionHardlineItemRarity.Uncommon);

		//! Compensators
		AddItem("Mosin_Compensator", ExpansionHardlineItemRarity.Rare);
		AddItem("MP5_Compensator", ExpansionHardlineItemRarity.Rare);

		//! Supressors
		AddItem("M4_Suppressor", ExpansionHardlineItemRarity.Epic);
		AddItem("AK_Suppressor", ExpansionHardlineItemRarity.Epic);
		AddItem("PistolSuppressor", ExpansionHardlineItemRarity.Rare);
	}

	void AddItem(string type, ExpansionHardlineItemRarity rarity)
	{
		type.ToLower();
		ItemRarity.Insert(type, rarity);
	}

	override string SettingName()
	{
		return "Hardline Settings";
	}

	ExpansionHardlineItemRarity GetItemRarityByType(string type)
	{
		type.ToLower();

		ExpansionHardlineItemRarity rarity;
		if (ItemRarity.Find(type, rarity))
			return rarity;

		return ExpansionHardlineItemRarity.NONE;
	}
	
	int GetReputationForRarity(ExpansionHardlineItemRarity rarity)
	{
		string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
		int rarityReq;
		EnScript.GetClassVar(this, rarityName + "ItemRequirement", 0, rarityReq);
		return rarityReq;
	}
	
	int GetLimitByReputation(int reputation, int reputationToUnlock, int minVal, int increment)
	{
		Print(ToString() + "::GetLimitByReputation - Start");
		Print(ToString() + "::GetLimitByReputation - Reputation: " + reputation + " | Reputation to unlock: " + reputationToUnlock + " | Min value: " + minVal + " | Increment: " + increment);
		int playerLevel = reputation / 1000;
		int limit;
		if (reputation > reputationToUnlock)
			limit = minVal + playerLevel * increment;
		else
			limit = minVal;
		Print(ToString() + "::GetLimitByReputation - Level: " + playerLevel + " | Limit: " + limit);
		return limit;
	}
};