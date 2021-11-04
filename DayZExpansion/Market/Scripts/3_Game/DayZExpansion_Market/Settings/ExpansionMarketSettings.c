/**
 * ExpansionMarketSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMarketSettingsBase
 * @brief		Market settings base class
 **/
class ExpansionMarketSettingsBase: ExpansionSettingBase
{
	bool MarketSystemEnabled = true;
	
	autoptr array<ref ExpansionMarketNetworkCategory> NetworkCategories;
	autoptr array<ref ExpansionMarketAmmoBoxesForAmmo> MarketAmmoBoxes;
	string CurrencyIcon;
	bool ATMSystemEnabled = true;
	int MaxDepositMoney = 100000;
	int DefaultDepositMoney = 0;
	bool ATMPlayerTransferEnabled = true;
	bool ATMPartyLockerEnabled = true;
	int MaxPartyDepositMoney = 100000;
	autoptr array<string> MarketVIPs;
}

/**@class		ExpansionMarketSettingsV1
 * @brief		Market settings v1 class
 **/
class ExpansionMarketSettingsV1: ExpansionMarketSettingsBase
{
	autoptr array<vector> LandSpawnPositions;
	autoptr array<vector> AirSpawnPositions;
	autoptr array<vector> WaterSpawnPositions;
}

/**@class		ExpansionMarketSettingsBaseV2
 * @brief		Market settings v2 (and v3) base class
 **/
class ExpansionMarketSettingsBaseV2: ExpansionMarketSettingsBase
{
	float SellPricePercent;

	autoptr array<ref ExpansionMarketSpawnPositionV1> LandSpawnPositions;
	autoptr array<ref ExpansionMarketSpawnPositionV1> AirSpawnPositions;
	autoptr array<ref ExpansionMarketSpawnPositionV1> WaterSpawnPositions;
}

/**@class		ExpansionMarketSettingsV2
 * @brief		Market settings v2 class
 **/
class ExpansionMarketSettingsV2: ExpansionMarketSettingsBaseV2
{
	ref ExpansionMarketMenuColorsV2 MarketMenuColors;
}

/**@class		ExpansionMarketSettingsV3
 * @brief		Market settings v3 class
 **/
class ExpansionMarketSettingsV3: ExpansionMarketSettingsBaseV2
{
	bool UseWholeMapForATMPlayerList;
	ref ExpansionMarketMenuColors MarketMenuColors;
}

/**@class		ExpansionMarketSettings
 * @brief		Market settings class
 **/
class ExpansionMarketSettings: ExpansionMarketSettingsBase
{
	static const int VERSION = 7;

	bool UseWholeMapForATMPlayerList;
	float SellPricePercent;
	int NetworkBatchSize;

	float MaxVehicleDistanceToTrader;
	float MaxLargeVehicleDistanceToTrader;
	autoptr TStringArray LargeVehicles;
	autoptr array<ref ExpansionMarketSpawnPosition> LandSpawnPositions;
	autoptr array<ref ExpansionMarketSpawnPosition> AirSpawnPositions;
	autoptr array<ref ExpansionMarketSpawnPosition> WaterSpawnPositions;

	ref ExpansionMarketMenuColors MarketMenuColors;
	
	[NonSerialized()]
	protected autoptr map<int, ref ExpansionMarketCategory> m_Categories;
	[NonSerialized()]
	protected autoptr array<ref ExpansionMarketTraderZone> m_TraderZones;
	[NonSerialized()]
	protected autoptr array<ref ExpansionMarketTrader> m_Traders;
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	void ExpansionMarketSettings()
	{
		//TraderPrint("ExpansionMarketSettings - Start");
		
		LargeVehicles = new TStringArray;
		LandSpawnPositions = new array<ref ExpansionMarketSpawnPosition>;
		AirSpawnPositions = new array<ref ExpansionMarketSpawnPosition>;
		WaterSpawnPositions = new array<ref ExpansionMarketSpawnPosition>;
		NetworkCategories = new array<ref ExpansionMarketNetworkCategory>;
		MarketMenuColors = new ExpansionMarketMenuColors;
		MarketAmmoBoxes = new array<ref ExpansionMarketAmmoBoxesForAmmo>;
		MarketVIPs = new array<string>;
		
		m_Categories = new map<int, ref ExpansionMarketCategory>;
		m_TraderZones = new array<ref ExpansionMarketTraderZone>;	
		m_Traders = new array<ref ExpansionMarketTrader>;

		//TraderPrint("ExpansionMarketSettings - End");
	}
	
	void ~ExpansionMarketSettings()
	{
		EXPrint("~ExpansionMarketSettings");
	}

	// ------------------------------------------------------------
	protected void LoadCategories()
	{
		//TraderPrint("LoadCategories - Start");
		
		if (!FileExist(EXPANSION_MARKET_FOLDER))
		{
			EXPrint("[ExpansionMarketSettings] No existing market category setting files at:" + EXPANSION_MARKET_FOLDER + ". Creating defaults!");
			
			MakeDirectory(EXPANSION_MARKET_FOLDER);
			DefaultCategories();
			return;
		}

		array< string > files = ExpansionStatic.FindFilesInLocation(EXPANSION_MARKET_FOLDER, ".json");

		foreach (string fileName : files)
		{
			//! Strip '.json' extension
			fileName = fileName.Substring(0, fileName.Length() - 5);

			ExpansionMarketCategory category = ExpansionMarketCategory.Load(fileName);

			TraderPrint("LoadCategories - Adding category ID " + category.CategoryID + " (" + category.m_FileName + ")");
			m_Categories.Insert(category.CategoryID, category);

			NetworkCategories.Insert(new ExpansionMarketNetworkCategory(category));
		}

		//TraderPrint("LoadCategories - End");
	}
	
