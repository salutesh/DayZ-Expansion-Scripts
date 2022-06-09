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

/**@class		ExpansionHardlineSettingsBase
 * @brief		Hardline settings base class
 **/
class ExpansionHardlineSettingsBase: ExpansionSettingBase
{
	// ------------------------------------------------------------
	void ExpansionHardlineSettingsBase()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionHardlineSettingsBase");
	#endif
	}
}

/**@class		ExpansionHardlineSettings
 * @brief		Hardline settings class
 **/
class ExpansionHardlineSettings: ExpansionHardlineSettingsBase
{
	static const int VERSION = 0;
	
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
	
	//! Targets
	int HumanityOnKillInfected;
	int HumanityOnKillAI;
	int HumanityOnKillBambi;
	int HumanityOnKillHero;
	int HumanityOnKillBandit;
	
	//! Events
	int HumanityLossOnDeath;
	
	int PoorItemRequirement;
	int CommonItemRequirement;
	int UncommonItemRequirement;
	int RareItemRequirement;
	int EpicItemRequirement;
	int LegendaryItemRequirement;
	int MythicItemRequirement;
	int ExoticItemRequirement;
	
	autoptr array<ref ExpansionHardlineItemData> HardlineItemDatas;

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionSpawnSettings()
	{
		HardlineItemDatas = new array<ref ExpansionHardlineItemData>
	}

	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
	#endif

		if (!HardlineItemDatas)
			HardlineItemDatas = new array<ref ExpansionHardlineItemData>;
				
		if (!ctx.Read(RankBambi))
		{
			Error(ToString() + "::OnRecieve RankBambi");
			return false;
		}
		
		if (!ctx.Read(RankSurvivor))
		{
			Error(ToString() + "::OnRecieve RankSurvivor");
			return false;
		}
		
		if (!ctx.Read(RankScout))
		{
			Error(ToString() + "::OnRecieve RankScout");
			return false;
		}
		
		if (!ctx.Read(RankPathfinder))
		{
			Error(ToString() + "::OnRecieve RankPathfinder");
			return false;
		}
		
		if (!ctx.Read(RankHero))
		{
			Error(ToString() + "::OnRecieve RankHero");
			return false;
		}
		
		if (!ctx.Read(RankSuperhero))
		{
			Error(ToString() + "::OnRecieve RankSuperhero");
			return false;
		}
		
		if (!ctx.Read(RankLegend))
		{
			Error(ToString() + "::OnRecieve RankLegend");
			return false;
		}
		
		if (!ctx.Read(RankKleptomaniac))
		{
			Error(ToString() + "::OnRecieve RankKleptomaniac");
			return false;
		}
		
		if (!ctx.Read(RankBully))
		{
			Error(ToString() + "::OnRecieve RankBully");
			return false;
		}
		
		if (!ctx.Read(RankBandit))
		{
			Error(ToString() + "::OnRecieve RankBandit");
			return false;
		}
		
		if (!ctx.Read(RankKiller))
		{
			Error(ToString() + "::OnRecieve RankKiller");
			return false;
		}
		
		if (!ctx.Read(RankMadman))
		{
			Error(ToString() + "::OnRecieve RankMadman");
			return false;
		}
		
		if (!ctx.Read(HumanityBandageTarget))
		{
			Error(ToString() + "::OnRecieve HumanityBandageTarget");
			return false;
		}
		
		if (!ctx.Read(HumanityOnKillInfected))
		{
			Error(ToString() + "::OnRecieve HumanityOnKillInfected");
			return false;
		}
		
		if (!ctx.Read(HumanityOnKillAI))
		{
			Error(ToString() + "::OnRecieve HumanityOnKillAI");
			return false;
		}
		
		if (!ctx.Read(HumanityOnKillBambi))
		{
			Error(ToString() + "::OnRecieve HumanityOnKillBambi");
			return false;
		}
		
		if (!ctx.Read(HumanityOnKillHero))
		{
			Error(ToString() + "::OnRecieve HumanityOnKillHero");
			return false;
		}
		
		if (!ctx.Read(HumanityOnKillBandit))
		{
			Error(ToString() + "::OnRecieve HumanityOnKillBandit");
			return false;
		}
		
		if (!ctx.Read(HumanityLossOnDeath))
		{
			Error(ToString() + "::OnRecieve HumanityLossOnDeath");
			return false;
		}
		
		if (!ctx.Read(PoorItemRequirement))
		{
			Error(ToString() + "::OnRecieve HumanityLossOnDeath");
			return false;
		}
		
		if (!ctx.Read(CommonItemRequirement))
		{
			Error(ToString() + "::OnRecieve CommonItemRequirement");
			return false;
		}
		
		if (!ctx.Read(UncommonItemRequirement))
		{
			Error(ToString() + "::OnRecieve UncommonItemRequirement");
			return false;
		}
		
		if (!ctx.Read(RareItemRequirement))
		{
			Error(ToString() + "::OnRecieve RareItemRequirement");
			return false;
		}
		
		if (!ctx.Read(EpicItemRequirement))
		{
			Error(ToString() + "::OnRecieve EpicItemRequirement");
			return false;
		}
		
		if (!ctx.Read(LegendaryItemRequirement))
		{
			Error(ToString() + "::OnRecieve LegendaryItemRequirement");
			return false;
		}
		
		if (!ctx.Read(MythicItemRequirement))
		{
			Error(ToString() + "::OnRecieve MythicItemRequirement");
			return false;
		}
		
		if (!ctx.Read(ExoticItemRequirement))
		{
			Error(ToString() + "::OnRecieve ExoticItemRequirement");
			return false;
		}
		
		int itemDataCount;
		if ( !ctx.Read( itemDataCount ) )
		{
			Error(ToString() + "::OnRecieve Hardline item data count");
			return false;
		}
		
		for (int i = 0; i < itemDataCount; i++)
		{			
			ExpansionHardlineItemData itemData = new ExpansionHardlineItemData();			
			if ( !itemData.OnRecieve( ctx ) )
			{
				Error(ToString() + "::OnRecieve Hardline item data");
				return false;
			}
			
			HardlineItemDatas.Insert(itemData);
		}
		
		m_IsLoaded = true;

