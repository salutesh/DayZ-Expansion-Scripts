/**
 * ExpansionP2PMarketSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuCategoryBase
{
	protected string DisplayName;
	protected string IconPath;
	protected ref TStringArray Included;
	protected ref TStringArray Excluded;

	void ExpansionP2PMarketMenuCategoryBase()
	{
		Included = new TStringArray;
		Excluded = new TStringArray;
	}

	void SetDisplayName(string name)
	{
		DisplayName = name;
	}

	string GetDisplayName()
	{
		return DisplayName;
	}

	void SetIconPath(string path)
	{
		IconPath = path;
	}

	string GetIconPath()
	{
		return IconPath;
	}

	void AddIncluded(string typeName)
	{
		Included.Insert(typeName);
	}

	TStringArray GetIncluded()
	{
		return Included;
	}

	void AddExcluded(string typeName)
	{
		Excluded.Insert(typeName);
	}

	TStringArray GetExcluded()
	{
		return Excluded;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(DisplayName);
		ctx.Write(IconPath);

		int i;
		int includedCount = Included.Count();
		ctx.Write(includedCount);
		for (i = 0; i < Included.Count(); i++)
		{
			string includedTypeName = Included[i];
			ctx.Write(includedTypeName);
		}

		int excludedCount = Excluded.Count();
		ctx.Write(excludedCount);
		for (i = 0; i < Excluded.Count(); i++)
		{
			string excludedTypeName = Excluded[i];
			ctx.Write(excludedTypeName);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		ctx.Read(DisplayName);
		ctx.Read(IconPath);

		int i;
		int includedCount;
		ctx.Read(includedCount);

		if (includedCount > 0)
		{
			if (!Included)
				Included = new TStringArray;
			else
				Included.Clear();

			for (i = 0; i < includedCount; i++)
			{
				string includedTypeName;
				ctx.Read(includedTypeName);
				Included.Insert(includedTypeName);
			}
		}

		int excludedCount;
		ctx.Read(excludedCount);

		if (excludedCount > 0)
		{
			if (!Excluded)
				Excluded = new TStringArray;
			else
				Excluded.Clear();

			for (i = 0; i < excludedCount; i++)
			{
				string excludedTypeName;
				ctx.Read(excludedTypeName);
				Excluded.Insert(excludedTypeName);
			}
		}

		return true;
	}
};

class ExpansionP2PMarketMenuCategory: ExpansionP2PMarketMenuCategoryBase
{
	protected ref array<ref ExpansionP2PMarketMenuSubCategory> SubCategories;

	void ExpansionP2PMarketMenuCategory()
	{
		Included = new TStringArray;
		Excluded = new TStringArray;
		SubCategories = new array<ref ExpansionP2PMarketMenuSubCategory>;
	}

	void AddSubCategory(ExpansionP2PMarketMenuSubCategory category)
	{
		SubCategories.Insert(category);
	}

	array<ref ExpansionP2PMarketMenuSubCategory> GetSubCategories()
	{
		return SubCategories;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		int subCategoriesCount = SubCategories.Count();
		ctx.Write(subCategoriesCount);
		for (int i = 0; i < SubCategories.Count(); i++)
		{
			SubCategories[i].OnSend(ctx);
		}
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		super.OnRecieve(ctx);

		int subCategoriesCount;
		ctx.Read(subCategoriesCount);

		if (subCategoriesCount > 0)
		{
			if (!SubCategories)
				SubCategories = new array<ref ExpansionP2PMarketMenuSubCategory>;
			else
				SubCategories.Clear();

			for (int i = 0; i < subCategoriesCount; i++)
			{
				ExpansionP2PMarketMenuSubCategory subCategory = new ExpansionP2PMarketMenuSubCategory();
				subCategory.OnRecieve(ctx);
				SubCategories.Insert(subCategory);
			}
		}

		return true;
	}
};

class ExpansionP2PMarketMenuSubCategory: ExpansionP2PMarketMenuCategoryBase {};

class ExpansionP2PMarketSettingsBase: ExpansionSettingBase
{
	bool Enabled;
	int MaxListingTime;
	int MaxListings;
	int ListingOwnerDiscountPercent;
	int ListingPricePercent;
	ref TStringArray ExcludedClassNames;
	ref array<ref ExpansionP2PMarketMenuCategory> MenuCategories;
};

class ExpansionP2PMarketSettings: ExpansionP2PMarketSettingsBase
{
	static const int VERSION = 3;

	int SalesDepositTime;
	bool DisallowUnpersisted;

	[NonSerialized()]
	bool m_IsLoaded;

	void ExpansionP2PMarketSettings()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionP2PMarketSettings");
	#endif

		ExcludedClassNames = new TStringArray;
		MenuCategories = new array<ref ExpansionP2PMarketMenuCategory>;
	}

	override bool OnRecieve( ParamsReadContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
	#endif

		ctx.Read(Enabled);
		ctx.Read(MaxListingTime);
		ctx.Read(MaxListings);
		ctx.Read(ListingOwnerDiscountPercent);
		ctx.Read(ListingPricePercent);
		ctx.Read(ExcludedClassNames);
		ctx.Read(SalesDepositTime);

		int menuCatergoriesCount;
		ctx.Read(menuCatergoriesCount);

		if (menuCatergoriesCount > 0)
		{
			if (!MenuCategories)
				MenuCategories = new array<ref ExpansionP2PMarketMenuCategory>;
			else
				MenuCategories.Clear();

			for (int i = 0; i < menuCatergoriesCount; i++)
			{
				ExpansionP2PMarketMenuCategory menuCategory = new ExpansionP2PMarketMenuCategory();
				menuCategory.OnRecieve(ctx);
				MenuCategories.Insert(menuCategory);
			}
		}

		m_IsLoaded = true;

		ExpansionSettings.SI_P2PMarket.Invoke();

		return true;
	}

	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
	#endif

		ctx.Write(Enabled);
		ctx.Write(MaxListingTime);
		ctx.Write(MaxListings);
		ctx.Write(ListingOwnerDiscountPercent);
		ctx.Write(ListingPricePercent);
		ctx.Write(ExcludedClassNames);
		ctx.Write(SalesDepositTime);

		int menuCatergoriesCount = MenuCategories.Count();
		ctx.Write(menuCatergoriesCount);
		for (int i = 0; i < MenuCategories.Count(); i++)
		{
			MenuCategories[i].OnSend(ctx);
		}
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

		auto rpc = CreateRPC();
		OnSend( rpc );
		rpc.Expansion_Send(true, identity);

		return 0;
	}

	override bool Copy( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
	#endif

		ExpansionP2PMarketSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	void CopyInternal(ExpansionP2PMarketSettings s)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		SalesDepositTime = s.SalesDepositTime;
		DisallowUnpersisted = s.DisallowUnpersisted;

		ExpansionP2PMarketSettingsBase sb = s;
		CopyInternal(sb);
	}

	void CopyInternal(ExpansionP2PMarketSettingsBase s)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		Enabled = s.Enabled;
		MaxListingTime = s.MaxListingTime;
		MaxListings = s.MaxListings;
		ListingOwnerDiscountPercent = s.ListingOwnerDiscountPercent;
		ListingPricePercent = s.ListingPricePercent;
		ExcludedClassNames = s.ExcludedClassNames;
		MenuCategories = s.MenuCategories;
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
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
	#endif

		m_IsLoaded = true;

		bool save;
		bool bmSettingsExist = FileExist(EXPANSION_P2PMARKET_SETTINGS);
		if (bmSettingsExist)
		{
			ExpansionP2PMarketSettings settingsDefault = new ExpansionP2PMarketSettings;
			settingsDefault.Defaults();

			ExpansionP2PMarketSettingsBase settingsBase;
			JsonFileLoader<ExpansionP2PMarketSettingsBase>.JsonLoadFile(EXPANSION_P2PMARKET_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionP2PMarketSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_P2PMARKET_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 1)
				{
					CopyInternal(settingsBase);
					SalesDepositTime = settingsDefault.SalesDepositTime;
				}
				else
				{
					JsonFileLoader<ExpansionP2PMarketSettings>.JsonLoadFile(EXPANSION_P2PMARKET_SETTINGS, this);
					DisallowUnpersisted = settingsDefault.DisallowUnpersisted;
				}

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionP2PMarketSettings>.JsonLoadFile(EXPANSION_P2PMARKET_SETTINGS, this);
			}
		}
		else
		{
			CF_Log.Info("[ExpansionP2PMarketSettings] No existing setting file:" + EXPANSION_P2PMARKET_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (save)
		{
			Save();
		}

		return bmSettingsExist;
	}

	override bool OnSave()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnSave");
	#endif

		JsonFileLoader<ExpansionP2PMarketSettings>.JsonSaveFile(EXPANSION_P2PMARKET_SETTINGS, this);

		return true;
	}

	override void Update( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_P2PMarket.Invoke();
	}

	override void Defaults()
	{
		m_Version = VERSION;

		Enabled = true;
		MaxListingTime = 691200; //! 8 days by default.
		MaxListings = 20; //! Max. listings a player can create.
		ListingOwnerDiscountPercent = 70; //! 70% discount on the listed item price for the item owner.
		ListingPricePercent = 30; //! 30% of the given listing price need to be paied to list a item.
		SalesDepositTime = 691200; //! 8 days by default.
		DisallowUnpersisted = false;

		DefaultMenuCategories();

		DefaultExcludedClassNames();
	}

	protected void DefaultMenuCategories()
	{
		//! Ammo
		ExpansionP2PMarketMenuCategory cat_Ammo = new ExpansionP2PMarketMenuCategory();
		cat_Ammo.SetDisplayName("Ammo");
		cat_Ammo.SetIconPath("set:dayz_inventory image:bullets");
		cat_Ammo.AddIncluded("Ammunition_Base");
		cat_Ammo.AddIncluded("Box_Base");
		MenuCategories.Insert(cat_Ammo);

		//! Ammo - Ammo-Boxes
		ExpansionP2PMarketMenuSubCategory sub_Cat_Ammo_Boxes = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Ammo_Boxes.SetDisplayName("Ammo-Boxes");
		sub_Cat_Ammo_Boxes.SetIconPath("set:dayz_inventory image:bullets");
		sub_Cat_Ammo_Boxes.AddIncluded("Box_Base");
		cat_Ammo.AddSubCategory(sub_Cat_Ammo_Boxes);

		//! Ammo - Bullets
		ExpansionP2PMarketMenuSubCategory sub_Cat_Ammo_Bullets = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Ammo_Bullets.SetDisplayName("Ammo-Bullets");
		sub_Cat_Ammo_Bullets.SetIconPath("set:dayz_inventory image:bullets");
		sub_Cat_Ammo_Bullets.AddIncluded("Ammunition_Base");
		cat_Ammo.AddSubCategory(sub_Cat_Ammo_Bullets);

		//! Containers
		ExpansionP2PMarketMenuCategory cat_Containers = new ExpansionP2PMarketMenuCategory();
		cat_Containers.SetDisplayName("Containers");
		cat_Containers.SetIconPath("set:dayz_inventory image:barrel");
		cat_Containers.AddIncluded("Container_Base");
		MenuCategories.Insert(cat_Containers);

		//! Consumables
		ExpansionP2PMarketMenuCategory cat_Consumables = new ExpansionP2PMarketMenuCategory();
		cat_Consumables.SetDisplayName("Consumables");
		cat_Consumables.SetIconPath("set:dayz_inventory image:food");
		cat_Consumables.AddIncluded("Edible_Base");
		MenuCategories.Insert(cat_Consumables);

		//! Consumables - Drinks
		ExpansionP2PMarketMenuSubCategory sub_Cat_Consumables_Drinks = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Consumables_Drinks.SetDisplayName("Drinks");
		sub_Cat_Consumables_Drinks.SetIconPath("set:dayz_inventory image:canteen");
		sub_Cat_Consumables_Drinks.AddIncluded("SodaCan_ColorBase");
		sub_Cat_Consumables_Drinks.AddIncluded("WaterBottle");
		sub_Cat_Consumables_Drinks.AddIncluded("Canteen");
		sub_Cat_Consumables_Drinks.AddIncluded("Vodka");
	#ifdef EXPANSIONMOD
		sub_Cat_Consumables_Drinks.AddIncluded("ExpansionMilkBottle");
	#endif
		cat_Consumables.AddSubCategory(sub_Cat_Consumables_Drinks);

		//! Consumables - Fish
		ExpansionP2PMarketMenuSubCategory sub_Cat_Consumables_Fish = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Consumables_Fish.SetDisplayName("Fish");
		sub_Cat_Consumables_Fish.SetIconPath("set:dayz_inventory image:food");
		sub_Cat_Consumables_Fish.AddIncluded("CarpFilletMeat");
		sub_Cat_Consumables_Fish.AddIncluded("MackerelFilletMeat");
		sub_Cat_Consumables_Fish.AddIncluded("Carp");
		sub_Cat_Consumables_Fish.AddIncluded("Sardines");
		sub_Cat_Consumables_Fish.AddIncluded("Mackerel");
		cat_Consumables.AddSubCategory(sub_Cat_Consumables_Fish);

		//! Consumables - Food
		ExpansionP2PMarketMenuSubCategory sub_Cat_Consumables_Food = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Consumables_Food.SetDisplayName("Food");
		sub_Cat_Consumables_Food.SetIconPath("set:dayz_inventory image:food");
		sub_Cat_Consumables_Food.AddIncluded("Zagorky");
		sub_Cat_Consumables_Food.AddIncluded("ZagorkyChocolate");
		sub_Cat_Consumables_Food.AddIncluded("ZagorkyPeanuts");
		sub_Cat_Consumables_Food.AddIncluded("PowderedMilk");
		sub_Cat_Consumables_Food.AddIncluded("BoxCerealCrunchin");
		sub_Cat_Consumables_Food.AddIncluded("Rice");
		sub_Cat_Consumables_Food.AddIncluded("Marmalade");
		sub_Cat_Consumables_Food.AddIncluded("Honey");
		sub_Cat_Consumables_Food.AddIncluded("SaltySticks");
		sub_Cat_Consumables_Food.AddIncluded("Crackers");
		sub_Cat_Consumables_Food.AddIncluded("Chips");
		sub_Cat_Consumables_Food.AddIncluded("Pajka");
		sub_Cat_Consumables_Food.AddIncluded("Pate");
		sub_Cat_Consumables_Food.AddIncluded("BrisketSpread");
		sub_Cat_Consumables_Food.AddIncluded("SardinesCan");
		sub_Cat_Consumables_Food.AddIncluded("TunaCan");
		sub_Cat_Consumables_Food.AddIncluded("DogFoodCan");
		sub_Cat_Consumables_Food.AddIncluded("CatFoodCan");
		sub_Cat_Consumables_Food.AddIncluded("PorkCan");
		sub_Cat_Consumables_Food.AddIncluded("Lunchmeat");
		sub_Cat_Consumables_Food.AddIncluded("UnknownFoodCan");
		sub_Cat_Consumables_Food.AddIncluded("PeachesCan");
		sub_Cat_Consumables_Food.AddIncluded("SpaghettiCan");
		sub_Cat_Consumables_Food.AddIncluded("BakedBeansCan");
		sub_Cat_Consumables_Food.AddIncluded("TacticalBaconCan");
		cat_Consumables.AddSubCategory(sub_Cat_Consumables_Food);

		//! Consumables - Meat
		ExpansionP2PMarketMenuSubCategory sub_Cat_Consumables_Meat = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Consumables_Meat.SetDisplayName("Meat");
		sub_Cat_Consumables_Meat.SetIconPath("set:dayz_inventory image:food");
		sub_Cat_Consumables_Meat.AddIncluded("BearSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("GoatSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("BoarSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("PigSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("DeerSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("WolfSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("CowSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("SheepSteakMeat");
		sub_Cat_Consumables_Meat.AddIncluded("ChickenBreastMeat");
		sub_Cat_Consumables_Meat.AddIncluded("RabbitLegMeat");
		cat_Consumables.AddSubCategory(sub_Cat_Consumables_Meat);

		//! Consumables - Meds
		ExpansionP2PMarketMenuSubCategory sub_Cat_Consumables_Meds = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Consumables_Meds.SetDisplayName("Meds");
		sub_Cat_Consumables_Meds.SetIconPath("set:dayz_inventory image:medicalbandage");
		sub_Cat_Consumables_Meds.AddIncluded("CharcoalTablets");
		sub_Cat_Consumables_Meds.AddIncluded("DisinfectantAlcohol");
		sub_Cat_Consumables_Meds.AddIncluded("PurificationTablets");
		sub_Cat_Consumables_Meds.AddIncluded("VitaminBottle");
		sub_Cat_Consumables_Meds.AddIncluded("DisinfectantSpray");
		sub_Cat_Consumables_Meds.AddIncluded("TetracyclineAntibiotics");
		sub_Cat_Consumables_Meds.AddIncluded("PainkillerTablets");
		sub_Cat_Consumables_Meds.AddIncluded("IodineTincture");
		cat_Consumables.AddSubCategory(sub_Cat_Consumables_Meds);

		//! Consumables - Vegetables
		ExpansionP2PMarketMenuSubCategory sub_Cat_Consumables_Vegetables = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Consumables_Vegetables.SetDisplayName("Vegetables");
		sub_Cat_Consumables_Vegetables.SetIconPath("set:dayz_inventory image:food");
		sub_Cat_Consumables_Vegetables.AddIncluded("Apple");
		sub_Cat_Consumables_Vegetables.AddIncluded("GreenBellPepper");
		sub_Cat_Consumables_Vegetables.AddIncluded("Zucchini");
		sub_Cat_Consumables_Vegetables.AddIncluded("Pumpkin");
		sub_Cat_Consumables_Vegetables.AddIncluded("SlicedPumpkin");
		sub_Cat_Consumables_Vegetables.AddIncluded("PotatoSeed");
		sub_Cat_Consumables_Vegetables.AddIncluded("Potato");
		sub_Cat_Consumables_Vegetables.AddIncluded("Tomato");
		sub_Cat_Consumables_Vegetables.AddIncluded("SambucusBerry");
		sub_Cat_Consumables_Vegetables.AddIncluded("CaninaBerry");
		sub_Cat_Consumables_Vegetables.AddIncluded("Plum");
		sub_Cat_Consumables_Vegetables.AddIncluded("Pear");
		sub_Cat_Consumables_Vegetables.AddIncluded("MushroomBase");
		cat_Consumables.AddSubCategory(sub_Cat_Consumables_Vegetables);

		//! Equipment
		ExpansionP2PMarketMenuCategory cat_Equipment = new ExpansionP2PMarketMenuCategory();
		cat_Equipment.SetDisplayName("Equipment");
		cat_Equipment.SetIconPath("set:dayz_inventory image:body");
		cat_Equipment.AddIncluded("ClothingBase");
		cat_Equipment.AddIncluded("Clothing_Base");
		MenuCategories.Insert(cat_Equipment);

		//! Equipment - Armbands
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Armbands = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Armbands.SetDisplayName("Armbands");
		sub_Cat_Equipment_Armbands.SetIconPath("set:dayz_inventory image:armband");
		sub_Cat_Equipment_Armbands.AddIncluded("Armband_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Armbands);

		//! Equipment - Backpacks
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Backpacks = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Backpacks.SetDisplayName("Backpacks");
		sub_Cat_Equipment_Backpacks.SetIconPath("set:dayz_inventory image:back");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Backpacks.AddIncluded("ChildBag_ColorBase");
		sub_Cat_Equipment_Backpacks.AddIncluded("DryBag_ColorBase");
		sub_Cat_Equipment_Backpacks.AddIncluded("TaloonBag_ColorBase");
		sub_Cat_Equipment_Backpacks.AddIncluded("SmershBag");
		sub_Cat_Equipment_Backpacks.AddIncluded("AssaultBag_ColorBase");
		sub_Cat_Equipment_Backpacks.AddIncluded("HuntingBag");
		sub_Cat_Equipment_Backpacks.AddIncluded("TortillaBag");
		sub_Cat_Equipment_Backpacks.AddIncluded("CoyoteBag_ColorBase");
		sub_Cat_Equipment_Backpacks.AddIncluded("MountainBag_ColorBase");
		sub_Cat_Equipment_Backpacks.AddIncluded("AliceBag_ColorBase");
#else
		//! 1.22+
		sub_Cat_Equipment_Backpacks.AddIncluded("Backpack_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Backpacks);

		//! Equipment - Belts
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Belts = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Belts.SetDisplayName("Belts");
		sub_Cat_Equipment_Belts.SetIconPath("set:dayz_inventory image:hips");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Belts.AddIncluded("CivilianBelt");
		sub_Cat_Equipment_Belts.AddIncluded("MilitaryBelt");
#else
		//! 1.22+
		sub_Cat_Equipment_Belts.AddIncluded("Belt_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Belts);

		//! Equipment - Blouses & Suits
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_BlousesAndSuits = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_BlousesAndSuits.SetDisplayName("Blouses & Suits");
		sub_Cat_Equipment_BlousesAndSuits.SetIconPath("set:dayz_inventory image:body");
		sub_Cat_Equipment_BlousesAndSuits.AddIncluded("Blouse_ColorBase");
		sub_Cat_Equipment_BlousesAndSuits.AddIncluded("ManSuit_ColorBase");
		sub_Cat_Equipment_BlousesAndSuits.AddIncluded("WomanSuit_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_BlousesAndSuits);

		//! Equipment - Boots & Shoes
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_BootsAndShoes = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_BootsAndShoes.SetDisplayName("Boots & Shoes");
		sub_Cat_Equipment_BootsAndShoes.SetIconPath("set:dayz_inventory image:feet");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("AthleticShoes_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("JoggingShoes_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("Sneakers_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("Ballerinas_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("DressShoes_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("HikingBootsLow_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("WorkingBoots_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("HikingBoots_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("CombatBoots_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("JungleBoots_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("Wellies_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("TTSKOBoots");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("MilitaryBoots_ColorBase");
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("NBCBootsBase");
#else
		//! 1.22+
		sub_Cat_Equipment_BootsAndShoes.AddIncluded("Shoes_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_BootsAndShoes);

		//! Equipment - Caps
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Caps = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Caps.SetDisplayName("Caps");
		sub_Cat_Equipment_Caps.SetIconPath("set:dayz_inventory image:headgear");
		sub_Cat_Equipment_Caps.AddIncluded("BaseballCap_ColorBase");
		sub_Cat_Equipment_Caps.AddIncluded("PrisonerCap");
		sub_Cat_Equipment_Caps.AddIncluded("PilotkaCap");
		sub_Cat_Equipment_Caps.AddIncluded("PoliceCap");
		sub_Cat_Equipment_Caps.AddIncluded("FlatCap_ColorBase");
		sub_Cat_Equipment_Caps.AddIncluded("ZmijovkaCap_ColorBase");
		sub_Cat_Equipment_Caps.AddIncluded("RadarCap_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Caps);

		//! Equipment - Coats & Jackets
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_CoatsAndJackets = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_CoatsAndJackets.SetDisplayName("Coats & Jackets");
		sub_Cat_Equipment_CoatsAndJackets.SetIconPath("set:dayz_inventory image:body");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("LabCoat");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("TrackSuitJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("DenimJacket");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("WoolCoat_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("RidersJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("FirefighterJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("JumpsuitJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("BomberJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("QuiltedJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("PrisonUniformJacket");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("PoliceJacketOrel");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("PoliceJacket");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("ParamedicJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("HikingJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("Raincoat_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("TTsKOJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("BDUJacket");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("HuntingJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("M65Jacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("GorkaEJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("USMCJacket_ColorBase");
		sub_Cat_Equipment_CoatsAndJackets.AddIncluded("NBCJacketBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_CoatsAndJackets);

		//! Equipment - Eyewear
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Eyewear = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Eyewear.SetDisplayName("Eyewear");
		sub_Cat_Equipment_Eyewear.SetIconPath("set:dayz_inventory image:eyewear");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Eyewear.AddIncluded("SportGlasses_ColorBase");
		sub_Cat_Equipment_Eyewear.AddIncluded("ThinFramesGlasses");
		sub_Cat_Equipment_Eyewear.AddIncluded("ThickFramesGlasses");
		sub_Cat_Equipment_Eyewear.AddIncluded("DesignerGlasses");
		sub_Cat_Equipment_Eyewear.AddIncluded("AviatorGlasses");
		sub_Cat_Equipment_Eyewear.AddIncluded("TacticalGoggles");
		sub_Cat_Equipment_Eyewear.AddIncluded("NVGHeadstrap");
		sub_Cat_Equipment_Eyewear.AddIncluded("EyePatch_Improvised");
#else
		//! 1.22+
		sub_Cat_Equipment_Eyewear.AddIncluded("Glasses_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Eyewear);

		//! Equipment - Ghillies
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Ghillies = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Ghillies.SetDisplayName("Ghillies");
		sub_Cat_Equipment_Ghillies.SetIconPath("set:dayz_inventory image:missing");
		sub_Cat_Equipment_Ghillies.AddIncluded("GhillieHood_ColorBase");
		sub_Cat_Equipment_Ghillies.AddIncluded("GhillieBushrag_ColorBase");
		sub_Cat_Equipment_Ghillies.AddIncluded("GhillieTop_ColorBase");
		sub_Cat_Equipment_Ghillies.AddIncluded("GhillieSuit_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Ghillies);

		//! Equipment - Gloves
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Gloves = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Gloves.SetDisplayName("Gloves");
		sub_Cat_Equipment_Gloves.SetIconPath("set:dayz_inventory image:gloves");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Gloves.AddIncluded("SurgicalGloves_ColorBase");
		sub_Cat_Equipment_Gloves.AddIncluded("WorkingGloves_ColorBase");
		sub_Cat_Equipment_Gloves.AddIncluded("TacticalGloves_ColorBase");
		sub_Cat_Equipment_Gloves.AddIncluded("OMNOGloves_ColorBase");
		sub_Cat_Equipment_Gloves.AddIncluded("NBCGloves_ColorBase");
#else
		//! 1.22+
		sub_Cat_Equipment_Gloves.AddIncluded("Gloves_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Gloves);

		//! Equipment - Hats & Hoods
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_HatsAndHoods = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_HatsAndHoods.SetDisplayName("Hats & Hoods");
		sub_Cat_Equipment_HatsAndHoods.SetIconPath("set:dayz_inventory image:headgear");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("MedicalScrubsHat_ColorBase");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("CowboyHat_ColorBase");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("MilitaryBeret_ColorBase");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("BeanieHat_ColorBase");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("Ushanka_ColorBase");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("BoonieHat_ColorBase");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("OfficerHat");
		sub_Cat_Equipment_HatsAndHoods.AddIncluded("NBCHood");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_HatsAndHoods);

		//! Equipment - Helmets
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Helmets = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Helmets.SetDisplayName("Helmets");
		sub_Cat_Equipment_Helmets.SetIconPath("set:dayz_inventory image:headgear");
		sub_Cat_Equipment_Helmets.AddIncluded("HelmetBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Helmets);

		//! Equipment - Masks
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Masks = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Masks.SetDisplayName("Masks");
		sub_Cat_Equipment_Masks.SetIconPath("set:dayz_inventory image:mask");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Masks.AddIncluded("SurgicalMask");
		sub_Cat_Equipment_Masks.AddIncluded("NioshFaceMask");
		sub_Cat_Equipment_Masks.AddIncluded("HockeyMask");
		sub_Cat_Equipment_Masks.AddIncluded("BalaclavaMask_ColorBase");
		sub_Cat_Equipment_Masks.AddIncluded("Balaclava3Holes_ColorBase");
		sub_Cat_Equipment_Masks.AddIncluded("WeldingMask");
		sub_Cat_Equipment_Masks.AddIncluded("GasMask");
		sub_Cat_Equipment_Masks.AddIncluded("GP5GasMask");
		sub_Cat_Equipment_Masks.AddIncluded("AirborneMask");
#else
		//! 1.22+
		sub_Cat_Equipment_Masks.AddIncluded("Mask_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Masks);

		//! Equipment - Pants
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Pants = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Pants.SetDisplayName("Pants");
		sub_Cat_Equipment_Pants.SetIconPath("set:dayz_inventory image:legs");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Pants.AddIncluded("MedicalScrubsPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("TrackSuitPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("PrisonUniformPants");
		sub_Cat_Equipment_Pants.AddIncluded("Breeches_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("SlacksPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("CanvasPantsMidi_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("CanvasPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("JumpsuitPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("PolicePants");
		sub_Cat_Equipment_Pants.AddIncluded("ParamedicPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("FirefightersPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("CargoPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("ShortJeans_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("Jeans_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("TTSKOPants");
		sub_Cat_Equipment_Pants.AddIncluded("BDUPants");
		sub_Cat_Equipment_Pants.AddIncluded("USMCPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("PolicePantsOrel");
		sub_Cat_Equipment_Pants.AddIncluded("HunterPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("GorkaPants_ColorBase");
		sub_Cat_Equipment_Pants.AddIncluded("NBCPantsBase");
#else
		//! 1.22+
		sub_Cat_Equipment_Pants.AddIncluded("Pants_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Pants);

		//! Equipment - Shirts
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Shirts = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Shirts.SetDisplayName("Shirts");
		sub_Cat_Equipment_Shirts.SetIconPath("set:dayz_inventory image:body");
		sub_Cat_Equipment_Shirts.AddIncluded("TShirt_ColorBase");
		sub_Cat_Equipment_Shirts.AddIncluded("Shirt_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Shirts);

		//! Equipment - Skirts & Dresses
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_SkirtsAndDresses = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_SkirtsAndDresses.SetDisplayName("Skirts & Dresses");
		sub_Cat_Equipment_SkirtsAndDresses.SetIconPath("set:dayz_inventory image:body");
		sub_Cat_Equipment_SkirtsAndDresses.AddIncluded("Skirt_ColorBase");
		sub_Cat_Equipment_SkirtsAndDresses.AddIncluded("MiniDress_ColorBase");
		sub_Cat_Equipment_SkirtsAndDresses.AddIncluded("NurseDress_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_SkirtsAndDresses);

		//! Equipment - Sweaters & Hoodies
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_SweatersAndHoodies = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_SweatersAndHoodies.SetDisplayName("Sweaters & Hoodies");
		sub_Cat_Equipment_SweatersAndHoodies.SetIconPath("set:dayz_inventory image:body");
		sub_Cat_Equipment_SweatersAndHoodies.AddIncluded("Sweater_ColorBase");
		sub_Cat_Equipment_SweatersAndHoodies.AddIncluded("Hoodie_ColorBase");
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_SweatersAndHoodies);

		//! Equipment - Vests
		ExpansionP2PMarketMenuSubCategory sub_Cat_Equipment_Vests = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Equipment_Vests.SetDisplayName("Vests");
		sub_Cat_Equipment_Vests.SetIconPath("set:dayz_inventory image:vest");
#ifdef DAYZ_1_21
		sub_Cat_Equipment_Vests.AddIncluded("ReflexVest");
		sub_Cat_Equipment_Vests.AddIncluded("PoliceVest");
		sub_Cat_Equipment_Vests.AddIncluded("PressVest_ColorBase");
		sub_Cat_Equipment_Vests.AddIncluded("UKAssVest_ColorBase");
		sub_Cat_Equipment_Vests.AddIncluded("SmershVest");
		sub_Cat_Equipment_Vests.AddIncluded("HighCapacityVest_ColorBase");
		sub_Cat_Equipment_Vests.AddIncluded("PlateCarrierVest");
		sub_Cat_Equipment_Vests.AddIncluded("HuntingVest");
#else
		//! 1.22+
		sub_Cat_Equipment_Vests.AddIncluded("Vest_Base");
#endif
		cat_Equipment.AddSubCategory(sub_Cat_Equipment_Vests);

		//! Explosives
		ExpansionP2PMarketMenuCategory cat_Explosives = new ExpansionP2PMarketMenuCategory();
		cat_Explosives.SetDisplayName("Explosives");
		cat_Explosives.SetIconPath("set:dayz_inventory image:explosive");
		cat_Explosives.AddIncluded("ExplosivesBase");
		cat_Explosives.AddIncluded("RemoteDetonator");
		cat_Explosives.AddIncluded("RemoteDetonatorTrigger");
	#ifdef EXPANSIONMODBASEBUILDING
		cat_Explosives.AddIncluded("ExpansionSatchel");
	#endif
	#ifdef EXPANSIONMODWEAPONS
		cat_Explosives.AddIncluded("M18SmokeGrenade_ColorBase");
	#endif
		MenuCategories.Insert(cat_Explosives);

		//! Vehicles
		ExpansionP2PMarketMenuCategory cat_Vehicles = new ExpansionP2PMarketMenuCategory();
		cat_Vehicles.SetDisplayName("Vehicles");
		cat_Vehicles.SetIconPath("set:dayz_inventory image:cat_vehicle_body");
		cat_Vehicles.AddIncluded("CarScript");
		cat_Vehicles.AddIncluded("BoatScript");
		MenuCategories.Insert(cat_Vehicles);

	#ifdef EXPANSIONMODVEHICLE
		//! Vehicles - Cars
		ExpansionP2PMarketMenuSubCategory sub_Cat_Vehicles_Cars = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Vehicles_Cars.SetDisplayName("Cars");
		sub_Cat_Vehicles_Cars.SetIconPath("set:dayz_inventory image:cat_vehicle_body");
		sub_Cat_Vehicles_Cars.AddIncluded("OffroadHatchback");
		sub_Cat_Vehicles_Cars.AddIncluded("Hatchback_02");
		sub_Cat_Vehicles_Cars.AddIncluded("Sedan_02");
		sub_Cat_Vehicles_Cars.AddIncluded("CivilianSedan");
		sub_Cat_Vehicles_Cars.AddIncluded("Truck_01_Base");
		sub_Cat_Vehicles_Cars.AddIncluded("Offroad_02");
		sub_Cat_Vehicles_Cars.AddIncluded("ExpansionTractor");
		sub_Cat_Vehicles_Cars.AddIncluded("ExpansionUAZ");
		sub_Cat_Vehicles_Cars.AddIncluded("ExpansionBus");
		sub_Cat_Vehicles_Cars.AddIncluded("ExpansionVodnik");
		sub_Cat_Vehicles_Cars.AddIncluded("Expansion_Landrover");
		cat_Vehicles.AddSubCategory(sub_Cat_Vehicles_Cars);

		//! Vehicles - Helicopters
		ExpansionP2PMarketMenuSubCategory sub_Cat_Vehicles_Helis = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Vehicles_Helis.SetDisplayName("Helicopters");
		sub_Cat_Vehicles_Helis.SetIconPath("set:dayz_inventory image:cat_vehicle_body");
		sub_Cat_Vehicles_Helis.AddIncluded("ExpansionHelicopterScript");
		cat_Vehicles.AddSubCategory(sub_Cat_Vehicles_Helis);

		//! Vehicles - Boats
		ExpansionP2PMarketMenuSubCategory sub_Cat_Vehicles_Boats = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Vehicles_Boats.SetDisplayName("Boats");
		sub_Cat_Vehicles_Boats.SetIconPath("set:dayz_inventory image:cat_vehicle_body");
		sub_Cat_Vehicles_Boats.AddIncluded("ExpansionBoatScript");
		cat_Vehicles.AddSubCategory(sub_Cat_Vehicles_Boats);
	#endif

		//! Weapons
		ExpansionP2PMarketMenuCategory cat_Weapons = new ExpansionP2PMarketMenuCategory();
		cat_Weapons.SetDisplayName("Weapons");
		cat_Weapons.SetIconPath("set:dayz_inventory image:pistol");
		cat_Weapons.AddIncluded("Weapon_Base");
		cat_Weapons.AddIncluded("Weapon");
		cat_Weapons.AddIncluded("ButtstockBase");
		cat_Weapons.AddIncluded("OpticBase");
		cat_Weapons.AddIncluded("Magazine_Base");
		cat_Weapons.AddExcluded("Ammunition_Base");
		MenuCategories.Insert(cat_Weapons);

		//! Weapons - Attachments
		ExpansionP2PMarketMenuSubCategory sub_Cat_Weapons_Attachments = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Weapons_Attachments.SetDisplayName("Attachments");
		sub_Cat_Weapons_Attachments.SetIconPath("set:dayz_inventory image:weaponmuzzle");
		sub_Cat_Weapons_Attachments.AddIncluded("ButtstockBase");
		sub_Cat_Weapons_Attachments.AddIncluded("OpticBase");
		sub_Cat_Weapons_Attachments.AddIncluded("Magazine_Base");
		sub_Cat_Weapons_Attachments.AddExcluded("Ammunition_Base");
		cat_Weapons.AddSubCategory(sub_Cat_Weapons_Attachments);

		//! Weapons - Assault Rifles
		ExpansionP2PMarketMenuSubCategory sub_Cat_Weapons_AR = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Weapons_AR.SetDisplayName("Assault Rifles");
		sub_Cat_Weapons_AR.SetIconPath("set:dayz_inventory image:pistol");
#ifdef DAYZ_1_21
		sub_Cat_Weapons_AR.AddIncluded("FAL_Base");
		sub_Cat_Weapons_AR.AddIncluded("AKM_Base");
		sub_Cat_Weapons_AR.AddIncluded("AK74_Base");
		sub_Cat_Weapons_AR.AddIncluded("M4A1_Base");
		sub_Cat_Weapons_AR.AddIncluded("M16A2_Base");
		sub_Cat_Weapons_AR.AddIncluded("Famas_Base");
		sub_Cat_Weapons_AR.AddIncluded("Aug_Base");
	#ifdef EXPANSIONMODWEAPONS
		sub_Cat_Weapons_AR.AddIncluded("Expansion_M16_Base");
		sub_Cat_Weapons_AR.AddIncluded("Expansion_G36_Base");
	#endif
#else
		//! 1.22+
		sub_Cat_Weapons_AR.AddIncluded("RifleBoltFree_Base");
#endif
		cat_Weapons.AddSubCategory(sub_Cat_Weapons_AR);

		//! Weapons - Pistols
		ExpansionP2PMarketMenuSubCategory sub_Cat_Weapons_Pistols = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Weapons_Pistols.SetDisplayName("Pistols");
		sub_Cat_Weapons_Pistols.SetIconPath("set:dayz_inventory image:pistol");
		sub_Cat_Weapons_Pistols.AddIncluded("Pistol_Base");
		cat_Weapons.AddSubCategory(sub_Cat_Weapons_Pistols);

		//! Weapons - Rifles
		ExpansionP2PMarketMenuSubCategory sub_Cat_Weapons_Rifles = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Weapons_Rifles.SetDisplayName("Rifles");
		sub_Cat_Weapons_Rifles.SetIconPath("set:dayz_inventory image:pistol");
		sub_Cat_Weapons_Rifles.AddIncluded("Izh18_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("Ruger1022_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("Repeater_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("Mosin9130_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("CZ527_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("CZ550_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("Winchester70_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("SSG82_Base");
	#ifdef EXPANSIONMODWEAPONS
		sub_Cat_Weapons_Rifles.AddIncluded("Expansion_Kar98_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("Expansion_M14_Base");
		sub_Cat_Weapons_Rifles.AddIncluded("Expansion_M1A_Base");
	#endif
		cat_Weapons.AddSubCategory(sub_Cat_Weapons_Rifles);

		//! Weapons - Shotguns
		ExpansionP2PMarketMenuSubCategory sub_Cat_Weapons_Shotguns = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Weapons_Shotguns.SetDisplayName("Shotguns");
		sub_Cat_Weapons_Shotguns.SetIconPath("set:dayz_inventory image:pistol");
		sub_Cat_Weapons_Shotguns.AddIncluded("Izh18Shotgun_Base");
		sub_Cat_Weapons_Shotguns.AddIncluded("Izh43Shotgun_Base");
		sub_Cat_Weapons_Shotguns.AddIncluded("Mp133Shotgun_Base");
		sub_Cat_Weapons_Shotguns.AddIncluded("Remington12");
		sub_Cat_Weapons_Shotguns.AddIncluded("Saiga_Base");
		sub_Cat_Weapons_Shotguns.AddIncluded("SawedoffIzh18Shotgun");
		cat_Weapons.AddSubCategory(sub_Cat_Weapons_Shotguns);

		//! Weapons - Sniper-Rifles
		ExpansionP2PMarketMenuSubCategory sub_Cat_Weapons_SniperRifles = new ExpansionP2PMarketMenuSubCategory();
		sub_Cat_Weapons_SniperRifles.SetDisplayName("Sniper-Rifles");
		sub_Cat_Weapons_SniperRifles.SetIconPath("set:dayz_inventory image:pistol");
		sub_Cat_Weapons_SniperRifles.AddIncluded("VSS_Base");
		sub_Cat_Weapons_SniperRifles.AddIncluded("B95_base");
		sub_Cat_Weapons_SniperRifles.AddIncluded("SVD_Base");
		sub_Cat_Weapons_SniperRifles.AddIncluded("Scout_Base");
		sub_Cat_Weapons_SniperRifles.AddIncluded("SV98_Base");
	#ifdef EXPANSIONMODWEAPONS
		sub_Cat_Weapons_SniperRifles.AddIncluded("Expansion_AWM");
	#endif
		cat_Weapons.AddSubCategory(sub_Cat_Weapons_SniperRifles);
	}

	protected void DefaultExcludedClassNames()
	{
		ExcludedClassNames.Insert("SurvivorBase");
		ExcludedClassNames.Insert("ExpansionMoneyBase");
	}

	override string SettingName()
	{
		return "Black Market Settings";
	}
};