	// ------------------------------------------------------------
	protected void LoadTraderZones()
	{
		//TraderPrint("LoadTraderZones - Start");
		
		if (!FileExist(EXPANSION_TRADER_ZONES_FOLDER))
		{
			EXPrint("[ExpansionMarketSettings] No existing market zone setting files at:" + EXPANSION_TRADER_ZONES_FOLDER + ". Creating defaults!");
			
			MakeDirectory(EXPANSION_TRADER_ZONES_FOLDER);
			DefaultTraderZones();
			return;
		}

		array<string > files = ExpansionStatic.FindFilesInLocation(EXPANSION_TRADER_ZONES_FOLDER, ".json");
		
		foreach (string fileName : files)
		{
			//! Strip '.json' extension
			fileName = fileName.Substring(0, fileName.Length() - 5);

			ExpansionMarketTraderZone zone = ExpansionMarketTraderZone.Load(fileName);
			zone.Update();
			m_TraderZones.Insert(zone);
		}
		
		//TraderPrint("LoadTraderZones - End");
	}

	// ------------------------------------------------------------
	protected void LoadTraders()
	{
		//TraderPrint("LoadTraders - Start");
		
		if (!FileExist(EXPANSION_TRADER_FOLDER))
		{
			EXPrint("[ExpansionMarketSettings] No existing trader setting files at:" + EXPANSION_TRADER_FOLDER + ". Creating defaults!");

			MakeDirectory(EXPANSION_TRADER_FOLDER);

			DefaultTraders();

			return;
		}

		array< string > files = ExpansionStatic.FindFilesInLocation(EXPANSION_TRADER_FOLDER, ".json");
		
		foreach (string fileName : files)
		{
			//! Strip '.json' extension
			fileName = fileName.Substring(0, fileName.Length() - 5);

			m_Traders.Insert(ExpansionMarketTrader.Load(fileName));
		}
		
		//TraderPrint("LoadTraders - End");
	}	
	
	// ------------------------------------------------------------
	override bool OnRecieve(ParamsReadContext ctx)
	{
		//TraderPrint("OnRecieve - Start");
		
		ExpansionMarketSettings setting;
		if (!ctx.Read(setting))
		{
			Error("ExpansionMarketSettings::OnRecieve setting");
			return false;
		}

		CopyInternal(setting);

		MarketMenuColors.Update();
		
		m_IsLoaded = true;
		
		ExpansionSettings.SI_Market.Invoke();
		
		//TraderPrint("OnRecieve - End");

		return true;
	}
	
	override void OnSend(ParamsWriteContext ctx)
	{
		ExpansionMarketSettings thisSetting = this;
		ctx.Write(thisSetting);
	}
	
