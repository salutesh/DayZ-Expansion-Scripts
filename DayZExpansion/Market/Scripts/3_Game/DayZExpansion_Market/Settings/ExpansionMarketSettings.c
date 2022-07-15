/**
 * ExpansionMarketSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	string CurrencyIcon;
	bool ATMSystemEnabled = true;
	int MaxDepositMoney = 100000;
	int DefaultDepositMoney = 0;
	bool ATMPlayerTransferEnabled = true;
	bool ATMPartyLockerEnabled = true;
	int MaxPartyDepositMoney = 100000;
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
	static const int VERSION = 9;

	protected static ref map<string, string> s_MarketAmmoBoxes = new map<string, string>;

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
	
	autoptr TStringArray Currencies;
	
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
		
		m_Categories = new map<int, ref ExpansionMarketCategory>;
		m_TraderZones = new array<ref ExpansionMarketTraderZone>;	
		m_Traders = new array<ref ExpansionMarketTrader>;

		Currencies = new TStringArray;
		
		//! Ammo boxes and corresponding ammo are only needed on client
		if (!GetGame().IsDedicatedServer() && !s_MarketAmmoBoxes.Count())
		{
			int count = GetGame().ConfigGetChildrenCount("CfgVehicles");
		
			EXPrint(ToString() + " - enumerating " + count + " CfgVehicles entries");

			for (int i = 0; i < count; i++)
			{
				string className;

				GetGame().ConfigGetChildName("CfgVehicles", i, className);
				if (!GetGame().IsKindOf(className, "Box_Base"))
					continue;

				string path = "CfgVehicles " + className + " Resources";
				if (GetGame().ConfigIsExisting(path))
				{
					int resCount = GetGame().ConfigGetChildrenCount(path);

					for (int j = 0; j < resCount; j++)
					{
						string childName;
						GetGame().ConfigGetChildName(path, j, childName);
						if (!GetGame().IsKindOf(childName, "Ammunition_Base"))
							continue;

						s_MarketAmmoBoxes.Insert(childName, className);
					}
				}
			}

			EXPrint(ToString() + " - found " + s_MarketAmmoBoxes.Count() + " ammo boxes with corresponding ammo");
		}

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
		
		array< string > files = ExpansionStatic.FindFilesInLocation(EXPANSION_MARKET_FOLDER, ".json");

		if (!files.Count())
		{
			EXPrint("[ExpansionMarketSettings] No existing market category setting files at:" + EXPANSION_MARKET_FOLDER + ". Creating defaults!");
			
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_MARKET_FOLDER);
			DefaultCategories();
			return;
		}

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
		
		//! Move existing files over from old location in $profile to new location in $mission
		string folderNameOld = EXPANSION_FOLDER + "TraderZones\\";
		if (FileExist(folderNameOld))
			MoveSettings(folderNameOld, EXPANSION_TRADER_ZONES_FOLDER);

		array<string > files = ExpansionStatic.FindFilesInLocation(EXPANSION_TRADER_ZONES_FOLDER, ".json");

		if (!files.Count())
		{
			EXPrint("[ExpansionMarketSettings] No existing market zone setting files at:" + EXPANSION_TRADER_ZONES_FOLDER + ". Creating defaults!");
			
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_TRADER_ZONES_FOLDER);
			DefaultTraderZones();
			return;
		}
		
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
		
		array< string > files = ExpansionStatic.FindFilesInLocation(EXPANSION_TRADER_FOLDER, ".json");

		if (!files.Count())
		{
			EXPrint("[ExpansionMarketSettings] No existing trader setting files at:" + EXPANSION_TRADER_FOLDER + ". Creating defaults!");

			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_TRADER_FOLDER);

			DefaultTraders();

			return;
		}
		
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
		
		ExpansionMarketSettings s = new ExpansionMarketSettings;

		ctx.Read(s.MarketSystemEnabled);
		ctx.Read(s.NetworkCategories);
		ctx.Read(s.CurrencyIcon);
		ctx.Read(s.ATMSystemEnabled);
		ctx.Read(s.MaxDepositMoney);
		ctx.Read(s.DefaultDepositMoney);
		ctx.Read(s.ATMPlayerTransferEnabled);
		ctx.Read(s.ATMPartyLockerEnabled);
		ctx.Read(s.MaxPartyDepositMoney);

		ctx.Read(s.UseWholeMapForATMPlayerList);
		ctx.Read(s.SellPricePercent);
		ctx.Read(s.NetworkBatchSize);
		ctx.Read(s.MaxVehicleDistanceToTrader);
		ctx.Read(s.MaxLargeVehicleDistanceToTrader);
		ctx.Read(s.LargeVehicles);
		ctx.Read(s.Currencies);

		s.MarketMenuColors.OnReceive(ctx);

		CopyInternal(s);

		MarketMenuColors.Update();
		
		m_IsLoaded = true;
		
		ExpansionSettings.SI_Market.Invoke();
		
		//TraderPrint("OnRecieve - End");

		return true;
	}
	
	override void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(MarketSystemEnabled);
		ctx.Write(NetworkCategories);
		ctx.Write(CurrencyIcon);
		ctx.Write(ATMSystemEnabled);
		ctx.Write(MaxDepositMoney);
		ctx.Write(DefaultDepositMoney);
		ctx.Write(ATMPlayerTransferEnabled);
		ctx.Write(ATMPartyLockerEnabled);
		ctx.Write(MaxPartyDepositMoney);

		ctx.Write(UseWholeMapForATMPlayerList);
		ctx.Write(SellPricePercent);
		ctx.Write(NetworkBatchSize);
		ctx.Write(MaxVehicleDistanceToTrader);
		ctx.Write(MaxLargeVehicleDistanceToTrader);
		ctx.Write(LargeVehicles);
		ctx.Write(Currencies);
		//! Do not send vehicle spawn positions (only used on server)

		MarketMenuColors.OnSend(ctx);
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		MarketSystemEnabled = s.MarketSystemEnabled;

		//! @note Round to avoid mismatch between server and client due to quantization
		SellPricePercent = Math.Round(s.SellPricePercent);

		MarketMenuColors = s.MarketMenuColors;
		
		UseWholeMapForATMPlayerList = s.UseWholeMapForATMPlayerList;
		
		NetworkBatchSize = s.NetworkBatchSize;

		MaxVehicleDistanceToTrader = s.MaxVehicleDistanceToTrader;
		MaxLargeVehicleDistanceToTrader = s.MaxLargeVehicleDistanceToTrader;
		
		LargeVehicles.Copy(s.LargeVehicles);
		
		Currencies.Copy(s.Currencies);

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
		
		//! Need to clear spawn positions first
		AirSpawnPositions.Clear();
		for (i = 0; i < s.AirSpawnPositions.Count(); i++)
		{
			position = new ExpansionMarketSpawnPosition;
			position.Copy(s.AirSpawnPositions[i]);
			AirSpawnPositions.Insert(position);
		}
		
		//! Need to clear spawn positions first
		WaterSpawnPositions.Clear();
		for (i = 0; i < s.WaterSpawnPositions.Count(); i++)
		{
			position = new ExpansionMarketSpawnPosition;
			position.Copy(s.WaterSpawnPositions[i]);
			WaterSpawnPositions.Insert(position);
		}

		if (!m_Categories.Count())
		{
			//! NEVER clear these here (dealt with by ClearMarketCaches which is called on mission finish), otherwise respawning will get rid of already synched categories and trader menu will be empty
			for (i = 0; i < s.NetworkCategories.Count(); i++)
			{
				ExpansionMarketCategory category = new ExpansionMarketCategory;
				category.Copy(s.NetworkCategories[i]);
				m_Categories.Insert(category.CategoryID, category);
			}
		}

		ExpansionMarketSettingsBase sb = s;
		CopyInternal(sb);
	}
	
	private void CopyInternal( ExpansionMarketSettingsBase s)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		CurrencyIcon = s.CurrencyIcon;
		
		ATMSystemEnabled = s.ATMSystemEnabled;
		MaxDepositMoney = s.MaxDepositMoney;
		DefaultDepositMoney = s.DefaultDepositMoney;
		ATMPlayerTransferEnabled = s.ATMPlayerTransferEnabled;
		ATMPartyLockerEnabled = s.ATMPartyLockerEnabled;
		MaxPartyDepositMoney = s.MaxPartyDepositMoney;
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

		int sellPricePercent = networkItem.Packed & 0x0000ffff;
		if (sellPricePercent > 0x00007fff)
			sellPricePercent -= 0x00010000;

		int quantityPercent = (networkItem.Packed & 0x00ff0000) >> 16;
		if (quantityPercent > 0x7f)
			quantityPercent -= 0x100;

		ExpansionMarketItem item = category.AddItem(clsName, networkItem.MinPriceThreshold, networkItem.MaxPriceThreshold, networkItem.MinStockThreshold, networkItem.MaxStockThreshold, NULL, networkItem.Variants, sellPricePercent, quantityPercent, networkItem.ItemID, networkItem.AttachmentIDs);

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
		
		Currencies.Insert("expansiongoldbar");
		Currencies.Insert("expansiongoldnugget");
		Currencies.Insert("expansionsilverbar");
		Currencies.Insert("expansionsilvernugget");
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
		AddDefaultCategory(new ExpansionMarketGardening);
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
	#ifdef EXPANSIONMODWEAPONS
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
	#ifdef EXPANSIONMODWEAPONS
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
		
		if ( worldName.IndexOf("gloom") == worldName.Length() - 5 )
		{
			worldName = worldName.Substring(0, worldName.Length() - 5);
		}
		
		if (worldName == "chernarusplus")
		{
			m_TraderZones.Insert(new ExpansionMarketSvetloyarskZone);
			m_TraderZones.Insert(new ExpansionMarketKrasnostavZone);
			m_TraderZones.Insert(new ExpansionMarketKamenkaZone);
			m_TraderZones.Insert(new ExpansionMarketKamenkaBoatsZone);
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
		else if (worldName == "chiemsee")
		{
			m_TraderZones.Insert(new ExpansionMarketKiesWerkZone);
			m_TraderZones.Insert(new ExpansionMarketNeviHoffZone);
		}
		else
		{
			m_TraderZones.Insert(new ExpansionMarketTraderZone);
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
		m_Traders.Insert(new ExpansionMarketTraderClothingAccessories);
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
		
		if ( worldName.IndexOf("gloom") == worldName.Length() - 5 )
		{
			worldName = worldName.Substring(0, worldName.Length() - 5);
		}
		
		if (worldName == "chernarusplus")
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
		else if (worldName == "chiemsee")
		{
			DefaultChiemseeSpawnPositions();
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
	void DefaultChiemseeSpawnPositions()
	{
		//! TODO: Make Air, Boat and Ground spawn pos !
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
			
		//! Move existing settings file over from old location in $profile to new location in $mission
		string fileNameOld = EXPANSION_SETTINGS_FOLDER + "MarketSettings.json";
		if (FileExist(fileNameOld))
			MoveSettings(fileNameOld, EXPANSION_MARKET_SETTINGS);

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

					MarketMenuColors.Set("BaseColorVignette", settings_v2.MarketMenuColors.BaseColorVignette);
					MarketMenuColors.Set("BaseColorHeaders", settings_v2.MarketMenuColors.BaseColorHeaders);
					MarketMenuColors.Set("BaseColorLabels", settings_v2.MarketMenuColors.BaseColorLabels);
					MarketMenuColors.Set("BaseColorText", settings_v2.MarketMenuColors.BaseColorTexts);
					MarketMenuColors.Set("BaseColorCheckboxes", settings_v2.MarketMenuColors.BaseColorCheckboxes);
					MarketMenuColors.Set("BaseColorInfoSectionBackground", settings_v2.MarketMenuColors.BaseColorInfoSectionBackground);
					MarketMenuColors.Set("BaseColorTooltipsBackground", settings_v2.MarketMenuColors.BaseColorTooltipsBackground);
					MarketMenuColors.Set("ColorDecreaseQuantityButton", settings_v2.MarketMenuColors.ColorDecreaseQuantityButton);
					MarketMenuColors.Set("ColorDecreaseQuantityIcon", settings_v2.MarketMenuColors.ColorDecreaseQuantityIcon);
					MarketMenuColors.Set("ColorSetQuantityButton", settings_v2.MarketMenuColors.ColorSetQuantityButton);
					MarketMenuColors.Set("ColorIncreaseQuantityButton", settings_v2.MarketMenuColors.ColorIncreaseQuantityButton);
					MarketMenuColors.Set("ColorIncreaseQuantityIcon", settings_v2.MarketMenuColors.ColorIncreaseQuantityIcon);
					MarketMenuColors.Set("ColorSellPanel", settings_v2.MarketMenuColors.ColorSellPanel);
					MarketMenuColors.Set("ColorSellButton", settings_v2.MarketMenuColors.ColorSellButton);
					MarketMenuColors.Set("ColorBuyPanel", settings_v2.MarketMenuColors.ColorBuyPanel);
					MarketMenuColors.Set("ColorBuyButton", settings_v2.MarketMenuColors.ColorBuyButton);
					MarketMenuColors.Set("ColorMarketIcon", settings_v2.MarketMenuColors.ColorMarketIcon);
					MarketMenuColors.Set("ColorFilterOptionsButton", settings_v2.MarketMenuColors.ColorFilterOptionsButton);
					MarketMenuColors.Set("ColorFilterOptionsIcon", settings_v2.MarketMenuColors.ColorFilterOptionsIcon);
					MarketMenuColors.Set("ColorSearchFilterButton", settings_v2.MarketMenuColors.ColorSearchFilterButton);
					MarketMenuColors.Set("ColorCategoryButton", settings_v2.MarketMenuColors.ColorCategoryButton);
					MarketMenuColors.Set("ColorCategoryCollapseIcon", settings_v2.MarketMenuColors.ColorCategoryCollapseIcon);
					MarketMenuColors.Set("ColorCurrencyDenominationText", settings_v2.MarketMenuColors.ColorCurrencyDenominationTexts);
					MarketMenuColors.Set("ColorItemButton", settings_v2.MarketMenuColors.ColorItemButton);
					MarketMenuColors.Set("ColorItemInfoIcon", settings_v2.MarketMenuColors.ColorItemInfoIcon);
					MarketMenuColors.Set("ColorItemInfoTitle", settings_v2.MarketMenuColors.ColorItemInfoTitle);
					MarketMenuColors.Set("ColorItemInfoHasContainerItems", settings_v2.MarketMenuColors.ColorItemInfoHasContainerItems);
					MarketMenuColors.Set("ColorItemInfoHasAttachments", settings_v2.MarketMenuColors.ColorItemInfoHasAttachments);
					MarketMenuColors.Set("ColorItemInfoHasBullets", settings_v2.MarketMenuColors.ColorItemInfoHasBullets);
					MarketMenuColors.Set("ColorItemInfoIsAttachment", settings_v2.MarketMenuColors.ColorItemInfoIsAttachment);
					MarketMenuColors.Set("ColorItemInfoIsEquipped", settings_v2.MarketMenuColors.ColorItemInfoIsEquiped);
					MarketMenuColors.Set("ColorItemInfoAttachments", settings_v2.MarketMenuColors.ColorItemInfoAttachments);
					MarketMenuColors.Set("ColorToggleCategoriesText", settings_v2.MarketMenuColors.ColorToggleCategoriesText);
					MarketMenuColors.Set("ColorCategoryCorners", settings_v2.MarketMenuColors.ColorCategoryCorners);
					MarketMenuColors.Set("ColorCategoryBackground", settings_v2.MarketMenuColors.ColorCategoryBackground);

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
				
				if (settingsBase.m_Version < 9)
				{
					Currencies.Copy(settingsDefault.Currencies);
				}
				
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionMarketSettings>.JsonLoadFile(EXPANSION_MARKET_SETTINGS, this);
			}

			//! @note Round to avoid mismatch between server and client due to quantization
			SellPricePercent = Math.Round(SellPricePercent);

			MarketMenuColors.Update();

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

			LargeVehicles = StringArrayToLower(LargeVehicles);

			Currencies = StringArrayToLower(Currencies);
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
		return s_MarketAmmoBoxes.Get(name);
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

	static TStringArray StringArrayToLower(TStringArray input)
	{
		TStringArray output();
		foreach (string entry : input)
		{
			entry.ToLower();
			output.Insert(entry);
		}
		return output;
	}
}