		return true;
	}

	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
	#endif

		
		//! Rank requirements
		ctx.Write(RankBambi);
		ctx.Write(RankSurvivor);
		
		//! Hero ranks
		ctx.Write(RankScout);
		ctx.Write(RankPathfinder);
		ctx.Write(RankHero);
		ctx.Write(RankSuperhero);
		ctx.Write(RankLegend);
		
		//! Bandit ranks
		ctx.Write(RankKleptomaniac);
		ctx.Write(RankBully);
		ctx.Write(RankBandit);
		ctx.Write(RankKiller);
		ctx.Write(RankMadman);
		
		ctx.Write(HumanityBandageTarget);
		
		ctx.Write(HumanityOnKillInfected);
		ctx.Write(HumanityOnKillAI);
		ctx.Write(HumanityOnKillBambi);
		ctx.Write(HumanityOnKillHero);
		ctx.Write(HumanityOnKillBandit);
		ctx.Write(HumanityLossOnDeath);
		
		ctx.Write(PoorItemRequirement);
		ctx.Write(CommonItemRequirement);
		ctx.Write(UncommonItemRequirement);
		ctx.Write(RareItemRequirement);
		ctx.Write(EpicItemRequirement);
		ctx.Write(LegendaryItemRequirement);
		ctx.Write(MythicItemRequirement);
		ctx.Write(ExoticItemRequirement);
		
		int itemDataCount = HardlineItemDatas.Count();
		ctx.Write( itemDataCount );
		
		for (int i = 0; i < itemDataCount; i++)
		{
			ExpansionHardlineItemData itemData = HardlineItemDatas.Get(i);
			if (!itemData)
				continue;
			
			itemData.OnSend( ctx );
		}
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

		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Hardline, true, identity );

		return 0;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionHardlineSettingsBase s)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

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
		bool hardlineSettingsExist = FileExist(EXPANSION_HARDLINE_SETTINGS);

		if (hardlineSettingsExist)
		{
			EXPrint("[ExpansionHardlineSettings] Load existing setting file:" + EXPANSION_HARDLINE_SETTINGS);

			ExpansionHardlineSettings settingsDefault = new ExpansionHardlineSettings;
			settingsDefault.Defaults();

			ExpansionHardlineSettingsBase settingsBase;
			JsonFileLoader<ExpansionHardlineSettingsBase>.JsonLoadFile(EXPANSION_HARDLINE_SETTINGS, settingsBase);
			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionHardlineSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_HARDLINE_SETTINGS + "\" to v" + VERSION);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionHardlineSettings>.JsonLoadFile(EXPANSION_HARDLINE_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionHardlineSettings] No existing setting file:" + EXPANSION_HARDLINE_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
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
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_Hardline.Invoke();
	}
	
	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

		//! Rank requirements
		RankBambi = 0;
		RankSurvivor = 0;
		
		//! Hero ranks
		RankScout = 1000;
		RankPathfinder = 2000;
		RankHero = 3000;
		RankSuperhero = 4000;
		RankLegend = 5000;
		
		//! Bandit ranks
		RankKleptomaniac = -1000;
		RankBully = -2000;
		RankBandit = -3000;
		RankKiller = -4000;
		RankMadman = -5000;
		
		//! Actions
		HumanityBandageTarget = 10;
		
		//! Targets
		HumanityOnKillInfected = 10;
		HumanityOnKillAI = 10;
		HumanityOnKillBambi = 10;
		HumanityOnKillHero = 10;
		HumanityOnKillBandit = 10;
		
		//! Events
		HumanityLossOnDeath = 1000;
		
		//! Item rarity rank requirements
		PoorItemRequirement = 0;
		CommonItemRequirement = 0;
		UncommonItemRequirement = 1000;
		RareItemRequirement = 2000;
		EpicItemRequirement = 3000;
		LegendaryItemRequirement = 4000;
		MythicItemRequirement = 5000;
		ExoticItemRequirement = 10000;
		
		DefaultHardlineItemData();
	}

	// -----------------------------------------------------------
	// ExpansionHardlineModule LoadItemHardlineData
	// Server
	// -----------------------------------------------------------
	void DefaultHardlineItemData()
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		if (!HardlineItemDatas)
			HardlineItemDatas = new array<ref ExpansionHardlineItemData>;

		string worldName;
		GetGame().GetWorldName(worldName);
		worldName.ToLower();
		
	#ifdef NAMALSK_SURVIVAL
		AddItem("dzn_module_card", ExpansionHardlineItemRarity.MYTHIC);
		AddItem("dzn_module_lantia", ExpansionHardlineItemRarity.MYTHIC);
		AddItem("dzn_module_surge", ExpansionHardlineItemRarity.MYTHIC);
		
		AddItem("ER7_Gauss_Magazine", ExpansionHardlineItemRarity.EXOTIC);
		AddItem("Ammo_ER7RFW", ExpansionHardlineItemRarity.EXOTIC);
		AddItem("ER7_Scope", ExpansionHardlineItemRarity.EXOTIC);
		AddItem("ER7_Gauss", ExpansionHardlineItemRarity.EXOTIC);
		AddItem("ER7_Gauss_Battery", ExpansionHardlineItemRarity.EXOTIC);
	#endif
	
	#ifdef EXPANSIONMODMARKET
		AddItem("ExpansionGoldBar", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionGoldBar", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionGoldNugget", ExpansionHardlineItemRarity.EPIC);
	#endif
		
		//! Ammo
		AddItem("Ammo_12gaPellets", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_12gaRubberSlug", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_12gaSlug", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_22", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_308Win", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_308WinTracer", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_357", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_380", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_45ACP", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_545x39", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_545x39Tracer", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_556x45", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_556x45Tracer", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_762x39", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_762x39Tracer", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_762x54", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_762x54Tracer", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_9x19", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_9x39", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_9x39AP", ExpansionHardlineItemRarity.RARE);

		AddItem("Ammo_Flare", ExpansionHardlineItemRarity.RARE);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Ammo_ArrowBolt", ExpansionHardlineItemRarity.COMMON);

		AddItem("ExpansionAmmoLAW", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("ExpansionAmmoRPG", ExpansionHardlineItemRarity.LEGENDARY);
		
		AddItem("Ammo_Expansion_M203_Smoke_White", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Smoke_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Smoke_Green", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Smoke_Yellow", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Smoke_Purple", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("Ammo_Expansion_M203_Smoke_Teargas", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Green", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Yellow", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Purple", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("Ammo_Expansion_M203_HE", ExpansionHardlineItemRarity.RARE);
		
		AddItem("Ammo_Expansion_46x30", ExpansionHardlineItemRarity.RARE);
		AddItem("Ammo_Expansion_338", ExpansionHardlineItemRarity.RARE);
		AddItem("Expansion_Ammo_8mm", ExpansionHardlineItemRarity.RARE);
	#endif
		
		//! Ammo boxes
		AddItem("AmmoBox_00buck_10rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_12gaSlug_10Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_12gaRubberSlug_10Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_22_50Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_308Win_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_308WinTracer_20Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_357_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_380_35rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_45ACP_25rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_545x39_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_545x39Tracer_20Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_556x45_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_556x45Tracer_20Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_762x39_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_762x39Tracer_20Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_762x54_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_762x54Tracer_20Rnd", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AmmoBox_9x19_25rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_9x39_20rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_9x39AP_20rnd", ExpansionHardlineItemRarity.RARE);
	
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("AmmoBox_Expansion_46x30_25rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox_Expansion_338_15rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Expansion_AmmoBox_8mm_15rnd", ExpansionHardlineItemRarity.RARE);
	#endif
	
		//! Assault Rifles
		AddItem("FAL", ExpansionHardlineItemRarity.EPIC);
		AddItem("AKM", ExpansionHardlineItemRarity.EPIC);
		AddItem("AK101", ExpansionHardlineItemRarity.EPIC);
		AddItem("AK74", ExpansionHardlineItemRarity.EPIC);
		AddItem("M4A1", ExpansionHardlineItemRarity.EPIC);
		AddItem("M16A2", ExpansionHardlineItemRarity.EPIC);
		AddItem("FAMAS", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("Aug", ExpansionHardlineItemRarity.EPIC);
		AddItem("AugShort", ExpansionHardlineItemRarity.EPIC);
	
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_M16", ExpansionHardlineItemRarity.EPIC);
		AddItem("Expansion_G36", ExpansionHardlineItemRarity.EPIC);
	#endif
		
		//! Backbacks
		AddItem("ChildBag_Red", ExpansionHardlineItemRarity.COMMON);
		AddItem("ChildBag_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("ChildBag_Green", ExpansionHardlineItemRarity.COMMON);
		
		AddItem("DryBag_Orange", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DryBag_Yellow", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DryBag_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DryBag_Black", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DryBag_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DryBag_Green", ExpansionHardlineItemRarity.UNCOMMON);
		
		AddItem("TaloonBag_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("TaloonBag_Orange", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("TaloonBag_Violet", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("TaloonBag_Green", ExpansionHardlineItemRarity.UNCOMMON);
	
		AddItem("SmershBag", ExpansionHardlineItemRarity.RARE);
		
		AddItem("AssaultBag_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("AssaultBag_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("AssaultBag_Ttsko", ExpansionHardlineItemRarity.RARE);
		
		AddItem("HuntingBag", ExpansionHardlineItemRarity.RARE);
	
	#ifdef EXPANSIONMOD	
		AddItem("TortillaBag", ExpansionHardlineItemRarity.RARE);
	#else
		AddItem("TortillaBag", ExpansionHardlineItemRarity.RARE);
	#endif	
	
	#ifdef EXPANSIONMOD
		AddItem("CoyoteBag_Brown", ExpansionHardlineItemRarity.RARE);
		AddItem("CoyoteBag_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("ExpansionCoyoteBlack", ExpansionHardlineItemRarity.RARE);
	#else
		AddItem("CoyoteBag_Brown", ExpansionHardlineItemRarity.RARE);
	#endif	

		AddItem("MountainBag_Red", ExpansionHardlineItemRarity.EPIC);
		AddItem("MountainBag_Blue", ExpansionHardlineItemRarity.EPIC);
		AddItem("MountainBag_Orange", ExpansionHardlineItemRarity.EPIC);
		AddItem("MountainBag_Green", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("AliceBag_Green", ExpansionHardlineItemRarity.EPIC);
		AddItem("AliceBag_Black", ExpansionHardlineItemRarity.EPIC);
		AddItem("AliceBag_Camo", ExpansionHardlineItemRarity.EPIC);
		
	#ifdef EXPANSIONMODVEHICLE
		//! Boats
		AddItem("ExpansionUtilityBoat", ExpansionHardlineItemRarity.RARE);
		AddItem("ExpansionZodiacBoat", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionLHD", ExpansionHardlineItemRarity.EXOTIC);
	#endif
		
		//! Boots and Shoes
		AddItem("AthleticShoes_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("AthleticShoes_Brown", ExpansionHardlineItemRarity.COMMON);
		AddItem("AthleticShoes_Grey", ExpansionHardlineItemRarity.COMMON);
		AddItem("AthleticShoes_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("AthleticShoes_Green", ExpansionHardlineItemRarity.COMMON);
		
		AddItem("JoggingShoes_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("JoggingShoes_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("JoggingShoes_Red", ExpansionHardlineItemRarity.COMMON);
		AddItem("JoggingShoes_Violet", ExpansionHardlineItemRarity.COMMON);
		AddItem("JoggingShoes_White", ExpansionHardlineItemRarity.COMMON);

		AddItem("Sneakers_Green", ExpansionHardlineItemRarity.COMMON);
		AddItem("Sneakers_Red", ExpansionHardlineItemRarity.COMMON);
		AddItem("Sneakers_White", ExpansionHardlineItemRarity.COMMON);
		AddItem("Sneakers_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("Sneakers_Gray", ExpansionHardlineItemRarity.COMMON);
		
		AddItem("Ballerinas_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("Ballerinas_Red", ExpansionHardlineItemRarity.COMMON);
		AddItem("Ballerinas_White", ExpansionHardlineItemRarity.COMMON);
		AddItem("Ballerinas_Yellow", ExpansionHardlineItemRarity.COMMON);	

		AddItem("DressShoes_White", ExpansionHardlineItemRarity.COMMON);
		AddItem("DressShoes_Beige", ExpansionHardlineItemRarity.COMMON);
		AddItem("DressShoes_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("DressShoes_Brown", ExpansionHardlineItemRarity.COMMON);
		AddItem("DressShoes_Sunburst", ExpansionHardlineItemRarity.COMMON);

		AddItem("HikingBootsLow_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HikingBootsLow_Grey", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HikingBootsLow_Beige", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HikingBootsLow_Black", ExpansionHardlineItemRarity.UNCOMMON);
		
		AddItem("WorkingBoots_Yellow", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingBoots_Grey", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingBoots_Brown", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingBoots_Beige", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingBoots_Green", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("HikingBoots_Brown", ExpansionHardlineItemRarity.RARE);
		AddItem("HikingBoots_Black", ExpansionHardlineItemRarity.RARE);
	
		AddItem("CombatBoots_Beige", ExpansionHardlineItemRarity.RARE);
		AddItem("CombatBoots_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("CombatBoots_Brown", ExpansionHardlineItemRarity.RARE);
		AddItem("CombatBoots_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("CombatBoots_Grey", ExpansionHardlineItemRarity.RARE);

		AddItem("JungleBoots_Beige", ExpansionHardlineItemRarity.RARE);
		AddItem("JungleBoots_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("JungleBoots_Brown", ExpansionHardlineItemRarity.RARE);
		AddItem("JungleBoots_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("JungleBoots_Olive", ExpansionHardlineItemRarity.RARE);

		AddItem("Wellies_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("Wellies_Brown", ExpansionHardlineItemRarity.RARE);
		AddItem("Wellies_Grey", ExpansionHardlineItemRarity.RARE);
		AddItem("Wellies_Green", ExpansionHardlineItemRarity.RARE);

		AddItem("TTSKOBoots", ExpansionHardlineItemRarity.EPIC);

		AddItem("MilitaryBoots_Redpunk", ExpansionHardlineItemRarity.EPIC);
		AddItem("MilitaryBoots_Bluerock", ExpansionHardlineItemRarity.EPIC);
		AddItem("MilitaryBoots_Beige", ExpansionHardlineItemRarity.EPIC);
		AddItem("MilitaryBoots_Black", ExpansionHardlineItemRarity.EPIC);
		AddItem("MilitaryBoots_Brown", ExpansionHardlineItemRarity.EPIC);
			
		AddItem("NBCBootsGray", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("NBCBootsYellow", ExpansionHardlineItemRarity.LEGENDARY);
		
		//! Buttstocks
		AddItem("MP5k_StockBttstck", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("Fal_OeBttstck", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Fal_FoldingBttstck", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("Saiga_Bttstck", ExpansionHardlineItemRarity.RARE);

		AddItem("AKS74U_Bttstck", ExpansionHardlineItemRarity.RARE);

		AddItem("AK74_WoodBttstck", ExpansionHardlineItemRarity.RARE);

		AddItem("AK_PlasticBttstck", ExpansionHardlineItemRarity.RARE);
		AddItem("AK_WoodBttstck", ExpansionHardlineItemRarity.RARE);
		AddItem("AK_FoldingBttstck", ExpansionHardlineItemRarity.RARE);

		AddItem("M4_OEBttstck", ExpansionHardlineItemRarity.EPIC);
		AddItem("M4_MPBttstck", ExpansionHardlineItemRarity.EPIC);
		AddItem("M4_CQBBttstck", ExpansionHardlineItemRarity.EPIC);

		AddItem("PP19_Bttstck", ExpansionHardlineItemRarity.EPIC);
			
		AddItem("GhillieAtt_Tan", ExpansionHardlineItemRarity.EPIC);
		AddItem("GhillieAtt_Woodland", ExpansionHardlineItemRarity.EPIC);
		AddItem("GhillieAtt_Mossy", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("OffroadHatchback", ExpansionHardlineItemRarity.EPIC);
		AddItem("Hatchback_02", ExpansionHardlineItemRarity.EPIC);
		AddItem("Sedan_02", ExpansionHardlineItemRarity.EPIC);
		AddItem("CivilianSedan", ExpansionHardlineItemRarity.EPIC);
		AddItem("Truck_01_Covered", ExpansionHardlineItemRarity.EPIC);	
		//! Vanilla - Not released yet
		//AddItem("Truck_02", ExpansionHardlineItemRarity.EPIC);
		//AddItem("Van_01", ExpansionHardlineItemRarity.EPIC);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractor", ExpansionHardlineItemRarity.RARE);
		AddItem("ExpansionUAZ", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionUAZRoofless", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionUAZCargo", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionUAZCargoRoofless", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionBus", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("ExpansionVodnik", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("Expansion_Landrover", ExpansionHardlineItemRarity.LEGENDARY);
	#endif
		
		//! Coats and Jackets
		AddItem("LabCoat", ExpansionHardlineItemRarity.COMMON);

		AddItem("TrackSuitJacket_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("TrackSuitJacket_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("TrackSuitJacket_Green", ExpansionHardlineItemRarity.COMMON);
		AddItem("TrackSuitJacket_LightBlue", ExpansionHardlineItemRarity.COMMON);
		AddItem("TrackSuitJacket_Red", ExpansionHardlineItemRarity.COMMON);

		AddItem("DenimJacket", ExpansionHardlineItemRarity.COMMON);

		AddItem("WoolCoat_Red", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_Beige", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_RedCheck", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_BlueCheck", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_GreyCheck", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_BrownCheck", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_BlackCheck", ExpansionHardlineItemRarity.COMMON);
		AddItem("WoolCoat_Green", ExpansionHardlineItemRarity.COMMON);
	
	#ifdef EXPANSIONMOD
		AddItem("RidersJacket_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("ExpansionRidersJacketBrown", ExpansionHardlineItemRarity.COMMON);
		AddItem("ExpansionRidersJacketDarkBlue", ExpansionHardlineItemRarity.COMMON);
	#else
		AddItem("RidersJacket_Black", ExpansionHardlineItemRarity.COMMON);
	#endif
	
		AddItem("FirefighterJacket_Beige", ExpansionHardlineItemRarity.COMMON);
		AddItem("FirefighterJacket_Black", ExpansionHardlineItemRarity.COMMON);
		
		AddItem("JumpsuitJacket_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("JumpsuitJacket_Gray", ExpansionHardlineItemRarity.COMMON);
		AddItem("JumpsuitJacket_Green", ExpansionHardlineItemRarity.COMMON);
		AddItem("JumpsuitJacket_Red", ExpansionHardlineItemRarity.COMMON);

		AddItem("BomberJacket_Brown", ExpansionHardlineItemRarity.COMMON);
		AddItem("BomberJacket_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("BomberJacket_Grey", ExpansionHardlineItemRarity.COMMON);
		AddItem("BomberJacket_Maroon", ExpansionHardlineItemRarity.COMMON);
		AddItem("BomberJacket_SkyBlue", ExpansionHardlineItemRarity.COMMON);
		AddItem("BomberJacket_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("BomberJacket_Olive", ExpansionHardlineItemRarity.COMMON);

		AddItem("QuiltedJacket_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Red", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Grey", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Orange", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Yellow", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Violet", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Black", ExpansionHardlineItemRarity.COMMON);
		AddItem("QuiltedJacket_Green", ExpansionHardlineItemRarity.COMMON);

		AddItem("PrisonUniformJacket", ExpansionHardlineItemRarity.COMMON);

		AddItem("PoliceJacketOrel", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("PoliceJacket", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("ParamedicJacket_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("ParamedicJacket_Crimson", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("ParamedicJacket_Green", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("HikingJacket_Black", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HikingJacket_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HikingJacket_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HikingJacket_Green", ExpansionHardlineItemRarity.UNCOMMON);
	
		AddItem("Raincoat_Pink", ExpansionHardlineItemRarity.RARE);
		AddItem("Raincoat_Orange", ExpansionHardlineItemRarity.RARE);
		AddItem("Raincoat_Yellow", ExpansionHardlineItemRarity.RARE);
		AddItem("Raincoat_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("Raincoat_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("Raincoat_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("Raincoat_Green", ExpansionHardlineItemRarity.RARE);

		AddItem("TTsKOJacket_Camo", ExpansionHardlineItemRarity.EPIC);		

		AddItem("BDUJacket", ExpansionHardlineItemRarity.EPIC);

		AddItem("HuntingJacket_Autumn", ExpansionHardlineItemRarity.EPIC);
		AddItem("HuntingJacket_Brown", ExpansionHardlineItemRarity.EPIC);
		AddItem("HuntingJacket_Spring", ExpansionHardlineItemRarity.EPIC);
		AddItem("HuntingJacket_Summer", ExpansionHardlineItemRarity.EPIC);
		AddItem("HuntingJacket_Winter", ExpansionHardlineItemRarity.EPIC);

		AddItem("M65Jacket_Black", ExpansionHardlineItemRarity.EPIC);
		AddItem("M65Jacket_Khaki", ExpansionHardlineItemRarity.EPIC);
		AddItem("M65Jacket_Tan", ExpansionHardlineItemRarity.EPIC);
		AddItem("M65Jacket_Olive", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("GorkaEJacket_Summer", ExpansionHardlineItemRarity.EPIC);
		AddItem("GorkaEJacket_Flat", ExpansionHardlineItemRarity.EPIC);
		AddItem("GorkaEJacket_Autumn", ExpansionHardlineItemRarity.EPIC);
		AddItem("GorkaEJacket_PautRev", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("USMCJacket_Desert", ExpansionHardlineItemRarity.EPIC);
		AddItem("USMCJacket_Woodland", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("NBCJacketGray", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("NBCJacketYellow", ExpansionHardlineItemRarity.LEGENDARY);
		
		//! Containers
		AddItem("SmallProtectorCase", ExpansionHardlineItemRarity.RARE);
		AddItem("AmmoBox", ExpansionHardlineItemRarity.EPIC);
		AddItem("BarrelHoles_Blue", ExpansionHardlineItemRarity.EPIC);
		AddItem("BarrelHoles_Green", ExpansionHardlineItemRarity.EPIC);
		AddItem("BarrelHoles_Red", ExpansionHardlineItemRarity.EPIC);
		AddItem("BarrelHoles_Yellow", ExpansionHardlineItemRarity.EPIC);
		AddItem("Barrel_Blue", ExpansionHardlineItemRarity.EPIC);
		AddItem("Barrel_Green", ExpansionHardlineItemRarity.EPIC);
		AddItem("Barrel_Red", ExpansionHardlineItemRarity.EPIC);
		AddItem("Barrel_Yellow", ExpansionHardlineItemRarity.EPIC);
		AddItem("SeaChest", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("WoodenCrate", ExpansionHardlineItemRarity.LEGENDARY);
	
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionToolBox", ExpansionHardlineItemRarity.RARE);
		AddItem("ExpansionSafeMini", ExpansionHardlineItemRarity.RARE);
		AddItem("ExpansionSafeMedium", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionSafeLarge", ExpansionHardlineItemRarity.LEGENDARY);
	#endif
		
		//! Crossbows
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Crossbow", ExpansionHardlineItemRarity.RARE);
	#endif
		
		//! Electronics
		AddItem("PersonalRadio", ExpansionHardlineItemRarity.RARE);
		AddItem("Megaphone", ExpansionHardlineItemRarity.RARE);
		AddItem("ElectronicRepairKit", ExpansionHardlineItemRarity.RARE);
		AddItem("CableReel", ExpansionHardlineItemRarity.RARE);
		AddItem("BatteryCharger", ExpansionHardlineItemRarity.RARE);
		AddItem("BaseRadio", ExpansionHardlineItemRarity.EPIC);
		AddItem("Rangefinder", ExpansionHardlineItemRarity.EPIC);
		AddItem("NVGoggles", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("AlarmClock_Blue", ExpansionHardlineItemRarity.UNCOMMON);
	#ifdef DAYZ_1_17
		AddItem("KitchenTimer", ExpansionHardlineItemRarity.UNCOMMON);
	#endif
	#ifdef NAMALSK_SURVIVAL
		AddItem("dzn_tool_watch", ExpansionHardlineItemRarity.UNCOMMON);
	#endif
		//! EXPLOSIVE GRANADES
		AddItem("RGD5Grenade", ExpansionHardlineItemRarity.EPIC);
		AddItem("M67Grenade", ExpansionHardlineItemRarity.EPIC);
		
		//! FLASHBANGS
		AddItem("FlashGrenade", ExpansionHardlineItemRarity.RARE);

		//! SMOKE GRANADES
	#ifdef EXPANSIONMODWEAPONS
		AddItem("M18SmokeGrenade_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("Expansion_M18SmokeGrenade_Teargas", ExpansionHardlineItemRarity.RARE);
	#else
		AddItem("M18SmokeGrenade_Red", ExpansionHardlineItemRarity.RARE);
	#endif
		AddItem("M18SmokeGrenade_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("M18SmokeGrenade_Yellow", ExpansionHardlineItemRarity.RARE);
		AddItem("M18SmokeGrenade_Purple", ExpansionHardlineItemRarity.RARE);
		AddItem("M18SmokeGrenade_White", ExpansionHardlineItemRarity.RARE);
		AddItem("RDG2SmokeGrenade_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("RDG2SmokeGrenade_White", ExpansionHardlineItemRarity.RARE);
		
	#ifdef EXPANSIONMODMISSIONS
		//! Airdrop smoke
		AddItem("ExpansionSupplySignal", ExpansionHardlineItemRarity.EXOTIC);
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
		//! EXPLOSIVE CHARGES
		AddItem("ExpansionSatchel", ExpansionHardlineItemRarity.LEGENDARY);
	#endif
		
		//! Ghillies
		AddItem("GhillieHood_Tan", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieHood_Woodland", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieHood_Mossy", ExpansionHardlineItemRarity.LEGENDARY);
		
		AddItem("GhillieBushrag_Tan", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieBushrag_Woodland", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieBushrag_Mossy", ExpansionHardlineItemRarity.LEGENDARY);
		
		AddItem("GhillieTop_Tan", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieTop_Woodland", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieTop_Mossy", ExpansionHardlineItemRarity.LEGENDARY);
		
		AddItem("GhillieSuit_Tan", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieSuit_Woodland", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("GhillieSuit_Mossy", ExpansionHardlineItemRarity.LEGENDARY);
		
		//! Gloves
		AddItem("SurgicalGloves_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("SurgicalGloves_LightBlue", ExpansionHardlineItemRarity.COMMON);
		AddItem("SurgicalGloves_Green", ExpansionHardlineItemRarity.COMMON);
		AddItem("SurgicalGloves_White", ExpansionHardlineItemRarity.COMMON);
		
		AddItem("WorkingGloves_Yellow", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingGloves_Black", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingGloves_Beige", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WorkingGloves_Brown", ExpansionHardlineItemRarity.UNCOMMON);
	
	#ifdef EXPANSIONMOD
		AddItem("TacticalGloves_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("ExpansionTacticalGlovesDesert", ExpansionHardlineItemRarity.RARE);
	#else
		AddItem("TacticalGloves_Black", ExpansionHardlineItemRarity.RARE);
	#endif
		AddItem("TacticalGloves_Beige", ExpansionHardlineItemRarity.RARE);
		AddItem("TacticalGloves_Green", ExpansionHardlineItemRarity.RARE);
	
		AddItem("OMNOGloves_Gray", ExpansionHardlineItemRarity.EPIC);
		AddItem("OMNOGloves_Brown", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("NBCGlovesGray", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("NBCGlovesYellow", ExpansionHardlineItemRarity.LEGENDARY);
		
		//! Handguards
		AddItem("MP5_PlasticHndgrd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("MP5_RailHndgrd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("AK74_Hndgrd", ExpansionHardlineItemRarity.RARE);

		AddItem("AK_WoodHndgrd", ExpansionHardlineItemRarity.EPIC);
		AddItem("AK_RailHndgrd", ExpansionHardlineItemRarity.EPIC);
		AddItem("AK_PlasticHndgrd", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("M4_PlasticHndgrd", ExpansionHardlineItemRarity.RARE);
		AddItem("M4_RISHndgrd", ExpansionHardlineItemRarity.EPIC);
		AddItem("M4_MPHndgrd", ExpansionHardlineItemRarity.EPIC);
		
		//! Hats
		AddItem("MedicalScrubsHat_Blue", ExpansionHardlineItemRarity.COMMON);
		AddItem("MedicalScrubsHat_White", ExpansionHardlineItemRarity.COMMON);
		AddItem("MedicalScrubsHat_Green", ExpansionHardlineItemRarity.COMMON);

	#ifdef EXPANSIONMOD
		AddItem("CowboyHat_Brown", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("ExpansionCowboyHatGator", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("ExpansionCowboyHatSnake", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("ExpansionCowboyHatRattleSnake", ExpansionHardlineItemRarity.UNCOMMON);
	#else
		AddItem("CowboyHat_Brown", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("CowboyHat_black", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("CowboyHat_darkBrown", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("CowboyHat_green", ExpansionHardlineItemRarity.UNCOMMON);
	#endif

		AddItem("MilitaryBeret_ChDKZ", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("MilitaryBeret_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("MilitaryBeret_UN", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("MilitaryBeret_CDF", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("MilitaryBeret_NZ", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("BeanieHat_Pink", ExpansionHardlineItemRarity.UNCOMMON);		
		AddItem("BeanieHat_Beige", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BeanieHat_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BeanieHat_Brown", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BeanieHat_Grey", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BeanieHat_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BeanieHat_Black", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BeanieHat_Green", ExpansionHardlineItemRarity.UNCOMMON);
		
		AddItem("Ushanka_Black", ExpansionHardlineItemRarity.EPIC);
		AddItem("Ushanka_Blue", ExpansionHardlineItemRarity.EPIC);
		AddItem("Ushanka_Green", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("BoonieHat_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_NavyBlue", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Orange", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Tan", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Olive", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_DPM", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Dubok", ExpansionHardlineItemRarity.RARE);
		AddItem("BoonieHat_Flecktran", ExpansionHardlineItemRarity.RARE);

		AddItem("OfficerHat", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("NBCHoodGray", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("NBCHoodYellow", ExpansionHardlineItemRarity.LEGENDARY);
		
		//! Helis
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionGyrocopter", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("ExpansionUh1h", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("ExpansionMerlin", ExpansionHardlineItemRarity.LEGENDARY);
	#endif
		
		//! Helmets
		AddItem("ConstructionHelmet_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("ConstructionHelmet_Orange", ExpansionHardlineItemRarity.RARE);
		AddItem("ConstructionHelmet_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("ConstructionHelmet_White", ExpansionHardlineItemRarity.RARE);
		AddItem("ConstructionHelmet_Yellow", ExpansionHardlineItemRarity.RARE);
		AddItem("ConstructionHelmet_Lime", ExpansionHardlineItemRarity.RARE);
		
		AddItem("SkateHelmet_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("SkateHelmet_Gray", ExpansionHardlineItemRarity.RARE);
		AddItem("SkateHelmet_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("SkateHelmet_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("SkateHelmet_Green", ExpansionHardlineItemRarity.RARE);

		AddItem("HockeyHelmet_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("HockeyHelmet_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("HockeyHelmet_White", ExpansionHardlineItemRarity.RARE);
		AddItem("HockeyHelmet_Black", ExpansionHardlineItemRarity.RARE);

		AddItem("DirtBikeHelmet_Mouthguard", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Visor", ExpansionHardlineItemRarity.RARE);
		
		AddItem("DirtBikeHelmet_Chernarus", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Police", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("DirtBikeHelmet_Khaki", ExpansionHardlineItemRarity.RARE);

		AddItem("MotoHelmet_Lime", ExpansionHardlineItemRarity.RARE);
		AddItem("MotoHelmet_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("MotoHelmet_Red", ExpansionHardlineItemRarity.RARE);
		AddItem("MotoHelmet_White", ExpansionHardlineItemRarity.RARE);
		AddItem("MotoHelmet_Grey", ExpansionHardlineItemRarity.RARE);
		AddItem("MotoHelmet_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("MotoHelmet_Green", ExpansionHardlineItemRarity.RARE);

		AddItem("DarkMotoHelmet_Grey", ExpansionHardlineItemRarity.EPIC);
		AddItem("DarkMotoHelmet_Lime", ExpansionHardlineItemRarity.EPIC);
		AddItem("DarkMotoHelmet_Blue", ExpansionHardlineItemRarity.EPIC);
		AddItem("DarkMotoHelmet_Red", ExpansionHardlineItemRarity.EPIC);
		AddItem("DarkMotoHelmet_White", ExpansionHardlineItemRarity.EPIC);
		AddItem("DarkMotoHelmet_Black", ExpansionHardlineItemRarity.EPIC);
		AddItem("DarkMotoHelmet_Green", ExpansionHardlineItemRarity.EPIC);

		AddItem("TankerHelmet", ExpansionHardlineItemRarity.EPIC);
		
		AddItem("GreatHelm", ExpansionHardlineItemRarity.EPIC);
	
		AddItem("ZSh3PilotHelmet", ExpansionHardlineItemRarity.EPIC);
		AddItem("ZSh3PilotHelmet_Green", ExpansionHardlineItemRarity.EPIC);
		AddItem("ZSh3PilotHelmet_Black", ExpansionHardlineItemRarity.EPIC);
			
		AddItem("FirefightersHelmet_Red", ExpansionHardlineItemRarity.EPIC);
		AddItem("FirefightersHelmet_White", ExpansionHardlineItemRarity.EPIC);
		AddItem("FirefightersHelmet_Yellow", ExpansionHardlineItemRarity.EPIC);
				
	#ifdef EXPANSIONMOD
		AddItem("Mich2001Helmet", ExpansionHardlineItemRarity.EPIC);
		AddItem("ExpansionMich2001Desert", ExpansionHardlineItemRarity.EPIC);
	#else
		AddItem("Mich2001Helmet", ExpansionHardlineItemRarity.EPIC);
	#endif
		
		AddItem("GorkaHelmetVisor", ExpansionHardlineItemRarity.EPIC);
		AddItem("GorkaHelmet", ExpansionHardlineItemRarity.EPIC);
		AddItem("GorkaHelmet_Black", ExpansionHardlineItemRarity.EPIC);

		AddItem("Ssh68Helmet", ExpansionHardlineItemRarity.EPIC);

		AddItem("BallisticHelmet_UN", ExpansionHardlineItemRarity.EPIC);
		AddItem("BallisticHelmet_Black", ExpansionHardlineItemRarity.EPIC);
		AddItem("BallisticHelmet_Green", ExpansionHardlineItemRarity.EPIC);
		
		// Holsters/Pouches
		AddItem("ChestHolster", ExpansionHardlineItemRarity.RARE);	
		AddItem("PlateCarrierHolster", ExpansionHardlineItemRarity.RARE);
		AddItem("PlateCarrierPouches", ExpansionHardlineItemRarity.RARE);
		AddItem("NylonKnifeSheath", ExpansionHardlineItemRarity.RARE);
		
		//! Kits
		AddItem("SewingKit", ExpansionHardlineItemRarity.RARE);
		AddItem("LeatherSewingKit", ExpansionHardlineItemRarity.RARE);
		AddItem("WeaponCleaningKit", ExpansionHardlineItemRarity.RARE);
		
		//! Knifes
		AddItem("KitchenKnife", ExpansionHardlineItemRarity.RARE);
		AddItem("SteakKnife", ExpansionHardlineItemRarity.COMMON);
		AddItem("HuntingKnife", ExpansionHardlineItemRarity.RARE);
		AddItem("CombatKnife", ExpansionHardlineItemRarity.EPIC);
		AddItem("KukriKnife", ExpansionHardlineItemRarity.RARE);
		AddItem("FangeKnife", ExpansionHardlineItemRarity.COMMON);
		AddItem("Machete", ExpansionHardlineItemRarity.RARE);
		AddItem("CrudeMachete", ExpansionHardlineItemRarity.RARE);
		AddItem("OrientalMachete", ExpansionHardlineItemRarity.RARE);
		AddItem("Cleaver", ExpansionHardlineItemRarity.RARE);
		
		//! Launchers
		AddItem("Flaregun", ExpansionHardlineItemRarity.RARE);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("ExpansionLAW", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("ExpansionRPG7", ExpansionHardlineItemRarity.LEGENDARY);
		AddItem("Expansion_M79", ExpansionHardlineItemRarity.LEGENDARY);
	#endif
		
		//! Lights
		AddItem("Chemlight_Blue", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Chemlight_Green", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Chemlight_Red", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Chemlight_White", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Chemlight_Yellow", ExpansionHardlineItemRarity.UNCOMMON);

		AddItem("Roadflare", ExpansionHardlineItemRarity.RARE);
		AddItem("Matchbox", ExpansionHardlineItemRarity.RARE);
		AddItem("PetrolLighter", ExpansionHardlineItemRarity.RARE);
		AddItem("Flashlight", ExpansionHardlineItemRarity.RARE);
		AddItem("XmasLights", ExpansionHardlineItemRarity.RARE);
		AddItem("PortableGasLamp", ExpansionHardlineItemRarity.RARE);
		AddItem("PortableGasStove", ExpansionHardlineItemRarity.RARE);
		AddItem("Headtorch_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("Headtorch_Grey", ExpansionHardlineItemRarity.RARE);
		AddItem("Spotlight", ExpansionHardlineItemRarity.RARE);
		
		AddItem("UniversalLight", ExpansionHardlineItemRarity.RARE);
		AddItem("TLRLight", ExpansionHardlineItemRarity.RARE);
		
		//! Liquids
		AddItem("EngineOil", ExpansionHardlineItemRarity.EPIC);	
		AddItem("CanisterGasoline", ExpansionHardlineItemRarity.EPIC);
		
		//! Locks
		AddItem("CombinationLock", ExpansionHardlineItemRarity.RARE);
		AddItem("CombinationLock4", ExpansionHardlineItemRarity.EPIC);
	#ifdef EXPANSIONMODBASEBUILDING		
		AddItem("ExpansionCodeLock", ExpansionHardlineItemRarity.EPIC);
	#endif
		
		AddItem("Mag_IJ70_8Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_CZ75_15Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_Glock_15Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_MKII_10Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_1911_7Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_FNX45_15Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_Deagle_9Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_CZ527_5rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_CZ61_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_PP19_64Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_UMP_25Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_MP5_15Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_MP5_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_FAL_20Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_Saiga_5Rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_Saiga_8Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Saiga_Drum20Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_AKM_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_AKM_Palm30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_AKM_Drum75Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_AK101_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_AK74_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_AK74_45Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_STANAG_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_STANAGCoupled_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_STANAG_60Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_CMAG_10Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_CMAG_20Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_CMAG_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_CMAG_40Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_VSS_10Rnd", ExpansionHardlineItemRarity.EPIC);	
		AddItem("Mag_VAL_20Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Ruger1022_15Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Ruger1022_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_SVD_10Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Scout_5Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_FAMAS_25Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Aug_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_P1_8Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_CZ550_4rnd", ExpansionHardlineItemRarity.RARE);
		AddItem("Mag_CZ550_10rnd", ExpansionHardlineItemRarity.EPIC);

	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Mag_Expansion_Taser", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_Expansion_M9_15Rnd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Mag_Expansion_Kedr_20Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Expansion_MP7_40Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Expansion_Vityaz_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Expansion_MPX_50Rnd", ExpansionHardlineItemRarity.EPIC);			
		AddItem("Mag_Expansion_M14_10Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Expansion_M14_20Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Expansion_G36_30Rnd", ExpansionHardlineItemRarity.EPIC);
		AddItem("Mag_Expansion_AWM_5Rnd", ExpansionHardlineItemRarity.LEGENDARY);
	#endif
		
		//! Masks
		AddItem("SurgicalMask", ExpansionHardlineItemRarity.COMMON);
		AddItem("NioshFaceMask", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("HockeyMask", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BalaclavaMask_Beige", ExpansionHardlineItemRarity.RARE);
		AddItem("BalaclavaMask_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("BalaclavaMask_Pink", ExpansionHardlineItemRarity.RARE);
		AddItem("BalaclavaMask_White", ExpansionHardlineItemRarity.RARE);
		AddItem("BalaclavaMask_Blackskull", ExpansionHardlineItemRarity.RARE);
		AddItem("BalaclavaMask_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("BalaclavaMask_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("Balaclava3Holes_Beige", ExpansionHardlineItemRarity.RARE);
		AddItem("Balaclava3Holes_Blue", ExpansionHardlineItemRarity.RARE);
		AddItem("Balaclava3Holes_Black", ExpansionHardlineItemRarity.RARE);
		AddItem("Balaclava3Holes_Green", ExpansionHardlineItemRarity.RARE);
		AddItem("WeldingMask", ExpansionHardlineItemRarity.RARE);
		AddItem("GasMask", ExpansionHardlineItemRarity.EPIC);
		AddItem("GP5GasMask", ExpansionHardlineItemRarity.EPIC);
		AddItem("AirborneMask", ExpansionHardlineItemRarity.EPIC);

		//! Meat
		AddItem("BearSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("GoatSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BoarSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("PigSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DeerSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("WolfSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("CowSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("SheepSteakMeat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("ChickenBreastMeat", ExpansionHardlineItemRarity.UNCOMMON);
	#ifdef EXPANSIONMOD
		AddItem("RabbitLegMeat", ExpansionHardlineItemRarity.UNCOMMON);
	#endif

		//! Medical
		AddItem("CharcoalTablets", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BandageDressing", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("DisinfectantAlcohol", ExpansionHardlineItemRarity.RARE);
		AddItem("PurificationTablets", ExpansionHardlineItemRarity.RARE);
		AddItem("BloodTestKit", ExpansionHardlineItemRarity.RARE);
		AddItem("Thermometer", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("VitaminBottle", ExpansionHardlineItemRarity.RARE);
		AddItem("DisinfectantSpray", ExpansionHardlineItemRarity.RARE);
		AddItem("TetracyclineAntibiotics", ExpansionHardlineItemRarity.RARE);
		AddItem("PainkillerTablets", ExpansionHardlineItemRarity.RARE);
		AddItem("StartKitIV", ExpansionHardlineItemRarity.RARE);
		AddItem("Heatpack", ExpansionHardlineItemRarity.RARE);
		AddItem("SalineBag", ExpansionHardlineItemRarity.EPIC);
		AddItem("BloodBagEmpty", ExpansionHardlineItemRarity.EPIC);
		AddItem("FirstAidKit", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Epinephrine", ExpansionHardlineItemRarity.EPIC);
		AddItem("Morphine", ExpansionHardlineItemRarity.EPIC);
		AddItem("IodineTincture", ExpansionHardlineItemRarity.EPIC);
		AddItem("AntiChemInjector", ExpansionHardlineItemRarity.EPIC);

		//! Melee
		AddItem("BrassKnuckles_Dull", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BrassKnuckles_Shiny", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("StunBaton", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Broom", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Broom_Birch", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("Pipe", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("CattleProd", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BaseballBat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("NailedBaseballBat", ExpansionHardlineItemRarity.UNCOMMON);
		AddItem("BarbedBaseballBat", ExpansionHardlineItemRarity.UNCOMMON);
		
		//! Compensators
		AddItem("Mosin_Compensator", ExpansionHardlineItemRarity.RARE);
		AddItem("MP5_Compensator", ExpansionHardlineItemRarity.RARE);

		//! Supressors
		AddItem("M4_Suppressor", ExpansionHardlineItemRarity.EPIC);
		AddItem("AK_Suppressor", ExpansionHardlineItemRarity.EPIC);
		AddItem("PistolSuppressor", ExpansionHardlineItemRarity.RARE);

	}
	
	void AddItem(string type, int rarity)
	{
		ExpansionHardlineItemData data = new ExpansionHardlineItemData;
		type.ToLower();
		data.SetType(type);
		data.SetRatity(rarity);
		HardlineItemDatas.Insert(data);
	}
	
	override string SettingName()
	{
		return "Hardline Settings";
	}
	
	ExpansionHardlineItemData GetHardlineItemDataByType(string type)
	{
		if (!HardlineItemDatas || HardlineItemDatas.Count() == 0)
			return NULL;
		
		for (int i = 0; i < HardlineItemDatas.Count(); i++)
		{
			ExpansionHardlineItemData current = HardlineItemDatas[i];
			if (!current)
				continue;
			
			if (current.GetType() == type)
				return current;
		}
		
		return NULL;
	}
};
