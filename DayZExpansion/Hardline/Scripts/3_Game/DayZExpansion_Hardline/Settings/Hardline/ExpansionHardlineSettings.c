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

/**@class		ExpansionHardlineSettings
 * @brief		Hardline settings class
 **/
class ExpansionHardlineSettings: ExpansionSettingBase
{
	static const int VERSION = 10;

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
	bool UseItemRarityOnInventoryIcons;
	bool UseItemRarityForMarketPurchase;
	bool UseItemRarityForMarketSell;

	int MaxReputation;
	int ReputationLossPercent;
	int ReputationBonusPercent;
	
	ExpansionHardlineItemRarity DefaultItemRatity;
	//! Maps need to be always set as the last params
	ref map<string, int> EntityReputation;

	ref map<string, ExpansionHardlineItemRarity> ItemRarity;

	[NonSerialized()]
	private bool m_IsLoaded;

	void ExpansionHardlineSettings()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this);
	#endif

		EntityReputation = new map<string, int>;
		ItemRarity = new map<string, ExpansionHardlineItemRarity>;
	}

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

		if (!ctx.Read(UseItemRarityOnInventoryIcons))
		{
			Error(ToString() + "::OnRecieve UseItemRarityOnInventoryIcons");
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

	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(ctx);
	#endif

		ctx.Write(ShowHardlineHUD);
		ctx.Write(UseReputation);
		ctx.Write(EnableItemRarity);
		ctx.Write(UseItemRarityOnInventoryIcons);
	#ifdef EXPANSIONMOD
		ctx.Write(MaxReputation);
	#endif
	}

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
		UseItemRarityOnInventoryIcons = s.UseItemRarityOnInventoryIcons;
		UseItemRarityForMarketPurchase = s.UseItemRarityForMarketPurchase;
		UseItemRarityForMarketSell = s.UseItemRarityForMarketSell;

		MaxReputation = s.MaxReputation;
		ReputationLossPercent = s.ReputationLossPercent;
		ReputationBonusPercent = s.ReputationBonusPercent;
		EntityReputation = s.EntityReputation;

		DefaultItemRatity = s.DefaultItemRatity;
		
		EntityReputation = s.EntityReputation;
		ItemRarity = s.ItemRarity;
	}

	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		m_IsLoaded = false;
	}

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

			#ifdef EXPANSIONMODAI
				if (m_Version < 7)
				{
					UseFactionReputation = settingsDefault.UseFactionReputation;
					EnableFactionPersistence = settingsDefault.EnableFactionPersistence;
				}
			#endif

				if (m_Version < 8)
					MaxReputation = settingsDefault.MaxReputation;

				if (m_Version < 9)
				{
					UseItemRarityOnInventoryIcons = settingsDefault.UseItemRarityOnInventoryIcons;
					ReputationLossPercent = settingsDefault.ReputationLossPercent;
					ReputationBonusPercent = settingsDefault.ReputationBonusPercent;
					DefaultEntityReputation();
				}
				
				if (m_Version < 10)
					DefaultItemRatity = settingsDefault.DefaultItemRatity;

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
		map<string, ExpansionHardlineItemRarity> itemRarity = new map<string, ExpansionHardlineItemRarity>;
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

	override bool OnSave()
	{
		Print("[ExpansionHardlineSettings] Saving settings");

		JsonFileLoader<ExpansionHardlineSettings>.JsonSaveFile( EXPANSION_HARDLINE_SETTINGS, this );

		return true;
	}

	override void Update( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_Hardline.Invoke();
	}

	override void Defaults()
	{
		m_Version = VERSION;

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
		UseItemRarityOnInventoryIcons = true;
		UseItemRarityForMarketPurchase = true;
		UseItemRarityForMarketSell = true;

		MaxReputation = 0;

		ReputationLossPercent = 10;
		ReputationBonusPercent = 10;

		DefaultItemRatity = ExpansionHardlineItemRarity.Common;
		
		DefaultEntityReputation();

		DefaultItemRarity();
	}

	protected void DefaultEntityReputation()
	{
		//! Player
		EntityReputation.Insert("PlayerBase", 100);

		//! Animals
		EntityReputation.Insert("Animal_BosTaurus", 10);
		EntityReputation.Insert("Animal_CanisLupus_Grey", 10);
		EntityReputation.Insert("Animal_CanisLupus_White", 10);
		EntityReputation.Insert("Animal_CapraHircus", 10);
		EntityReputation.Insert("Animal_CapreolusCapreolus", 10);
		EntityReputation.Insert("Animal_CervusElaphus", 10);
		EntityReputation.Insert("Animal_GallusGallusDomesticus", 1);
		EntityReputation.Insert("Animal_OvisAries", 10);
		EntityReputation.Insert("Animal_SusDomesticus", 10);
		EntityReputation.Insert("Animal_SusScrofa", 10);
		EntityReputation.Insert("Animal_UrsusArctos", 50);
		EntityReputation.Insert("Animal_UrsusMaritimus", 50);

		//! Civilian Infected
		EntityReputation.Insert("ZmbM_CitizenASkinny_Base", 10);
		EntityReputation.Insert("ZmbM_CitizenASkinny_LT_Base", 10);
		EntityReputation.Insert("ZmbM_CitizenBFat_Base", 10);
		EntityReputation.Insert("ZmbM_ClerkFat_Base", 10);
		EntityReputation.Insert("ZmbM_ClerkFat_LT_Base", 10);
		EntityReputation.Insert("ZmbM_CommercialPilotOld_Base", 10);
		EntityReputation.Insert("ZmbM_CommercialPilotOld_LT_Base", 10);
		EntityReputation.Insert("ZmbM_ConstrWorkerNormal_Base", 10);
		EntityReputation.Insert("ZmbM_DoctorFat_Base", 10);
		EntityReputation.Insert("ZmbM_FarmerFat_Base", 10);
		EntityReputation.Insert("ZmbM_FarmerFat_LT_Base", 10);
		EntityReputation.Insert("ZmbM_FirefighterNormal_Base", 10);
		EntityReputation.Insert("ZmbM_FishermanOld_Base", 10);
		EntityReputation.Insert("ZmbM_HandymanNormal_Base", 10);
		EntityReputation.Insert("ZmbM_HeavyIndustryWorker_Base", 10);
		EntityReputation.Insert("ZmbM_HermitSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_HikerSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_HunterOld_Base", 10);
		EntityReputation.Insert("ZmbM_Jacket_Base", 10);
		EntityReputation.Insert("ZmbM_Jacket_LT_Base", 10);
		EntityReputation.Insert("ZmbM_JoggerSkinny_Base", 10);
		EntityReputation.Insert("ZmbM__Runner_Base", 10);
		EntityReputation.Insert("ZmbM_JournalistSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_MechanicSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_MotobikerFat_Base", 10);
		EntityReputation.Insert("ZmbM_OffshoreWorker_Base", 10);
		EntityReputation.Insert("ZmbM_ParamedicNormal_Base", 10);
		EntityReputation.Insert("ZmbM_PatientSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_PolicemanFat_Base", 10);
		EntityReputation.Insert("ZmbM_PolicemanSpecForce_Base", 10);
		EntityReputation.Insert("ZmbM_priestPopSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_PrisonerSkinny_Base", 10);
		EntityReputation.Insert("ZmbM_SkaterYoung_Base", 10);
		EntityReputation.Insert("ZmbM_SkaterYoung_LT_Base", 10);
		EntityReputation.Insert("ZmbM_SurvivorDean_Base", 10);
		EntityReputation.Insert("ZmbM_VillagerOld_Base", 10);
		EntityReputation.Insert("ZmbM_VillagerOld_LT_Base", 10);
		EntityReputation.Insert("ZmbF_BlueCollarFat_Base", 10);
		EntityReputation.Insert("ZmbF_CitizenANormal_Base", 10);
		EntityReputation.Insert("ZmbF_CitizenANormal_LT_Base", 10);
		EntityReputation.Insert("ZmbF_CitizenBSkinny_Base", 10);
		EntityReputation.Insert("ZmbF_Clerk_Normal_Base", 10);
		EntityReputation.Insert("ZmbF_ClerkFat_Base", 10);
		EntityReputation.Insert("ZmbF_Clerk_Normal_LT_Base", 10);
		EntityReputation.Insert("ZmbF_DoctorSkinny_Base", 10);
		EntityReputation.Insert("ZmbF_HikerSkinny_Base", 10);
		EntityReputation.Insert("ZmbF_JoggerSkinny_Base", 10);
		EntityReputation.Insert("ZmbF_Runner_Base", 10);
		EntityReputation.Insert("ZmbF_JournalistNormal_Base", 10);
		EntityReputation.Insert("ZmbF_JournalistNormal_LT_Base", 10);
		EntityReputation.Insert("ZmbF_MechanicNormal_Base", 10);
		EntityReputation.Insert("ZmbF_MilkMaidOld_Base", 10);
		EntityReputation.Insert("ZmbF_MilkMaidOld_LT_Base", 10);
		EntityReputation.Insert("ZmbF_NurseFat_Base", 10);
		EntityReputation.Insert("ZmbF_ParamedicNormal_Base", 10);
		EntityReputation.Insert("ZmbF_PatientOld_Base", 10);
		EntityReputation.Insert("ZmbF_PoliceWomanNormal_Base", 10);
		EntityReputation.Insert("ZmbF_ShortSkirt_Base", 10);
		EntityReputation.Insert("ZmbF_ShortSkirt_LT_Base", 10);
		EntityReputation.Insert("ZmbF_SkaterYoung_Base", 10);
		EntityReputation.Insert("ZmbF_SkaterYoung_LT_Base", 10);
		EntityReputation.Insert("ZmbF_SurvivorNormal_Base", 10);
		EntityReputation.Insert("ZmbF_SurvivorNormal_LT_Base", 10);
		EntityReputation.Insert("ZmbF_VillagerOld_Base", 10);
		EntityReputation.Insert("ZmbF_VillagerOld_LT_Base", 10);

		//! Military Infected
		EntityReputation.Insert("ZmbM_PatrolNormal_Base", 20);
		EntityReputation.Insert("ZmbM_Soldier_Base", 20);
		EntityReputation.Insert("ZmbM_SoldierNormal_Base", 20);
		EntityReputation.Insert("ZmbM_usSoldier_normal_Base", 20);

		//! Imune Infected
		EntityReputation.Insert("ZmbM_NBC_Yellow", 20);

		//! Imune Military Infected
		EntityReputation.Insert("ZmbM_NBC_Grey", 20);

	#ifdef EXPANSIONMODAI
		//! AI
		EntityReputation.Insert("eAIBase", 100);
	#endif
	}

	protected void DefaultItemRarity()
	{
	#ifdef NAMALSK_SURVIVAL
		//! Namalsk Consumables
		AddItem("dzn_Canteen", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_WaterBottle", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SodaCan_Pipsi", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SodaCan_Cola", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SodaCan_Spite", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SodaCan_Kvass", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SodaCan_Fronta", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_BakedBeansCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_PeachesCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SardinesCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SpaghettiCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_TunaCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_UnknownFoodCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_DogFoodCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_CatFoodCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_PorkCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Lunchmeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Pajka", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Pate", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Marmalade", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Honey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Pot", ExpansionHardlineItemRarity.Uncommon);

		//! Namalsk Medical
		AddItem("dzn_vega7_enzyme", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_oxygenated_blood", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_FirstAidKit", ExpansionHardlineItemRarity.Rare);

		//! Namalsk Misc
		AddItem("dzn_DuctTape", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_EpoxyPutty", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ElectronicRepairKit", ExpansionHardlineItemRarity.Rare);

		//! Namalsk Lore
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
		AddItem("dzn_phoenix_stash", ExpansionHardlineItemRarity.Legendary);

		AddItem("dzn_apsi", ExpansionHardlineItemRarity.Legendary);
		AddItem("dzn_vinyl_bubson", ExpansionHardlineItemRarity.Legendary);

		AddItem("dzn_detector", ExpansionHardlineItemRarity.Epic);

		//! Namalsk Tools
		AddItem("dzn_tool_watch", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_tool_watch2", ExpansionHardlineItemRarity.Uncommon);

		//! Namalsk LEHS Suit Items
		AddItem("dzn_lehs_o2tank", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_lehs_battery", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_blueprint_lehs", ExpansionHardlineItemRarity.Legendary);
		AddItem("dzn_lehs", ExpansionHardlineItemRarity.Exotic);
		AddItem("dzn_lehs_helmet", ExpansionHardlineItemRarity.Exotic);

		//! Namalsk Clothing
		AddItem("dzn_gorka_up_winter", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_sumrak_up_coldw", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_yeger_up_coldw", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_yeger_up_woodl", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_bubblegoose_blue", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_bubblegoose_red", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_bubblegoose_orange", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_bubblegoose_green", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_tacticalshirt_olive_ru", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_tacticalshirt_tan_ru", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_tacticalshirt_white_ru", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_gorka_pants_winter", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_sumrak_pants_coldw", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_yeger_pants_coldw", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_yeger_pants_woodl", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_ukassvest_white", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_smershvest_white", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_smershvest_black", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_ballisticvest_nac", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_platecarriervest_nac", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_platecarrierholster_nac", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_platecarrierpouches_nac", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_highcapacityvest_nac", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_bagtortilla_white", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_assaultbag_winter", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_smershbackpack_white", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_smershbackpack_black", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_coyotebag_white", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_alicebag_wintercamo", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_bandana_wintercamopattern", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_gorkahood_flat", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_gorkahood_autumn", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_gorkahood_pautrev", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_gorkahood_winter", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_sumrakhood_coldw", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_yegerhood_coldw", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_yegerhood_woodl", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_milberet_red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_milberet_blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_milberet_green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_milberet_black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_milberet_specnaz", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_BallisticHelmet_White", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_GorkaHelmet_White", ExpansionHardlineItemRarity.Epic);
		AddItem("dzn_att_ghillie_winter", ExpansionHardlineItemRarity.Epic);

		//! Namalsk Attachments
		AddItem("dzn_att_ghillie_tandark", ExpansionHardlineItemRarity.Rare);

		//! Namalsk Ghillies
		AddItem("dzn_ghilliebushragwinter", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghilliebushragtandark", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghillietopwinter", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghillietoptandark", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghilliesuitwinter", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghilliesuittandark", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghilliehoodwinter", ExpansionHardlineItemRarity.Rare);
		AddItem("dzn_ghilliehoodtandark", ExpansionHardlineItemRarity.Rare);

		//! Namalsk Memes
		AddItem("dzn_meme_banana", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_snowball_large", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_snowball_medium", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_snowball_small", ExpansionHardlineItemRarity.Uncommon);

		//! Namalsk Opened Cans
		AddItem("dzn_BakedBeansCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_PeachesCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SpaghettiCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_SardinesCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_TunaCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_UnknownFoodCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_DogFoodCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_CatFoodCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_PorkCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Lunchmeat_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Pajka_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("dzn_Pate_Opened", ExpansionHardlineItemRarity.Uncommon);
	#endif

	#ifdef EXPANSIONMODMARKET
		AddItem("ExpansionGoldBar", ExpansionHardlineItemRarity.Epic);
		AddItem("ExpansionGoldNugget", ExpansionHardlineItemRarity.Epic);
	#endif

		//! Ammo
		AddItem("Ammo_12gaPellets", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_12gaRubberSlug", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_12gaSlug", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_22", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_308Win", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_308WinTracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_357", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_380", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_45ACP", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_545x39", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_545x39Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_556x45", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_556x45Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_762x39", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_762x39Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_762x54", ExpansionHardlineItemRarity.Rare);
		AddItem("Ammo_762x54Tracer", ExpansionHardlineItemRarity.Rare);

		AddItem("Ammo_9x19", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_9x39", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ammo_9x39AP", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_Flare", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Ammo_HuntingBolt", ExpansionHardlineItemRarity.Uncommon);
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

		//! Armbands
		AddItem("Armband_White", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Yellow", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Orange", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Green", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Pink", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Black", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_APA", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Altis", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_BabyDeer", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Bear", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Bohemia", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_BrainZ", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_CDF", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_CHEL", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_CMC", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Cannibals", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Chedaki", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Chernarus", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_DayZ", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_HunterZ", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Livonia", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_LivoniaArmy", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_LivoniaPolice", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_NAPA", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_NSahrani", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Pirates", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_RSTA", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Refuge", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Rooster", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_SSahrani", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Snake", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_TEC", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_UEC", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Wolf", ExpansionHardlineItemRarity.Common);
		AddItem("Armband_Zenit", ExpansionHardlineItemRarity.Common);

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

		//! Backpacks
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
		AddItem("TortillaBag", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMOD
		AddItem("CoyoteBag_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionCoyoteBlack", ExpansionHardlineItemRarity.Rare);
	#endif
		AddItem("CoyoteBag_Brown", ExpansionHardlineItemRarity.Rare);

		AddItem("MountainBag_Red", ExpansionHardlineItemRarity.Epic);
		AddItem("MountainBag_Blue", ExpansionHardlineItemRarity.Epic);
		AddItem("MountainBag_Orange", ExpansionHardlineItemRarity.Epic);
		AddItem("MountainBag_Green", ExpansionHardlineItemRarity.Epic);

		AddItem("AliceBag_Green", ExpansionHardlineItemRarity.Epic);
		AddItem("AliceBag_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("AliceBag_Camo", ExpansionHardlineItemRarity.Epic);

		//! Bandanas
		AddItem("Bandana_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Bandana_Pink", ExpansionHardlineItemRarity.Common);
		AddItem("Bandana_Yellow", ExpansionHardlineItemRarity.Common);
		AddItem("Bandana_RedPattern", ExpansionHardlineItemRarity.Common);
		AddItem("Bandana_BlackPattern", ExpansionHardlineItemRarity.Common,);
		AddItem("Bandana_PolkaPattern", ExpansionHardlineItemRarity.Common);
		AddItem("Bandana_Greenpattern", ExpansionHardlineItemRarity.Common);
		AddItem("Bandana_CamoPattern", ExpansionHardlineItemRarity.Common);

		//! Batteries
		AddItem("Battery9V", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CarBattery", ExpansionHardlineItemRarity.Rare);
		AddItem("TruckBattery", ExpansionHardlineItemRarity.Rare);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionHelicopterBattery", ExpansionHardlineItemRarity.Epic);
		//AddItem("ExpansionAircraftBattery", ExpansionHardlineItemRarity.Epic);
	#endif

		//! Bayonets
		AddItem("Mosin_Bayonet", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SKS_Bayonet", ExpansionHardlineItemRarity.Uncommon);
		AddItem("M9A1_Bayonet", ExpansionHardlineItemRarity.Uncommon);
		AddItem("AK_Bayonet", ExpansionHardlineItemRarity.Uncommon);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kar98_Bayonet", ExpansionHardlineItemRarity.Uncommon);
	#endif

		//! Belts
		AddItem("CivilianBelt", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MilitaryBelt", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HipPack_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HipPack_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HipPack_Medical", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HipPack_Party", ExpansionHardlineItemRarity.Uncommon);

		//! Blouses and Suits
		AddItem("Blouse_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Blouse_Violet", ExpansionHardlineItemRarity.Common);
		AddItem("Blouse_White", ExpansionHardlineItemRarity.Common);
		AddItem("Blouse_Green", ExpansionHardlineItemRarity.Common);

		AddItem("ManSuit_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_Black", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_DarkGrey", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_Khaki", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_LightGrey", ExpansionHardlineItemRarity.Common);
		AddItem("ManSuit_White", ExpansionHardlineItemRarity.Common);

		AddItem("WomanSuit_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_Black", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_DarkGrey", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_Khaki", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_LightGrey", ExpansionHardlineItemRarity.Common);
		AddItem("WomanSuit_White", ExpansionHardlineItemRarity.Common);

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

		//! Caps
		AddItem("BaseballCap_CMMG_Pink", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_Pink", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BaseballCap_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_Red", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_CMMG_Black", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_Black", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_Olive", ExpansionHardlineItemRarity.Common);
		AddItem("BaseballCap_Camo", ExpansionHardlineItemRarity.Common);

		AddItem("PrisonerCap", ExpansionHardlineItemRarity.Common);
		AddItem("PilotkaCap", ExpansionHardlineItemRarity.Common);
		AddItem("PoliceCap", ExpansionHardlineItemRarity.Common);

		AddItem("FlatCap_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_Red", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_BrownCheck", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_GreyCheck", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_Black", ExpansionHardlineItemRarity.Common);
		AddItem("FlatCap_BlackCheck", ExpansionHardlineItemRarity.Common);

		AddItem("ZmijovkaCap_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("ZmijovkaCap_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("ZmijovkaCap_Red", ExpansionHardlineItemRarity.Common);
		AddItem("ZmijovkaCap_Black", ExpansionHardlineItemRarity.Common);
		AddItem("ZmijovkaCap_Green", ExpansionHardlineItemRarity.Common);

		AddItem("RadarCap_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("RadarCap_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("RadarCap_Red", ExpansionHardlineItemRarity.Common);
		AddItem("RadarCap_Black", ExpansionHardlineItemRarity.Common);
		AddItem("RadarCap_Green", ExpansionHardlineItemRarity.Common);

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
		AddItem("ExpansionRidersJacketBrown", ExpansionHardlineItemRarity.Common);
		AddItem("ExpansionRidersJacketDarkBlue", ExpansionHardlineItemRarity.Common);
	#endif
		AddItem("RidersJacket_Black", ExpansionHardlineItemRarity.Common);

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
		AddItem("Crossbow_Autumn", ExpansionHardlineItemRarity.Rare);
		AddItem("Crossbow_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Crossbow_Summer", ExpansionHardlineItemRarity.Rare);
		AddItem("Crossbow_Wood", ExpansionHardlineItemRarity.Rare);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Crossbow", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Drinks
		AddItem("SodaCan_Pipsi", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SodaCan_Cola", ExpansionHardlineItemRarity.Rare);
		AddItem("SodaCan_Spite", ExpansionHardlineItemRarity.Rare);
		AddItem("SodaCan_Kvass", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SodaCan_Fronta", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WaterBottle", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Canteen", ExpansionHardlineItemRarity.Rare);
		AddItem("Vodka", ExpansionHardlineItemRarity.Uncommon);
	#ifdef EXPANSIONMOD
		AddItem("ExpansionMilkBottle", ExpansionHardlineItemRarity.Uncommon);
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

		//! Explosive Grenades
		AddItem("RGD5Grenade", ExpansionHardlineItemRarity.Epic);
		AddItem("M67Grenade", ExpansionHardlineItemRarity.Epic);

		AddItem("RemoteDetonator", ExpansionHardlineItemRarity.Epic);
		AddItem("RemoteDetonatorTrigger", ExpansionHardlineItemRarity.Epic);
		AddItem("ImprovisedExplosive", ExpansionHardlineItemRarity.Epic);
		AddItem("Plastic_Explosive", ExpansionHardlineItemRarity.Epic);
		AddItem("Grenade_ChemGas", ExpansionHardlineItemRarity.Epic);

		//! Flashbangs
		AddItem("FlashGrenade", ExpansionHardlineItemRarity.Rare);

		//! Smoke Grenades
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_M18SmokeGrenade_Teargas", ExpansionHardlineItemRarity.Rare);
	#endif
		AddItem("M18SmokeGrenade_Red", ExpansionHardlineItemRarity.Rare);
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

		//! Explosive Charges
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionSatchel", ExpansionHardlineItemRarity.Legendary);
	#endif
		AddItem("ClaymoreMine", ExpansionHardlineItemRarity.Epic);
		AddItem("LandMineTrap", ExpansionHardlineItemRarity.Epic);

		//! Eyewear
		AddItem("SportGlasses_Orange", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SportGlasses_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SportGlasses_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SportGlasses_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ThinFramesGlasses", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ThickFramesGlasses", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DesignerGlasses", ExpansionHardlineItemRarity.Uncommon);
		AddItem("AviatorGlasses", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TacticalGoggles", ExpansionHardlineItemRarity.Rare);
		AddItem("NVGHeadstrap", ExpansionHardlineItemRarity.Epic);
		AddItem("EyePatch_Improvised", ExpansionHardlineItemRarity.Common);
		AddItem("EyeMask_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_Christmas", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_Dead", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_NewYears", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_Valentines", ExpansionHardlineItemRarity.Uncommon);
		AddItem("EyeMask_Yellow", ExpansionHardlineItemRarity.Uncommon);

		//! Fish
		AddItem("CarpFilletMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MackerelFilletMeat", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Carp", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Sardines", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mackerel", ExpansionHardlineItemRarity.Uncommon);

		//! Fishing
		AddItem("Worm", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BoneHook", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Hook", ExpansionHardlineItemRarity.Uncommon);
		AddItem("FishingRod", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ImprovisedFishingRod", ExpansionHardlineItemRarity.Common);

		//! Flags
		AddItem("Flag_Chernarus", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Chedaki", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_NAPA", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_CDF", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Livonia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Altis", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_SSahrani", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_NSahrani", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_DayZ", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_LivoniaArmy", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Bohemia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_APA", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_UEC", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Pirates", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Cannibals", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Bear", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Wolf", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_BabyDeer", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Rooster", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_LivoniaPolice", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_CMC", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_TEC", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_CHEL", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Zenit", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_HunterZ", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_BrainZ", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Refuge", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_RSTA", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Snake", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Crook", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Rex", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Flag_Zagorky", ExpansionHardlineItemRarity.Uncommon);
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("Expansion_Flag_Expansion", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Australia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Canada", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Chernarus", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_France", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Germany", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Latvia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Luxembourg", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Mexico", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Netherlands", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_NewZealand", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Norway", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Poland", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Russia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_UK", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_USA", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Scotland", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Finland", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Sweden", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Spain", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Brazil", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Portugal", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Belgium", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Livonia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Japan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_China", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_SouthKorea", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_UN", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_NATO", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Pirate", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Chedaki", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_NAPA", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_CDF", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_NuevoRico", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Borduria", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Biohazard", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_AnyoneInCherno", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Ireland", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Wales", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Switzerland", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_SriLanka", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_SouthAfrica", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Sicily", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_OffWithHead", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Gibraltar", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Czechia", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Fari", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_DayZWhite", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_DayZBlack", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_DoubleAxe", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Grenade", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Yellow", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Orange", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Pink", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Purple", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Expansion_Flag_Rainbow", ExpansionHardlineItemRarity.Uncommon);
	#endif

		//! Food
		AddItem("Zagorky", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ZagorkyChocolate", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ZagorkyPeanuts", ExpansionHardlineItemRarity.Uncommon);

		AddItem("PowderedMilk", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BoxCerealCrunchin", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Rice", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Marmalade", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Honey", ExpansionHardlineItemRarity.Uncommon);

		AddItem("SaltySticks", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Crackers", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Chips", ExpansionHardlineItemRarity.Uncommon);

		AddItem("Pajka", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pate", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BrisketSpread", ExpansionHardlineItemRarity.Uncommon);

		AddItem("SardinesCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TunaCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DogFoodCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CatFoodCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PorkCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Lunchmeat", ExpansionHardlineItemRarity.Uncommon);

		AddItem("UnknownFoodCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PeachesCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SpaghettiCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BakedBeansCan", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TacticalBaconCan", ExpansionHardlineItemRarity.Uncommon);
		
		AddItem("BakedBeansCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PeachesCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SpaghettiCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SardinesCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TunaCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("UnknownFoodCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("DogFoodCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CatFoodCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PorkCan_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Lunchmeat_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pajka_Opened", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pate_Opened", ExpansionHardlineItemRarity.Uncommon);

	#ifdef EXPANSIONMOD
		AddItem("ExpansionBread1", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionBread2", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionBread3", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCheese1", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCheese2", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCheese3", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCheese4", ExpansionHardlineItemRarity.Uncommon);
	#endif

		//! Gardening
		AddItem("GardenLime", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PepperSeeds", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TomatoSeeds", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ZucchiniSeeds", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PumpkinSeeds", ExpansionHardlineItemRarity.Uncommon);

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
		AddItem("ExpansionTacticalGlovesDesert", ExpansionHardlineItemRarity.Rare);
	#endif
		AddItem("TacticalGloves_Black", ExpansionHardlineItemRarity.Rare);
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

		//! Holsters/Pouches
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

		//! Magazines
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
		AddItem("MimeMask_Female", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MimeMask_Male", ExpansionHardlineItemRarity.Uncommon);

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

		//! Navigation
		AddItem("Compass", ExpansionHardlineItemRarity.Uncommon);
		AddItem("OrienteeringCompass", ExpansionHardlineItemRarity.Uncommon);

		if (ExpansionStatic.GetCanonicalWorldName() == "namalsk")
		{
			AddItem("dzn_map_namalsk", ExpansionHardlineItemRarity.Uncommon);
		}
		else
		{
			AddItem("ChernarusMap", ExpansionHardlineItemRarity.Uncommon);
		}

		AddItem("Binoculars", ExpansionHardlineItemRarity.Rare);
		AddItem("GPSReceiver", ExpansionHardlineItemRarity.Rare);
	#ifdef EXPANSIONMODNAVIGATION
		AddItem("ExpansionGPS", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Optics
		AddItem("PistolOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("ReflexOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("M4_CarryHandleOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("BUISOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("FNP45_MRDSOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("ACOGOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("ACOGOptic_6x", ExpansionHardlineItemRarity.Rare);
		AddItem("M68Optic", ExpansionHardlineItemRarity.Rare);
		AddItem("M4_T3NRDSOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("KobraOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("KashtanOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("PUScopeOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("HuntingOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("PSO1Optic", ExpansionHardlineItemRarity.Rare);
		AddItem("PSO11Optic", ExpansionHardlineItemRarity.Rare);
		AddItem("KazuarOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("StarlightOptic", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_M1A_RailAtt", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_MP5_RailAtt", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionReflexMRSOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionDeltapointOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionEXPS3HoloOptic", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionHAMROptic", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_PMII25Optic", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionKar98ScopeOptic", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Pants
		AddItem("MedicalScrubsPants_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("MedicalScrubsPants_Green", ExpansionHardlineItemRarity.Common);
		AddItem("MedicalScrubsPants_White", ExpansionHardlineItemRarity.Common);

		AddItem("TrackSuitPants_Black", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitPants_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitPants_Green", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitPants_Red", ExpansionHardlineItemRarity.Common);
		AddItem("TrackSuitPants_LightBlue", ExpansionHardlineItemRarity.Common);

		AddItem("PrisonUniformPants", ExpansionHardlineItemRarity.Common);

		AddItem("Breeches_Pink", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_White", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Beetcheck", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Browncheck", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Black", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Blackcheck", ExpansionHardlineItemRarity.Common);
		AddItem("Breeches_Green", ExpansionHardlineItemRarity.Common);

		AddItem("SlacksPants_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_DarkGrey", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_LightGrey", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_White", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_Black", ExpansionHardlineItemRarity.Common);
		AddItem("SlacksPants_Khaki", ExpansionHardlineItemRarity.Common);

		AddItem("CanvasPantsMidi_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPantsMidi_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPantsMidi_Red", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPantsMidi_Violet", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPantsMidi_Beige", ExpansionHardlineItemRarity.Common);

		AddItem("CanvasPants_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPants_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPants_Red", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPants_Violet", ExpansionHardlineItemRarity.Common);
		AddItem("CanvasPants_Beige", ExpansionHardlineItemRarity.Common);

		AddItem("JumpsuitPants_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("JumpsuitPants_Green", ExpansionHardlineItemRarity.Common);
		AddItem("JumpsuitPants_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("JumpsuitPants_Red", ExpansionHardlineItemRarity.Common);

		AddItem("PolicePants", ExpansionHardlineItemRarity.Uncommon);

		AddItem("ParamedicPants_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("ParamedicPants_Crimson", ExpansionHardlineItemRarity.Common);
		AddItem("ParamedicPants_Green", ExpansionHardlineItemRarity.Common);

		AddItem("FirefightersPants_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("FirefightersPants_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CargoPants_Beige", ExpansionHardlineItemRarity.Rare);
		AddItem("CargoPants_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("CargoPants_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("CargoPants_Green", ExpansionHardlineItemRarity.Rare);
		AddItem("CargoPants_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("ShortJeans_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("ShortJeans_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("ShortJeans_Darkblue", ExpansionHardlineItemRarity.Common);
		AddItem("ShortJeans_Red", ExpansionHardlineItemRarity.Common);
		AddItem("ShortJeans_Black", ExpansionHardlineItemRarity.Common);
		AddItem("ShortJeans_Green", ExpansionHardlineItemRarity.Common);

		AddItem("Jeans_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Jeans_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("Jeans_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("Jeans_BlueDark", ExpansionHardlineItemRarity.Common);
		AddItem("Jeans_Green", ExpansionHardlineItemRarity.Common);
		AddItem("Jeans_Black", ExpansionHardlineItemRarity.Common);

		AddItem("TTSKOPants", ExpansionHardlineItemRarity.Rare);

		AddItem("BDUPants", ExpansionHardlineItemRarity.Rare);

		AddItem("USMCPants_Desert", ExpansionHardlineItemRarity.Rare);
		AddItem("USMCPants_Woodland", ExpansionHardlineItemRarity.Rare);

		AddItem("PolicePantsOrel", ExpansionHardlineItemRarity.Uncommon);

		AddItem("HunterPants_Winter", ExpansionHardlineItemRarity.Rare);
		AddItem("HunterPants_Autumn", ExpansionHardlineItemRarity.Rare);
		AddItem("HunterPants_Brown", ExpansionHardlineItemRarity.Rare);
		AddItem("HunterPants_Spring", ExpansionHardlineItemRarity.Rare);
		AddItem("HunterPants_Summer", ExpansionHardlineItemRarity.Rare);

		AddItem("GorkaPants_Summer", ExpansionHardlineItemRarity.Rare);
		AddItem("GorkaPants_Autumn", ExpansionHardlineItemRarity.Rare);
		AddItem("GorkaPants_Flat", ExpansionHardlineItemRarity.Rare);
		AddItem("GorkaPants_PautRev", ExpansionHardlineItemRarity.Rare);

		AddItem("NBCPantsGray", ExpansionHardlineItemRarity.Legendary);
		AddItem("NBCPantsYellow", ExpansionHardlineItemRarity.Legendary);

		AddItem("Chainmail_Leggings", ExpansionHardlineItemRarity.Exotic);

		//! Pistols
		AddItem("MakarovIJ70", ExpansionHardlineItemRarity.Rare);
		AddItem("Derringer_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Derringer_Grey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Derringer_Pink", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CZ75", ExpansionHardlineItemRarity.Rare);
		AddItem("FNX45", ExpansionHardlineItemRarity.Rare);
		AddItem("Glock19", ExpansionHardlineItemRarity.Rare);
		AddItem("MKII", ExpansionHardlineItemRarity.Rare);
		AddItem("Colt1911", ExpansionHardlineItemRarity.Rare);
		AddItem("Engraved1911", ExpansionHardlineItemRarity.Rare);
		AddItem("Magnum", ExpansionHardlineItemRarity.Rare);
		AddItem("Deagle", ExpansionHardlineItemRarity.Rare);
		AddItem("Deagle_Gold", ExpansionHardlineItemRarity.Rare);
		AddItem("P1", ExpansionHardlineItemRarity.Rare);
		AddItem("Longhorn", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_M9", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Rifles
		AddItem("Izh18", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Ruger1022", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Repeater", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Mosin9130", ExpansionHardlineItemRarity.Rare);
		AddItem("CZ527", ExpansionHardlineItemRarity.Rare);
		AddItem("CZ550", ExpansionHardlineItemRarity.Rare);
		AddItem("Winchester70", ExpansionHardlineItemRarity.Rare);
		AddItem("SSG82", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kar98", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_M14", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_M1A", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Shirts and T-Shirts
		AddItem("TShirt_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_OrangeWhiteStripes", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_Red", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_RedBlackStripes", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_Beige", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_Black", ExpansionHardlineItemRarity.Common);
		AddItem("TShirt_Green", ExpansionHardlineItemRarity.Common);

	#ifdef EXPASIONMOD
		AddItem("ExpansionTee", ExpansionHardlineItemRarity.Common);
	#endif

		AddItem("TelnyashkaShirt", ExpansionHardlineItemRarity.Common);

		AddItem("Shirt_BlueCheck", ExpansionHardlineItemRarity.Common);
		AddItem("Shirt_BlueCheckBright", ExpansionHardlineItemRarity.Common);
		AddItem("Shirt_RedCheck", ExpansionHardlineItemRarity.Common);
		AddItem("Shirt_WhiteCheck", ExpansionHardlineItemRarity.Common);
		AddItem("Shirt_PlaneBlack", ExpansionHardlineItemRarity.Common);
		AddItem("Shirt_GreenCheck", ExpansionHardlineItemRarity.Common);

		AddItem("MedicalScrubsShirt_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("MedicalScrubsShirt_Green", ExpansionHardlineItemRarity.Common);
		AddItem("MedicalScrubsShirt_White", ExpansionHardlineItemRarity.Common);

		AddItem("ChernarusSportShirt", ExpansionHardlineItemRarity.Common);

		AddItem("TacticalShirt_Grey", ExpansionHardlineItemRarity.Rare);
		AddItem("TacticalShirt_Tan", ExpansionHardlineItemRarity.Rare);
		AddItem("TacticalShirt_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("TacticalShirt_Olive", ExpansionHardlineItemRarity.Rare);

		//! Shotguns
		AddItem("Mp133Shotgun", ExpansionHardlineItemRarity.Rare);
		AddItem("Izh43Shotgun", ExpansionHardlineItemRarity.Rare);
		AddItem("Izh18Shotgun", ExpansionHardlineItemRarity.Rare);
		AddItem("Saiga", ExpansionHardlineItemRarity.Rare);
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_BenelliM4", ExpansionHardlineItemRarity.Epic);
		AddItem("Expansion_DT11", ExpansionHardlineItemRarity.Epic);
	#endif

		//! Skirts and Dresses
		AddItem("Skirt_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Skirt_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Skirt_White", ExpansionHardlineItemRarity.Common);
		AddItem("Skirt_Yellow", ExpansionHardlineItemRarity.Common);

		AddItem("MiniDress_Pink", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_PinkChecker", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_RedChecker", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_BlueChecker", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_BlueWithDots", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_WhiteChecker", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_BrownChecker", ExpansionHardlineItemRarity.Common);
		AddItem("MiniDress_GreenChecker", ExpansionHardlineItemRarity.Common);

		AddItem("NurseDress_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("NurseDress_White", ExpansionHardlineItemRarity.Common);

		//! Sniper Rifles
		AddItem("VSS", ExpansionHardlineItemRarity.Rare);
		AddItem("ASVAL", ExpansionHardlineItemRarity.Rare);
		AddItem("B95", ExpansionHardlineItemRarity.Rare);
		AddItem("SVD", ExpansionHardlineItemRarity.Rare);

		if (ExpansionStatic.GetCanonicalWorldName() == "enoch")
		{
			AddItem("Scout_Livonia", ExpansionHardlineItemRarity.Rare);
		}
		else if (ExpansionStatic.GetCanonicalWorldName() == "chernarusplus")
		{
			AddItem("Scout_Chernarus", ExpansionHardlineItemRarity.Rare);
		}
		else
		{
			AddItem("Scout", ExpansionHardlineItemRarity.Rare);
		}
	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_AWM", ExpansionHardlineItemRarity.Epic);
	#endif

		//! Submachine-Guns
		AddItem("CZ61", ExpansionHardlineItemRarity.Rare);
		AddItem("UMP45", ExpansionHardlineItemRarity.Rare);
		AddItem("MP5K", ExpansionHardlineItemRarity.Rare);
		AddItem("AKS74U", ExpansionHardlineItemRarity.Rare);
		AddItem("PP19", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODWEAPONS
		AddItem("Expansion_Kedr", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_MPX", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_MP5", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_MP5SD", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_MP7", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_VityazSN", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Supply Items
		AddItem("Paper", ExpansionHardlineItemRarity.Common);
		AddItem("Pen_Black", ExpansionHardlineItemRarity.Common);
		AddItem("Pen_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Pen_Green", ExpansionHardlineItemRarity.Common);
		AddItem("Pen_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Rope", ExpansionHardlineItemRarity.Uncommon);
		AddItem("TannedLeather", ExpansionHardlineItemRarity.Uncommon);
		AddItem("AntiPestsSpray", ExpansionHardlineItemRarity.Rare);
		AddItem("MetalWire", ExpansionHardlineItemRarity.Rare);
		AddItem("EpoxyPutty", ExpansionHardlineItemRarity.Rare);
		AddItem("DuctTape", ExpansionHardlineItemRarity.Rare);
		AddItem("Pot", ExpansionHardlineItemRarity.Rare);
		AddItem("HandcuffKeys", ExpansionHardlineItemRarity.Rare);
		AddItem("Handcuffs", ExpansionHardlineItemRarity.Rare);
		AddItem("Netting", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BurlapSack", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WoodenPlank", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MetalPlate", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SmallGasCanister", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MediumGasCanister", ExpansionHardlineItemRarity.Uncommon);
		AddItem("LargeGasCanister", ExpansionHardlineItemRarity.Uncommon);
		AddItem("NailBox", ExpansionHardlineItemRarity.Rare);
		AddItem("Nail", ExpansionHardlineItemRarity.Rare);
		AddItem("BarbedWire", ExpansionHardlineItemRarity.Rare);
		AddItem("Fabric", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Camonet", ExpansionHardlineItemRarity.Rare);
		AddItem("HescoBox", ExpansionHardlineItemRarity.Rare);
		AddItem("PowerGenerator", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODBASEBUILDING
		//AddItem("ExpansionLumber0_5", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionLumber1", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionLumber1_5", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionLumber3", ExpansionHardlineItemRarity.Uncommon);

		//! For the BaseBuilding update
		//AddItem("ExpansionCement", ExpansionHardlineItemRarity.Uncommon);
		//AddItem("ExpansionBricks", ExpansionHardlineItemRarity.Uncommon);

		//AddItem("ExpansionMixerKit", ExpansionHardlineItemRarity.Rare);
		//AddItem("ExpansionMixerTank", ExpansionHardlineItemRarity.Rare);
		//AddItem("ExpansionMixerMotor", ExpansionHardlineItemRarity.Rare);
		//AddItem("ExpansionMixerHandle", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Sweaters and Hoodies
		AddItem("Sweater_Gray", ExpansionHardlineItemRarity.Common);
		AddItem("Sweater_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Sweater_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Sweater_Green", ExpansionHardlineItemRarity.Common);

		AddItem("Hoodie_Blue", ExpansionHardlineItemRarity.Common);
		AddItem("Hoodie_Black", ExpansionHardlineItemRarity.Common);
		AddItem("Hoodie_Brown", ExpansionHardlineItemRarity.Common);
		AddItem("Hoodie_Grey", ExpansionHardlineItemRarity.Common);
		AddItem("Hoodie_Red", ExpansionHardlineItemRarity.Common);
		AddItem("Hoodie_Green", ExpansionHardlineItemRarity.Common);

		//! Tents
		AddItem("PartyTent", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PartyTent_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PartyTent_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PartyTent_Lunapark", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MediumTent", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MediumTent_Orange", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MediumTent_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("LargeTent", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CarTent", ExpansionHardlineItemRarity.Uncommon);

	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionCamoTentKit", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionCamoBoxKit", ExpansionHardlineItemRarity.Uncommon);
	#endif

		//! Tools
		AddItem("Screwdriver", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Wrench", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pliers", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Whetstone", ExpansionHardlineItemRarity.Rare);
		AddItem("Hammer", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Hacksaw", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HandSaw", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CanOpener", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Hatchet", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Lockpick", ExpansionHardlineItemRarity.Rare);

		AddItem("LugWrench", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PipeWrench", ExpansionHardlineItemRarity.Uncommon);
		AddItem("FryingPan", ExpansionHardlineItemRarity.Rare);
		AddItem("Sickle", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Crowbar", ExpansionHardlineItemRarity.Rare);
		AddItem("Shovel", ExpansionHardlineItemRarity.Rare);
		AddItem("Pickaxe", ExpansionHardlineItemRarity.Rare);
		AddItem("SledgeHammer", ExpansionHardlineItemRarity.Uncommon);
		AddItem("FarmingHoe", ExpansionHardlineItemRarity.Uncommon);
		AddItem("WoodAxe", ExpansionHardlineItemRarity.Rare);
		AddItem("FirefighterAxe", ExpansionHardlineItemRarity.Rare);
		AddItem("Pitchfork", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Blowtorch", ExpansionHardlineItemRarity.Rare);
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionPropaneTorch", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionBoltCutters", ExpansionHardlineItemRarity.Rare);
	#endif

		//! Vegetables
		AddItem("Apple", ExpansionHardlineItemRarity.Uncommon);
		AddItem("GreenBellPepper", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Zucchini", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pumpkin", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SlicedPumpkin", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PotatoSeed", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Potato", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Tomato", ExpansionHardlineItemRarity.Uncommon);
		AddItem("SambucusBerry", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CaninaBerry", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Plum", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Pear", ExpansionHardlineItemRarity.Uncommon);
		//AddItem("Kiwi", ExpansionHardlineItemRarity.Uncommon);
		//AddItem("Orange", ExpansionHardlineItemRarity.Uncommon);
		//AddItem("Banana", ExpansionHardlineItemRarity.Uncommon);

		AddItem("AgaricusMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("AmanitaMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("MacrolepiotaMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("LactariusMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PsilocybeMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("AuriculariaMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("BoletusMushroom", ExpansionHardlineItemRarity.Uncommon);
		AddItem("PleurotusMushroom", ExpansionHardlineItemRarity.Uncommon);

		//! Vehicle parts
		AddItem("HeadlightH7_Box", ExpansionHardlineItemRarity.Uncommon);
		AddItem("HeadlightH7", ExpansionHardlineItemRarity.Uncommon);
		AddItem("CarRadiator", ExpansionHardlineItemRarity.Rare);

		AddItem("TireRepairKit", ExpansionHardlineItemRarity.Rare);
		AddItem("SparkPlug", ExpansionHardlineItemRarity.Rare);
		AddItem("GlowPlug", ExpansionHardlineItemRarity.Rare);
	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionIgniterPlug", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionHydraulicHoses", ExpansionHardlineItemRarity.Rare);
	#endif

		AddItem("HatchbackHood", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackHood_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackHood_White", ExpansionHardlineItemRarity.Rare);

		AddItem("HatchbackTrunk", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackTrunk_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackTrunk_White", ExpansionHardlineItemRarity.Rare);

		AddItem("HatchbackDoors_Driver", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackDoors_Driver_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackDoors_Driver_White", ExpansionHardlineItemRarity.Rare);

		AddItem("HatchbackDoors_CoDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackDoors_CoDriver_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("HatchbackDoors_CoDriver_White", ExpansionHardlineItemRarity.Rare);

		AddItem("HatchbackWheel", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Hood", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Hood_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Hood_Blue", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Trunk", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Trunk_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Trunk_Blue", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Door_1_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_1_1_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_1_1_Blue", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Door_1_2", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_1_2_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_1_2_Blue", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Door_2_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_2_1_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_2_1_Blue", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Door_2_2", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_2_2_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Hatchback_02_Door_2_2_Blue", ExpansionHardlineItemRarity.Rare);

		AddItem("Hatchback_02_Wheel", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanHood", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanHood_Wine", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanHood_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanTrunk", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanTrunk_Wine", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanTrunk_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanDoors_Driver", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_Driver_Wine", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_Driver_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanDoors_CoDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_CoDriver_Wine", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_CoDriver_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanDoors_BackLeft", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_BackLeft_Wine", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_BackLeft_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanDoors_BackRight", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_BackRight_Wine", ExpansionHardlineItemRarity.Rare);
		AddItem("CivSedanDoors_BackRight_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("CivSedanWheel", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Hood", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Hood_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Hood_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Trunk", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Trunk_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Trunk_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Door_1_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_1_1_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_1_1_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Door_1_2", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_1_2_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_1_2_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Door_2_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_2_1_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_2_1_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Door_2_2", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_2_2_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Sedan_02_Door_2_2_Grey", ExpansionHardlineItemRarity.Rare);

		AddItem("Sedan_02_Wheel", ExpansionHardlineItemRarity.Rare);

		AddItem("Truck_01_Hood", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_Hood_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_Hood_Orange", ExpansionHardlineItemRarity.Rare);

		AddItem("Truck_01_Door_1_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_Door_1_1_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_Door_1_1_Orange", ExpansionHardlineItemRarity.Rare);

		AddItem("Truck_01_Door_2_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_Door_2_1_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_Door_2_1_Orange", ExpansionHardlineItemRarity.Rare);

		AddItem("Truck_01_Wheel", ExpansionHardlineItemRarity.Rare);
		AddItem("Truck_01_WheelDouble", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Hood", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Trunk", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Door_1_1", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Door_1_2", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Door_2_1", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Door_2_2", ExpansionHardlineItemRarity.Rare);

		AddItem("Offroad_02_Wheel", ExpansionHardlineItemRarity.Rare);

	#ifdef EXPANSIONMODVEHICLE
		AddItem("ExpansionTractorDoorsDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionTractorDoorsCodriver", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionTractorBackWheel", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionTractorFrontWheel", ExpansionHardlineItemRarity.Rare);

		AddItem("ExpansionUAZDoorHood", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUAZDoorDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUAZDoorCoDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUAZDoorCargo1", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUAZDoorCargo2", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUAZWheel", ExpansionHardlineItemRarity.Rare);

		AddItem("Expansion_Landrover_Hood", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Landrover_Driverdoor", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Landrover_Codriverdoor", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Landrover_Left", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Landrover_Right", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Landrover_Trunk", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Landrover_Wheel", ExpansionHardlineItemRarity.Rare);

		AddItem("ExpansionVodnikDoorDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionVodnikDoorCoDriver", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionVodnikWheel", ExpansionHardlineItemRarity.Rare);

		AddItem("ExpansionMerlinBackWheel", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionMerlinFrontWheel", ExpansionHardlineItemRarity.Rare);

		AddItem("ExpansionUh1hDoor_1_1", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUh1hDoor_1_2", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUh1hDoor_2_1", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionUh1hDoor_2_2", ExpansionHardlineItemRarity.Rare);

		AddItem("Expansion_Mh6_Door_1_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_1_1_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_1_2", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_1_2_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_2_1", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_2_1_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_2_2", ExpansionHardlineItemRarity.Rare);
		AddItem("Expansion_Mh6_Door_2_2_Black", ExpansionHardlineItemRarity.Rare);

		AddItem("ExpansionBusWheel", ExpansionHardlineItemRarity.Rare);
		AddItem("ExpansionBusWheelDouble", ExpansionHardlineItemRarity.Rare);

		AddItem("ExpansionCarKey", ExpansionHardlineItemRarity.Epic);

		AddItem("ExpansionKeyChain_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Grey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Orange", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Pink", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Purp", ExpansionHardlineItemRarity.Uncommon);
		AddItem("ExpansionKeyChain_Yellow", ExpansionHardlineItemRarity.Uncommon);
	#endif

		//! Vests
		AddItem("ReflexVest", ExpansionHardlineItemRarity.Uncommon);

		AddItem("PoliceVest", ExpansionHardlineItemRarity.Rare);

		AddItem("PressVest_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("PressVest_LightBlue", ExpansionHardlineItemRarity.Rare);

		AddItem("UKAssVest_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("UKAssVest_Khaki", ExpansionHardlineItemRarity.Rare);
		AddItem("UKAssVest_Olive", ExpansionHardlineItemRarity.Rare);
		AddItem("UKAssVest_Camo", ExpansionHardlineItemRarity.Rare);

		AddItem("SmershVest", ExpansionHardlineItemRarity.Rare);

		AddItem("HighCapacityVest_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("HighCapacityVest_Olive", ExpansionHardlineItemRarity.Rare);

		AddItem("PlateCarrierVest", ExpansionHardlineItemRarity.Epic);
		AddItem("PlateCarrierVest_Green", ExpansionHardlineItemRarity.Epic);
		AddItem("PlateCarrierVest_Black", ExpansionHardlineItemRarity.Epic);
		AddItem("PlateCarrierVest_Camo", ExpansionHardlineItemRarity.Epic);
		AddItem("HuntingVest", ExpansionHardlineItemRarity.Rare);

		AddItem("Chestplate", ExpansionHardlineItemRarity.Epic);
		
	#ifdef EXPANSION_NAMALSK_ADVENTURE
		//! Anomaly Cores
		AddItem("Expansion_AnomalyCore_Ice", ExpansionHardlineItemRarity.Exotic);
		AddItem("Expansion_AnomalyCore_Fire", ExpansionHardlineItemRarity.Exotic);
		AddItem("Expansion_AnomalyCore_Warper", ExpansionHardlineItemRarity.Exotic);
		
		//! Circuit-Boards
		AddItem("ExpansionCircuitBoard_MK1", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionCircuitBoard_MK2", ExpansionHardlineItemRarity.Legendary);
		AddItem("ExpansionCircuitBoard_MK3", ExpansionHardlineItemRarity.Legendary);
		
		//! Misc
		AddItem("ExpansionAnomalyCoreProtectiveCase", ExpansionHardlineItemRarity.Legendary);
		AddItem("AmmoCleaningKit", ExpansionHardlineItemRarity.Rare);
		AddItem("HuntingBag_Hannah", ExpansionHardlineItemRarity.Epic);
		
		//! Quest Items
		AddItem("MedicalSupplies", ExpansionHardlineItemRarity.Quest);
		AddItem("AmmunitionSupplies", ExpansionHardlineItemRarity.Quest);
		AddItem("CommunityGoalTicket", ExpansionHardlineItemRarity.Legendary);
		
		//! Keycards
		AddItem("Expansion_KeyCard_NA_Antenna", ExpansionHardlineItemRarity.Mythic);
		AddItem("Expansion_KeyCard_A1_B1", ExpansionHardlineItemRarity.Mythic);
		AddItem("Expansion_KeyCard_Teleporter", ExpansionHardlineItemRarity.Mythic);
		
		//! Keys
		AddItem("Expansion_SupplyCrate_Bunker_Key", ExpansionHardlineItemRarity.Mythic);
		AddItem("Expansion_SupplyCrate_WorldT1_Key", ExpansionHardlineItemRarity.Mythic);
		AddItem("Expansion_Bunker_Generator_Key", ExpansionHardlineItemRarity.Mythic);
		
		//! Ammo
		AddItem("Expansion_Ammo_BoltAnomaly_Ice", ExpansionHardlineItemRarity.Exotic);
		AddItem("Expansion_Ammo_BoltAnomaly_Warper", ExpansionHardlineItemRarity.Exotic);
		
		//! Windstrides
	#ifdef WindstrideClothing
		AddItem("Poncho_Red_Checkered", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_Blue_Checkered", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_Yellow_Checkered", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_Black", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_BlackAndWhite", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Poncho_BlackAndWhite2", ExpansionHardlineItemRarity.Uncommon);
		
		AddItem("Blue_Leggings", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Latex_Leggings", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Latex_White_Leggings", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Adidas_Leggings", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Red_Leggings", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Purple_Leggings", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Fleece_Grey_Leggings", ExpansionHardlineItemRarity.Uncommon);
		
		AddItem("Winter_Parka_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Winter_Parka_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Military_Sweater", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Military_Sweater_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Military_Sweater_Blue", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Military_Sweater_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Military_Sweater_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Military_Sweater_Black", ExpansionHardlineItemRarity.Uncommon);
		
		AddItem("Fingerless_Gloves", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Fingerless_Gloves_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Fingerless_Gloves_Wool", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Fingerless_Gloves_Wool_Grey", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Fingerless_Gloves_Wool_Black", ExpansionHardlineItemRarity.Uncommon);
		
		AddItem("Shemagh_Brown", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Shemagh_Green", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Shemagh_Red", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Shemagh_White", ExpansionHardlineItemRarity.Uncommon);
		AddItem("Shemagh_Black", ExpansionHardlineItemRarity.Uncommon);
		
		AddItem("Canvas_Backpack_Black", ExpansionHardlineItemRarity.Rare);
		AddItem("Canvas_Backpack_White", ExpansionHardlineItemRarity.Rare);
		AddItem("Canvas_Backpack_Red", ExpansionHardlineItemRarity.Rare);
		AddItem("Canvas_Backpack_Blue", ExpansionHardlineItemRarity.Rare);
		AddItem("Canvas_Backpack_Purple", ExpansionHardlineItemRarity.Rare);
		
		AddItem("HumanSkull", ExpansionHardlineItemRarity.Poor);
		AddItem("SkullMask", ExpansionHardlineItemRarity.Poor);
	#endif
	#endif
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

		return DefaultItemRatity;
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