	// ------------------------------------------------------------
	override int Send(PlayerIdentity identity)
	{
		//TraderPrint("Send - Start");
		
		if (!IsMissionHost())
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend(rpc);
		rpc.Send(null, ExpansionSettingsRPC.Market, true, identity);
		
		//TraderPrint("Send - End and return");
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy(ExpansionSettingBase setting)
	{
		ExpansionMarketSettings s;
		if (!Class.CastTo(s, setting))
			return false;

		CopyInternal(s);
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionMarketSettings s)
	{
		EXPrint(ToString() + "::CopyInternal " + s);

		MarketSystemEnabled = s.MarketSystemEnabled;

		SellPricePercent = s.SellPricePercent;

		MarketMenuColors = s.MarketMenuColors;
		
		UseWholeMapForATMPlayerList = s.UseWholeMapForATMPlayerList;
		
		NetworkBatchSize = s.NetworkBatchSize;

		MaxVehicleDistanceToTrader = s.MaxVehicleDistanceToTrader;
		MaxLargeVehicleDistanceToTrader = s.MaxLargeVehicleDistanceToTrader;
		
		LargeVehicles.Copy(s.LargeVehicles);

		int i;
		ExpansionMarketSpawnPosition position;

		//! Need to clear spawn positions first
		LandSpawnPositions.Clear();
		for (i = 0; i < s.LandSpawnPositions.Count(); i++)
		{
			position = new ExpansionMarketSpawnPosition;
			position.Copy(s.LandSpawnPositions[i]);
			LandSpawnPositions.Insert(position);
		}
		EXPrint("LandSpawnPositions " + LandSpawnPositions.Count());
		
		//! Need to clear spawn positions first
		AirSpawnPositions.Clear();
		for (i = 0; i < s.AirSpawnPositions.Count(); i++)
		{
			position = new ExpansionMarketSpawnPosition;
			position.Copy(s.AirSpawnPositions[i]);
			AirSpawnPositions.Insert(position);
		}
		EXPrint("AirSpawnPositions " + AirSpawnPositions.Count());
		
		//! Need to clear spawn positions first
		WaterSpawnPositions.Clear();
		for (i = 0; i < s.WaterSpawnPositions.Count(); i++)
		{
			position = new ExpansionMarketSpawnPosition;
			position.Copy(s.WaterSpawnPositions[i]);
			WaterSpawnPositions.Insert(position);
		}
		EXPrint("WaterSpawnPositions " + WaterSpawnPositions.Count());

		if (!m_Categories.Count())
		{
			//! NEVER clear these here (dealt with by ClearMarketCaches which is called on mission finish), otherwise respawning will get rid of already synched categories and trader menu will be empty
			EXPrint("Syncing categories from network categories");
			for (i = 0; i < s.NetworkCategories.Count(); i++)
			{
				ExpansionMarketCategory category = new ExpansionMarketCategory;
				category.Copy(s.NetworkCategories[i]);
				m_Categories.Insert(category.CategoryID, category);
			}
		}
		EXPrint("Categories " + m_Categories.Count());

		ExpansionMarketSettingsBase sb = s;
		CopyInternal(sb);
	}
	
	private void CopyInternal( ExpansionMarketSettingsBase s)
	{
		EXPrint(ToString() + "::CopyInternal " + s);

		MarketAmmoBoxes = s.MarketAmmoBoxes;
		CurrencyIcon = s.CurrencyIcon;
		
		ATMSystemEnabled = s.ATMSystemEnabled;
		MaxDepositMoney = s.MaxDepositMoney;
		DefaultDepositMoney = s.DefaultDepositMoney;
		ATMPlayerTransferEnabled = s.ATMPlayerTransferEnabled;
		ATMPartyLockerEnabled = s.ATMPartyLockerEnabled;
		MaxPartyDepositMoney = s.MaxPartyDepositMoney;
				
		MarketVIPs.Copy(s.MarketVIPs);
	}
	
	// ------------------------------------------------------------
	ExpansionMarketItem UpdateMarketItem_Client(ExpansionMarketNetworkItem networkItem)
	{
		//TraderPrint("UpdateMarketItem_Client - Start - " + networkItem.ClassName + " (" + networkItem.Stock + ") catID " + networkItem.CategoryID);

		string clsName = networkItem.ClassName;
		clsName.ToLower();

		ExpansionMarketCategory category = GetCategory(networkItem.CategoryID);

		ExpansionMarketItem existingItem = category.GetItem(clsName, false);
		if ( existingItem )
		{
			//TraderPrint("UpdateMarketItem_Client - End and return existing item: " + existingItem);

			return existingItem;
		}

		int sellPricePercent = networkItem.Packed & 0x00ffffff;
		if (sellPricePercent > 0x007fffff)
			sellPricePercent -= 0x01000000;

		ExpansionMarketItem item = category.AddItem(clsName, networkItem.MinPriceThreshold, networkItem.MaxPriceThreshold, networkItem.MinStockThreshold, networkItem.MaxStockThreshold, NULL, networkItem.Variants, sellPricePercent, networkItem.ItemID, networkItem.AttachmentIDs);

		//TraderPrint("UpdateMarketItem_Client - End and return newly added item: " + item);

		return item;
	}
	
	// ------------------------------------------------------------
	private void AddDefaultCategory(ExpansionMarketCategory category)
	{
		category.Defaults();
		category.Save();
		category.Finalize();
		GetCategories().Set(category.CategoryID, category);
		NetworkCategories.Insert(new ExpansionMarketNetworkCategory(category));
	}

	override void Defaults()
	{
		m_Version = VERSION;
		
		MarketSystemEnabled = true;
		
		MaxVehicleDistanceToTrader = 120;
		MaxLargeVehicleDistanceToTrader = 744;
		LargeVehicles.Insert("expansionlhd");
		DefaultTraderSpawnAreas();
		MarketMenuColors.Update();
		DefaultMarketAmmoBoxes();
		
		CurrencyIcon = "DayZExpansion/Market/GUI/icons/coinstack2_64x64.edds";
		SellPricePercent = 75;
		NetworkBatchSize = 100;  //! Sync at most n items per batch
		
		ATMSystemEnabled = true;
		MaxDepositMoney = 100000;
		DefaultDepositMoney = 10000;
		ATMPlayerTransferEnabled = true;
		ATMPartyLockerEnabled = true;
		MaxPartyDepositMoney = 100000;
		UseWholeMapForATMPlayerList = false;
	}

	// ------------------------------------------------------------
	protected void DefaultCategories()
	{		
		//TraderPrint("DefaultCategories - Start");
		AddDefaultCategory(new ExpansionMarketExchange);
		AddDefaultCategory(new ExpansionMarketAmmo);
		AddDefaultCategory(new ExpansionMarketAmmoBoxes);
		AddDefaultCategory(new ExpansionMarketArmbands);
		AddDefaultCategory(new ExpansionMarketAssaultRifles);
		AddDefaultCategory(new ExpansionMarketBackpacks);
		AddDefaultCategory(new ExpansionMarketBandanas);
		AddDefaultCategory(new ExpansionMarketBatteries);
		AddDefaultCategory(new ExpansionMarketBelts);
		AddDefaultCategory(new ExpansionMarketBlousesAndSuits);
	#ifdef EXPANSIONMODVEHICLE
		AddDefaultCategory(new ExpansionMarketBoats);
	#endif
		AddDefaultCategory(new ExpansionMarketBootsAndShoes);
		AddDefaultCategory(new ExpansionMarketButtstocks);
		AddDefaultCategory(new ExpansionMarketCaps);
		AddDefaultCategory(new ExpansionMarketCars);
		AddDefaultCategory(new ExpansionMarketCoatsAndJackets);
		AddDefaultCategory(new ExpansionMarketContainers);
		AddDefaultCategory(new ExpansionMarketDrinks);
		AddDefaultCategory(new ExpansionMarketElectronics);
		AddDefaultCategory(new ExpansionMarketSubmachineGuns);
		AddDefaultCategory(new ExpansionMarketExplosives);
		AddDefaultCategory(new ExpansionMarketEyewear);
		AddDefaultCategory(new ExpansionMarketFish);
		AddDefaultCategory(new ExpansionMarketFishing);
		AddDefaultCategory(new ExpansionMarketFood); 
		AddDefaultCategory(new ExpansionMarketGhillies);
		AddDefaultCategory(new ExpansionMarketGloves);
		AddDefaultCategory(new ExpansionMarketHandguards);
		AddDefaultCategory(new ExpansionMarketHatsAndHoods);
		AddDefaultCategory(new ExpansionMarketHostersAndPouches);
	#ifdef EXPANSIONMODVEHICLE
		AddDefaultCategory(new ExpansionMarketHelicopters);
	#endif
		AddDefaultCategory(new ExpansionMarketHelmets);
		AddDefaultCategory(new ExpansionMarketKits);
		AddDefaultCategory(new ExpansionMarketKnifes);
	#ifdef EXPANSIONMOD
		AddDefaultCategory(new ExpansionMarketLaunchers);
	#endif
		AddDefaultCategory(new ExpansionMarketLights);
		AddDefaultCategory(new ExpansionMarketLiquids);
		AddDefaultCategory(new ExpansionMarketLocks);
		AddDefaultCategory(new ExpansionMarketMagazines);
		AddDefaultCategory(new ExpansionMarketMasks);
		AddDefaultCategory(new ExpansionMarketMeat);
		AddDefaultCategory(new ExpansionMarketMedical);
		AddDefaultCategory(new ExpansionMarketMelee);
		AddDefaultCategory(new ExpansionMarketMuzzles);
		AddDefaultCategory(new ExpansionMarketNavigation);
		AddDefaultCategory(new ExpansionMarketOptics);
		AddDefaultCategory(new ExpansionMarketPants);
		AddDefaultCategory(new ExpansionMarketPistols);
		AddDefaultCategory(new ExpansionMarketRifles);
		AddDefaultCategory(new ExpansionMarketShirtsAndTShirts);
		AddDefaultCategory(new ExpansionMarketShotguns);
		AddDefaultCategory(new ExpansionMarketSkirtsAndDresses);
		AddDefaultCategory(new ExpansionMarketSniperRifles);
		AddDefaultCategory(new ExpansionMarketSupplies);
		AddDefaultCategory(new ExpansionMarketSweatersAndHoodies);
		AddDefaultCategory(new ExpansionMarketTents);
		AddDefaultCategory(new ExpansionMarketTools);
		AddDefaultCategory(new ExpansionMarketVegetables);
		AddDefaultCategory(new ExpansionMarketVehicleParts);
		AddDefaultCategory(new ExpansionMarketVests);
	#ifdef EXPANSIONMOD
		AddDefaultCategory(new ExpansionMarketCrossbows);
	#endif
		AddDefaultCategory(new ExpansionMarketEvent);
	#ifdef EXPANSIONMODVEHICLE
		AddDefaultCategory(new ExpansionMarketSpraycans);
	#endif
		AddDefaultCategory(new ExpansionMarketFlags);
		AddDefaultCategory(new ExpansionMarketBayonets);
		AddDefaultCategory(new ExpansionMarketFurnishings);
		//TraderPrint("DefaultCategories - End");
	}
	
	// ------------------------------------------------------------
	protected void DefaultTraderZones()
	{		
		//TraderPrint("DefaultTraderZones - Start");
		string worldName;
		GetGame().GetWorldName(worldName);
		worldName.ToLower();
		
		if (worldName == "chernarusplus" || worldName == "chernarusplusgloom")
		{
			m_TraderZones.Insert(new ExpansionMarketSvetloyarskZone);
			m_TraderZones.Insert(new ExpansionMarketKrasnostavZone);
			m_TraderZones.Insert(new ExpansionMarketKamenkaZone);
			m_TraderZones.Insert(new ExpansionMarketKamenkaBoatsZone);
			m_TraderZones.Insert(new ExpansionMarketBalotaAircraftsZone);
			m_TraderZones.Insert(new ExpansionMarketBalotaAircraftsZone);
			m_TraderZones.Insert(new ExpansionMarketGreenMountainZone);
		}
		else if (worldName == "namalsk")
		{
			m_TraderZones.Insert(new ExpansionMarketJaloviskoZone);
			m_TraderZones.Insert(new ExpansionMarketNamalskAirstripZone);
			m_TraderZones.Insert(new ExpansionMarketTaraHarborZone);
		}
		else if (worldName == "takistanplus")
		{
			m_TraderZones.Insert(new ExpansionMarketMarastarZone);
		}
		
		for (int i = 0; i < m_TraderZones.Count(); i++)
		{
			m_TraderZones[i].Defaults();
			m_TraderZones[i].Save();
		}
		
		//TraderPrint("DefaultTraderZones - End");
	}

	// ------------------------------------------------------------
	protected void DefaultTraders()
	{
		//TraderPrint("DefaultTraders - Start");
	
	#ifdef EXPANSIONMODVEHICLE
		m_Traders.Insert(new ExpansionMarketTraderAircraft);
		m_Traders.Insert(new ExpansionMarketTraderBoats);
	#endif
		m_Traders.Insert(new ExpansionMarketTraderAttachments);
		m_Traders.Insert(new ExpansionMarketTraderBuildingSupplies);
		m_Traders.Insert(new ExpansionMarketTraderClothing);
		m_Traders.Insert(new ExpansionMarketTraderClothingAccessoires);
		m_Traders.Insert(new ExpansionMarketTraderComponents);
		m_Traders.Insert(new ExpansionMarketTraderConsumables);
		m_Traders.Insert(new ExpansionMarketTraderEvent);
		m_Traders.Insert(new ExpansionMarketTraderExchange);
		m_Traders.Insert(new ExpansionMarketTraderFishing);
		m_Traders.Insert(new ExpansionMarketTraderMedicals);
		m_Traders.Insert(new ExpansionMarketTraderSpecial);
		m_Traders.Insert(new ExpansionMarketTraderSpraycans);
		m_Traders.Insert(new ExpansionMarketTraderVehicleParts);
		m_Traders.Insert(new ExpansionMarketTraderVehicles);
		m_Traders.Insert(new ExpansionMarketTraderWeapons);
		
		foreach (ExpansionMarketTrader trader: m_Traders)
		{
			trader.Defaults();
			trader.Save();
			trader.Finalize();
		}
		
		//TraderPrint("DefaultTraders - End");
	}
	
	// ------------------------------------------------------------
	protected void DefaultTraderSpawnAreas()
	{
		//TraderPrint("DefaultTraderSpawnAreas - Start");

		string worldName;
		GetGame().GetWorldName(worldName);
		worldName.ToLower();
		
		if (worldName == "chernarusplus" || worldName == "chernarusplusgloom")
		{
			DefaultChernarusSpawnPositions();
		}
		else if (worldName == "namalsk")
		{
			DefaultNamalskSpawnPositions();
		}
		else if (worldName == "takistanplus")
		{
			DefaultTakistanSpawnPositions();
		}

		//TraderPrint("DefaultTraderSpawnAreas - End");
	}
	
	// ------------------------------------------------------------
	void DefaultChernarusSpawnPositions()
	{
		ExpansionMarketSpawnPosition position;
		
		//! Cars - Vehicle Trader - Krasno
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(11903.4, 140.0, 12455.1);
		position.Orientation = Vector(24.0, 0.0, 0.0); 
		LandSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(11898.4, 140.0, 12481.6);
		position.Orientation = Vector(24.0, 0.0, 0.0); 
		LandSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(11878.0, 140.0, 12482.8);
		position.Orientation = Vector(24.0, 0.0, 0.0); 
		LandSpawnPositions.Insert(position);
		
		//! Cars - Vehicle Trader - Kamenka
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(1145.0, 6.0, 2405.0);
		position.Orientation = Vector(0.0, 0.0, 0.0);
		LandSpawnPositions.Insert(position);
		
		// Cars - Vehicle Trader - Green Mountain Trader
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(3722.77, 402.0, 6018.93);
		position.Orientation = Vector(138.0, 0.0, 0.0);
		LandSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(3737.19, 402.7, 6001.95);
		position.Orientation = Vector(138.0, 0.0, 0.0);
		LandSpawnPositions.Insert(position);
		
		//! Aircraft - Aircraft Trader - Krasno
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(12178.9, 140.0, 12638.4);
		position.Orientation = Vector(-157.2, 0.0, 0.0); 
		AirSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(12126.7, 140.0, 12664.7);
		position.Orientation = Vector(-66.6, 0.0, 0.0); 
		AirSpawnPositions.Insert(position);
		
		//! Aircraft - Aircraft Trader - Balota
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(5006.27, 9.5, 2491.1);
		position.Orientation = Vector(-131.7, 0.0, 0.0); 
		AirSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(4982.0, 9.5, 2468.0);
		position.Orientation = Vector(-131.7, 0.0, 0.0);
		AirSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(4968.0, 9.5, 2513.0);
		position.Orientation = Vector(-131.7, 0.0, 0.0);
		AirSpawnPositions.Insert(position);
		
		//! Water - Boats Trader - Kamenka
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(1759.0, 0.0, 1994.0);
		position.Orientation = Vector(0.0, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
		
		//! Water - Boats Trader - Sventlo
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(14347.8, 0.0, 13235.8);
		position.Orientation = Vector(-147.5, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(14344.1, 0.0, 13219.7);
		position.Orientation = Vector(-147.5, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(14360.9, 0.0, 13246.7);
		position.Orientation = Vector(-147.5, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
	
		//! Water - LHD - Boats Trader - Kamenka
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(1760.0, 0.0, 1730.0);
		position.Orientation = Vector(0.0, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
		
		//! Water - LHD - Boats Trader - Sventlo
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(14540.0, 0.0, 12995.0);
		position.Orientation = Vector(0.0, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
	}
	
	// ------------------------------------------------------------
	void DefaultNamalskSpawnPositions()
	{
		ExpansionMarketSpawnPosition position;
		
		//! Aircraft - Aircraft Trader - Airfield
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(6316.43, 21.0, 9491.48);
		position.Orientation = Vector(0.0, 0.0, 0.0); 
		AirSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(6316.49, 21.0, 9463.48);
		position.Orientation = Vector(0.0, 0.0, 0.0); 
		AirSpawnPositions.Insert(position);
		
		//! Cars - Jalovisko Trader CameraType
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(8585, 15.3904, 10482.7);
		position.Orientation = Vector(-63.0, 0.0, 0.0); 
		LandSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(8561.96, 15.4844, 10495);
		position.Orientation = Vector(-63.0, 0.0, 0.0); 
		LandSpawnPositions.Insert(position);
		
		//! Water - Boats Trader - Tara Harbor
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(8055.79, 0.7295, 7582.59);
		position.Orientation = Vector(33.2414, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
		
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(8059, 0.233, 7597.12);
		position.Orientation = Vector(33.2414, 0.0, 0.0); 
		WaterSpawnPositions.Insert(position);
	}
	
	// ------------------------------------------------------------
	void DefaultTakistanSpawnPositions()
	{
		ExpansionMarketSpawnPosition position;
		
		//! Cars - Vehicle Trader - Marastar
		position = new ExpansionMarketSpawnPosition;
		position.Position = Vector(4603.24, 4.2, 12332.2);
		position.Orientation = Vector(203.0, 0.0, 0.0); 
		LandSpawnPositions.Insert(position);
	}
	
	// ------------------------------------------------------------
	protected void DefaultMarketAmmoBoxes()
	{
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_556x45","AmmoBox_556x45_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_556x45Tracer","AmmoBox_556x45Tracer_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_545x39","AmmoBox_545x39_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_545x39Tracer","AmmoBox_545x39Tracer_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_762x39","AmmoBox_762x39_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_762x39Tracer","AmmoBox_762x39Tracer_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_308Win","AmmoBox_308Win_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_308WinTracer","AmmoBox_308WinTracer_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_762x54","AmmoBox_762x54_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_762x54Tracer","AmmoBox_762x54Tracer_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_22","AmmoBox_22_50Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_380","AmmoBox_380_35rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_9x19","AmmoBox_9x19_25rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_45ACP","AmmoBox_45ACP_25rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_357","AmmoBox_357_20Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_12gaPellets","AmmoBox_00buck_10rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_12gaSlug","AmmoBox_12gaSlug_10Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_12gaRubberSlug","AmmoBox_12gaRubberSlug_10Rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_9x39AP","AmmoBox_9x39AP_20rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_9x39","AmmoBox_9x39_20rnd"));
		
	#ifdef EXPANSIONMOD
		//! Expansion
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Expansion_Ammo_8mm","Expansion_AmmoBox_8mm_15rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_Expansion_46x30","AmmoBox_Expansion_46x30_25rnd"));
		MarketAmmoBoxes.Insert(new ExpansionMarketAmmoBoxesForAmmo("Ammo_Expansion_338","AmmoBox_Expansion_338_15rnd"));
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
		//TraderPrint("Load - Start");

		m_IsLoaded = true;

		bool marketSettingsExist = FileExist(EXPANSION_MARKET_SETTINGS);

		//! Settings have to be loaded BEFORE categories, else NetworkCategories will not be updated correctly!
		if (marketSettingsExist)
		{
			EXPrint("[ExpansionMapSettings] Load existing setting file:" + EXPANSION_MARKET_SETTINGS);
			
			ExpansionMarketSettings settingsDefault = new ExpansionMarketSettings;
			settingsDefault.Defaults();

			ExpansionMarketSettingsBase settingsBase;
			JsonFileLoader<ExpansionMarketSettingsBase>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, settingsBase);

			bool save;

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionMarketTrader] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_MARKET_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 2)
				{
					ExpansionMarketSettingsV1 settings_v1;

					JsonFileLoader<ExpansionMarketSettingsV1>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, settings_v1);

					//! ExpansionMarketSpawnPosition was added with version 2
					foreach (vector landSpawnPos : settings_v1.LandSpawnPositions)
					{
						LandSpawnPositions.Insert(new ExpansionMarketSpawnPosition(landSpawnPos));
					}
					foreach (vector airSpawnPos : settings_v1.AirSpawnPositions)
					{
						AirSpawnPositions.Insert(new ExpansionMarketSpawnPosition(airSpawnPos));
					}
					foreach (vector waterSpawnPos : settings_v1.WaterSpawnPositions)
					{
						WaterSpawnPositions.Insert(new ExpansionMarketSpawnPosition(waterSpawnPos));
					}

					SellPricePercent = settingsDefault.SellPricePercent;  //! SellPricePercent was added with version 2
				}

				if (settingsBase.m_Version < 3)
				{
					ExpansionMarketSettingsV2 settings_v2;

					JsonFileLoader<ExpansionMarketSettingsV2>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, settings_v2);

					MarketMenuColors.BaseColorVignette = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorVignette);
					MarketMenuColors.BaseColorHeaders = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorHeaders);
					MarketMenuColors.BaseColorLabels = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorLabels);
					MarketMenuColors.BaseColorText = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorTexts);
					MarketMenuColors.BaseColorCheckboxes = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorCheckboxes);
					MarketMenuColors.BaseColorInfoSectionBackground = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorInfoSectionBackground);
					MarketMenuColors.BaseColorTooltipsBackground = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.BaseColorTooltipsBackground);
					MarketMenuColors.ColorDecreaseQuantityButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorDecreaseQuantityButton);
					MarketMenuColors.ColorDecreaseQuantityIcon = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorDecreaseQuantityIcon);
					MarketMenuColors.ColorSetQuantityButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorSetQuantityButton);
					MarketMenuColors.ColorIncreaseQuantityButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorIncreaseQuantityButton);
					MarketMenuColors.ColorIncreaseQuantityIcon = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorIncreaseQuantityIcon);
					MarketMenuColors.ColorSellPanel = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorSellPanel);
					MarketMenuColors.ColorSellButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorSellButton);
					MarketMenuColors.ColorBuyPanel = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorBuyPanel);
					MarketMenuColors.ColorBuyButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorBuyButton);
					MarketMenuColors.ColorMarketIcon = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorMarketIcon);
					MarketMenuColors.ColorFilterOptionsButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorFilterOptionsButton);
					MarketMenuColors.ColorFilterOptionsIcon = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorFilterOptionsIcon);
					MarketMenuColors.ColorSearchFilterButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorSearchFilterButton);
					MarketMenuColors.ColorCategoryButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorCategoryButton);
					MarketMenuColors.ColorCategoryCollapseIcon = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorCategoryCollapseIcon);
					MarketMenuColors.ColorCurrencyDenominationText = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorCurrencyDenominationTexts);
					MarketMenuColors.ColorItemButton = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemButton);
					MarketMenuColors.ColorItemInfoIcon = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoIcon);
					MarketMenuColors.ColorItemInfoTitle = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoTitle);
					MarketMenuColors.ColorItemInfoHasContainerItems = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoHasContainerItems);
					MarketMenuColors.ColorItemInfoHasAttachments = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoHasAttachments);
					MarketMenuColors.ColorItemInfoHasBullets = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoHasBullets);
					MarketMenuColors.ColorItemInfoIsAttachment = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoIsAttachment);
					MarketMenuColors.ColorItemInfoIsEquipped = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoIsEquiped);
					MarketMenuColors.ColorItemInfoAttachments = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorItemInfoAttachments);
					MarketMenuColors.ColorToggleCategoriesText = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorToggleCategoriesText);
					MarketMenuColors.ColorCategoryCorners = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorCategoryCorners);
					MarketMenuColors.ColorCategoryBackground = ExpansionColor.ARGBToHex(settings_v2.MarketMenuColors.ColorCategoryBackground);

					ExpansionMarketSettingsBaseV2 settingsBaseV2;
					JsonFileLoader<ExpansionMarketSettingsBaseV2>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, settingsBaseV2);

					CopyInternal(settingsBaseV2);
					
					UseWholeMapForATMPlayerList = settingsDefault.UseWholeMapForATMPlayerList;  //! UseWholeMapForATMPlayerList was added with version 3
				}

				if (settingsBase.m_Version < 4)
				{
					ExpansionMarketSettingsV3 settings_v3;

					JsonFileLoader<ExpansionMarketSettingsV3>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, settings_v3);

					SellPricePercent = settings_v3.SellPricePercent;
					MarketMenuColors = settings_v3.MarketMenuColors;
					UseWholeMapForATMPlayerList = settings_v3.UseWholeMapForATMPlayerList;

					//! Convert V3 spawn positions to V4
					ExpansionMarketSpawnPosition pos;
					foreach (ExpansionMarketSpawnPositionV1 asp_v1: settings_v3.AirSpawnPositions)
					{
						pos = new ExpansionMarketSpawnPosition(asp_v1.Position, asp_v1.Direction);
						AirSpawnPositions.Insert(pos);
					}
					foreach (ExpansionMarketSpawnPositionV1 lsp_v1: settings_v3.LandSpawnPositions)
					{
						pos = new ExpansionMarketSpawnPosition(lsp_v1.Position, lsp_v1.Direction);
						LandSpawnPositions.Insert(pos);
					}
					foreach (ExpansionMarketSpawnPositionV1 wsp_v1: settings_v3.WaterSpawnPositions)
					{
						pos = new ExpansionMarketSpawnPosition(wsp_v1.Position, wsp_v1.Direction);
						WaterSpawnPositions.Insert(pos);
					}
				}
				else
				{
					JsonFileLoader<ExpansionMarketSettings>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, this);
				}

				if (settingsBase.m_Version < 5)
				{
					NetworkBatchSize = settingsDefault.NetworkBatchSize;
				}

				if (settingsBase.m_Version < 6)
				{
					MaxVehicleDistanceToTrader = settingsDefault.MaxVehicleDistanceToTrader;
					MaxLargeVehicleDistanceToTrader = settingsDefault.MaxLargeVehicleDistanceToTrader;
					LargeVehicles.Copy(settingsDefault.LargeVehicles);
				}

				if (settingsBase.m_Version < 7)
				{
					MarketMenuColors.BaseColorTooltipsHeaders = settingsDefault.MarketMenuColors.BaseColorTooltipsHeaders;
				}
				
				CopyInternal(settingsBase);

				MarketMenuColors.Update();

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionMarketSettings>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, this);
			}

			if (NetworkCategories.Count() > 0)
			{
				//! Never use saved network categories
				NetworkCategories.Clear();
				//! Save without categories
				save = true;
			}

			//! Special case: Need to save before loading categories
			if (save)
				Save();

			foreach (string largeVehicle : LargeVehicles)
			{
				largeVehicle.ToLower();
			}
		}

		LoadCategories();
		LoadTraders();
		LoadTraderZones();

		if (!marketSettingsExist)
		{
			EXPrint("[ExpansionMarketSettings] No existing setting files:" + EXPANSION_MARKET_SETTINGS + ". Creating defaults!");
			Defaults();
			Save();
		}
		
		//TraderPrint("Load - End");
		
		//! NOTE: Special case, we always return true
		return true;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{		
		Print("[ExpansionMarketSettings] Saving settings");
		
		//! Never save network categories
		array<ref ExpansionMarketNetworkCategory> cats = NetworkCategories;
		NetworkCategories = NULL;

		JsonFileLoader<ExpansionMarketSettings>.JsonSaveFile(EXPANSION_MARKET_SETTINGS, this);

		NetworkCategories = cats;

		for (int i = 0; i < GetCategories().Count(); i++)
		{
			if (GetCategories().GetElement(i))
				GetCategories().GetElement(i).Save();
		}

		for (int j = 0; j < m_TraderZones.Count(); j++)
		{
			m_TraderZones[j].Save();
		}

		for (int k = 0; k < m_Traders.Count(); k++)
		{
			m_Traders[k].Save();
		}

		return true;
	}

	// ------------------------------------------------------------
	ExpansionMarketItem GetItem(string clsName, bool checkCategoryFinalized = true)
	{
		ExpansionMarketItem item;

		foreach (int categoryID, ExpansionMarketCategory currentCategory : m_Categories)
		{
			item = currentCategory.GetItem(clsName, checkCategoryFinalized);
			if (item)
				return item;
		}

		return NULL;
	}

	ExpansionMarketItem GetItem(int itemID, bool checkCategoryFinalized = true)
	{
		ExpansionMarketItem item;

		foreach (int categoryID, ExpansionMarketCategory currentCategory : m_Categories)
		{
			item = currentCategory.GetItem(itemID, checkCategoryFinalized);
			if (item)
				return item;
		}

		return NULL;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketCategory GetCategory
	// ------------------------------------------------------------
	map<int, ref ExpansionMarketCategory> GetCategories()
	{
		return m_Categories;
	}

	// ------------------------------------------------------------
	ExpansionMarketCategory GetCategory(int id)
	{
		return m_Categories.Get(id);
	}

	ExpansionMarketCategory GetCategory(string fileName)
	{
		foreach (int categoryID, ExpansionMarketCategory currentCategory : m_Categories)
		{
			if (currentCategory.m_FileName == fileName)
				return currentCategory;
		}

		return NULL;
	}

	// ------------------------------------------------------------
	ExpansionMarketTraderZone GetTraderZoneByPosition(vector position)
	{
		//TraderPrint("GetTraderZoneByPosition - Start");
		
		float closestDistance = -1;
		ExpansionMarketTraderZone closestZone;
		if (m_TraderZones.Count() != 0)
		{
			foreach (ExpansionMarketTraderZone currentZone : m_TraderZones)
			{
				float distance = vector.Distance(currentZone.Position, position);
				if (distance > currentZone.Radius)
					continue;
				if (closestDistance < 0 || distance <= closestDistance)
				{
					closestDistance = distance;
					closestZone = currentZone;
				}
			}
		}

		//TraderPrint("GetTraderZoneByPosition - End - Null");
		return closestZone;
	}

	float GetMinVehicleDistanceToTrader(string className)
	{
		className.ToLower();
		if (MaxVehicleDistanceToTrader < MaxLargeVehicleDistanceToTrader)
		{
			foreach (string largeVehicle : LargeVehicles)
			{
				if (className == largeVehicle)
					return MaxVehicleDistanceToTrader;  //! Use normal vehicle max distance as large vehicle min distance
			}
		}
		return 0;
	}

	float GetMaxVehicleDistanceToTrader(string className)
	{
		className.ToLower();
		foreach (string largeVehicle : LargeVehicles)
		{
			if (className == largeVehicle)
				return MaxLargeVehicleDistanceToTrader;
		}
		return MaxVehicleDistanceToTrader;
	}

	// ------------------------------------------------------------
	ExpansionMarketTrader GetMarketTrader(string fileName)
	{
		foreach (ExpansionMarketTrader currentTrader : m_Traders)
		{
			if (currentTrader.m_FileName == fileName)
			{
				return currentTrader;
			}
		}
		return NULL;
	}
	
	void AddMarketTrader(ExpansionMarketTrader trader)
	{
		EXPrint("Caching trader " + trader.m_FileName);
		m_Traders.Insert(trader);
	}

	void ClearMarketCaches()
	{
		EXPrint("Clearing cached categories " + m_Categories.Count());
		m_Categories.Clear();
		EXPrint("Clearing cached traders " + m_Traders.Count());
		m_Traders.Clear();
	}

	// ------------------------------------------------------------
	string GetAmmoBoxWithAmmoName(string name)
	{
		for (int i = 0; i < MarketAmmoBoxes.Count(); i++)
		{
			ExpansionMarketAmmoBoxesForAmmo current = MarketAmmoBoxes[i];
			if (current.Ammo == name)
				return current.AmmoBox;
		}
		
		return "";
	}

	// ------------------------------------------------------------
	void TraderPrint(string text)
	{
		Print("ExpansionMarketSettings::" + text );
	}
	
	// ------------------------------------------------------------
	bool IsMapChernarus()
	{
		string MissionWorldName = "empty";
		GetGame().GetWorldName(MissionWorldName);
		MissionWorldName.ToLower();
		
		if (MissionWorldName.Contains("chernarus"))
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------	
	override string SettingName()
	{
		return "Market Settings";
	}
}