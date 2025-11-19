/**
 * ExpansionP2PMarketModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionP2PMarketModule)]
class ExpansionP2PMarketModule: CF_ModuleWorld
{
	protected const int LISTINGS_PER_PAGE_COUNT = 14;
	
	protected static ExpansionP2PMarketModule s_Instance;
	static string s_P2PMarketConfigFolderPath = "$mission:expansion\\p2pmarket\\";

	protected static ref TStringArray s_DoorSlotKeywords = {"door", "hood", "trunk", "dver", "kapot", "bagazhnik"};
	protected static ref TStringArray s_WheelSlotKeywords = {"wheel", "koleso"};

	protected bool m_Initialized;
	protected float m_CheckListingsTime;
	protected const float CHECK_TICK_TIME = 60.0;
	protected ref ExpansionMarketModule m_MarketModule;

	protected ref map<int, ref ExpansionP2PMarketTraderConfig> m_P2PTraderConfig = new map<int, ref ExpansionP2PMarketTraderConfig>; //! Server
	protected ref map<int, ref array<ref ExpansionP2PMarketListing>> m_ListingsData = new map<int, ref array<ref ExpansionP2PMarketListing>>; //! Server
	protected ref map<int, ref array<ref ExpansionP2PMarketListing>> m_SoldListingsData = new map<int, ref array<ref ExpansionP2PMarketListing>>; //! Server
	protected ref map<int, ref array<ref ExpansionP2PMarketCategoryListings>> m_CategoryListings = new map<int, ref array<ref ExpansionP2PMarketCategoryListings>>; //! Server & Client
	protected ref map<int, ref map<int, ref array<ref ExpansionP2PMarketCategoryListings>>> m_TraderCategoryListings = new map<int, ref map<int, ref array<ref ExpansionP2PMarketCategoryListings>>>; //! Server & Client
	protected ref map<string, int> m_TradingPlayers = new map<string, int>; //! Server
	protected ref map<string, ref ExpansionP2PMarketCounters> m_PlayerDataCounters = new map<string, ref ExpansionP2PMarketCounters>; //! Server
	protected int m_ListingsCount; //! Server

	protected ref ExpansionP2PMarketPlayerInventory m_LocalEntityInventory; //! Client
	protected ref ScriptInvoker m_ListingsInvoker; //! Client
	protected ref ScriptInvoker m_ListingDetailsInvoker; //! Client
	protected ref ScriptInvoker m_CallbackInvoker; //! Client
	protected ref ScriptInvoker m_UpdateInvoker; //! Client
	
	protected ExpansionP2PMarketSettings m_P2PMarketSettings;

	static ref TStringArray m_HardcodedExcludes = {"AugOptic", "Magnum_Cylinder", "Magnum_Ejector", "M97DummyOptics"};

	void ExpansionP2PMarketModule()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
				
		s_Instance = this;
	}

	override void OnInit()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
	
		EnableMissionStart();
		EnableMissionLoaded();
		EnableInvokeConnect();
		#ifdef SERVER
		EnableUpdate();
		#endif
		Expansion_EnableRPCManager();
		
		Expansion_RegisterServerRPC("RPC_RemoveTradingPlayer");
		Expansion_RegisterServerRPC("RPC_RequestSaleFromListing");
		Expansion_RegisterServerRPC("RPC_RequestAllPlayerSales");
		Expansion_RegisterServerRPC("RPC_RequestBasicListingData");
		Expansion_RegisterClientRPC("RPC_SendBasicListingData");
		Expansion_RegisterClientRPC("RPC_SendCategoryListingsData");
		Expansion_RegisterServerRPC("RPC_RequestListItem");
		Expansion_RegisterClientRPC("RPC_Callback");
		Expansion_RegisterServerRPC("RPC_RequestPurchaseItem");
		Expansion_RegisterServerRPC("RPC_RequestListingDetails");
		Expansion_RegisterClientRPC("RPC_SendListingDetails");
		Expansion_RegisterClientRPC("RPC_UpdateClientRequest");
	}

	protected void CreateDirectoryStructure()
	{
		if (!FileExist(GetP2PMarketDataDirectory()))
			ExpansionStatic.MakeDirectoryRecursive(GetP2PMarketDataDirectory());
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
				
		m_P2PMarketSettings = GetExpansionSettings().GetP2PMarket(false);
		#ifdef SERVER
		if (m_P2PMarketSettings.Enabled)
		{
			CreateDirectoryStructure();
			LoadP2PMarketServerData();

			m_Initialized = true;
		}
		#endif
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		if (g_Game.IsServer() && g_Game.IsMultiplayer())
			ServerModuleInit();

		if (g_Game.IsClient())
			ClientModuleInit();
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (g_Game.IsServer() && g_Game.IsMultiplayer())
		{
			string playerUID = cArgs.Identity.GetId();
			//! Add counter data instance for connecting player
			ExpansionP2PMarketCounters counters = GetPlayerDataCounters(playerUID);
			
			//! Get all sales on player connection and display overall sold value in chat on players client
			int totalIncome;
			int salesCount;
			foreach (int traderID, array<ref ExpansionP2PMarketListing>> listings: m_SoldListingsData)
			{
				foreach (ExpansionP2PMarketListing soldListing: listings)
				{
					if (soldListing.GetListingState() != ExpansionP2PMarketListingState.SOLD || soldListing.GetOwnerUID() != playerUID)
						continue;

					totalIncome += soldListing.GetPrice();
					salesCount++;
				}
			}

			if (totalIncome > 0)
			{
				Callback(cArgs.Identity, ExpansionP2PMarketModuleCallback.MsgTotalSold, "", totalIncome, salesCount);
			}
		}
	}

	ExpansionP2PMarketCounters GetPlayerDataCounters(string playerUID)
	{
		ExpansionP2PMarketCounters counters;
		if (!m_PlayerDataCounters.Find(playerUID, counters))
		{
			counters = new ExpansionP2PMarketCounters();
			m_PlayerDataCounters.Insert(playerUID, counters);
		}

		return counters;
	}

	protected void ServerModuleInit()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		//! Server only
		if (g_Game.IsServer() && g_Game.IsMultiplayer())
		{
			m_MarketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));

			foreach (ExpansionP2PMarketTraderConfig config: m_P2PTraderConfig)
			{
				//! Spawn NPCs late so mapping already loaded
				config.Spawn();
			}
		}
	}

	protected void ClientModuleInit()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		if (g_Game.IsClient())
		{
			m_ListingsInvoker = new ScriptInvoker();
			m_ListingDetailsInvoker = new ScriptInvoker();
			m_CallbackInvoker = new ScriptInvoker();
			m_UpdateInvoker = new ScriptInvoker();
		}
	}

	protected void LoadP2PMarketServerData()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		//! Move existing configs (if any) from old to new location
		string dataDir = GetP2PMarketDataDirectory();
		array<string> p2pMarketFilesExisting = ExpansionStatic.FindFilesInLocation(dataDir, ".json");
		foreach (string existingFile: p2pMarketFilesExisting)
		{
			ExpansionStatic.CopyFileOrDirectoryTree(dataDir + existingFile, s_P2PMarketConfigFolderPath + existingFile, "", true);
		}

		if (FileExist(s_P2PMarketConfigFolderPath))
		{
			array<string> p2pTraderFiles = ExpansionStatic.FindFilesInLocation(s_P2PMarketConfigFolderPath, ".json");
			foreach (string fileName: p2pTraderFiles)
			{
				LoadP2PMarketTraderData(fileName, s_P2PMarketConfigFolderPath);
			}
		}
		else
		{
			if (ExpansionStatic.MakeDirectoryRecursive(s_P2PMarketConfigFolderPath))
				CreateDefaultP2PTraderConfig();
		}
		
		LoadListingCategories();
		UpdateListingsCategoriesData();
	}

	//! Server
	protected void LoadListingCategories()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
	
		int i;
		ExpansionP2PMarketCategoryListings categoryData;
		ExpansionP2PMarketMenuCategory menuCategory;
		ExpansionP2PMarketMenuSubCategory subCategory;
		ExpansionP2PMarketCategoryListings subCategoryData;
		array<ref ExpansionP2PMarketMenuSubCategory> subCategories;
		array<ref ExpansionP2PMarketCategoryListings> dataArray;
	
		//! Add "Uncategorized" category to menu settings
		menuCategory = new ExpansionP2PMarketMenuCategory();
		menuCategory.SetDisplayName("Uncategorized");
		menuCategory.SetCategoryIndex(m_P2PMarketSettings.MenuCategories.Count());
		menuCategory.SetIconPath("set:dayz_inventory image:missing");
		m_P2PMarketSettings.MenuCategories.Insert(menuCategory);
		
		//! Initialize global category listings
		for (i = 0; i < m_P2PMarketSettings.MenuCategories.Count(); ++i)
		{
			menuCategory = m_P2PMarketSettings.MenuCategories[i];
			menuCategory.SetCategoryIndex(i);
	
			dataArray = new array<ref ExpansionP2PMarketCategoryListings>;
			categoryData = new ExpansionP2PMarketCategoryListings();
			categoryData.m_CategoryIndex = i;
			dataArray.Insert(categoryData);
	
			subCategories = menuCategory.GetSubCategories();
			if (!subCategories)
			{
				m_CategoryListings.Set(i, dataArray);
				continue;
			}
	
			for (int j = 0; j < subCategories.Count(); ++j)
			{
				subCategory = subCategories[j];
				subCategory.SetCategoryIndex(i);
				subCategory.SetSubCategoryIndex(j);
				subCategoryData = new ExpansionP2PMarketCategoryListings();
				subCategoryData.m_CategoryIndex = i;
				subCategoryData.m_SubCategoryIndex = j;
				dataArray.Insert(subCategoryData);
			}
	
			m_CategoryListings.Set(i, dataArray);
		}
		
		foreach (int index, array<ref ExpansionP2PMarketCategoryListings> lc: m_CategoryListings)
		{
		    P2PDebugPrint("[GLOBAL] Category Index: " + index + " | Data count: " + lc.Count());
			foreach (ExpansionP2PMarketCategoryListings cle: lc)
			{
				 P2PDebugPrint("----- [CATEGORY] Category Index: " + cle.m_CategoryIndex + " | Sub catgory index: " + cle.m_SubCategoryIndex + " | Count: " + cle.m_Count);
			}
		}
	
		//! Initialize per-trader category listings
		foreach (int traderID, ref array<ref ExpansionP2PMarketListing> listings: m_ListingsData)
		{
			map<int, ref array<ref ExpansionP2PMarketCategoryListings>> traderCategoryMap = new map<int, ref array<ref ExpansionP2PMarketCategoryListings>>;
			for (int k = 0; k < m_P2PMarketSettings.MenuCategories.Count(); ++k)
			{
				menuCategory = m_P2PMarketSettings.MenuCategories[k];
				
				dataArray = new array<ref ExpansionP2PMarketCategoryListings>;
				categoryData = new ExpansionP2PMarketCategoryListings();
				categoryData.m_CategoryIndex = k;
				dataArray.Insert(categoryData);
				
				subCategories = menuCategory.GetSubCategories();
				if (!subCategories)
				{
					traderCategoryMap.Set(k, dataArray);
					continue;
				}
				
				for (int m = 0; m < subCategories.Count(); ++m)
				{
					subCategory = subCategories[m];
					subCategoryData = new ExpansionP2PMarketCategoryListings();
					subCategoryData.m_CategoryIndex = k;
					subCategoryData.m_SubCategoryIndex = m;
					dataArray.Insert(subCategoryData);
				}
				
				traderCategoryMap.Set(k, dataArray);
			}
			
			m_TraderCategoryListings.Set(traderID, traderCategoryMap);
		}
		
		foreach(int tID, map<int, ref array<ref ExpansionP2PMarketCategoryListings>> tclm: m_TraderCategoryListings)
		{
			P2PDebugPrint("[TRADER] Trader ID: " + tID + " | Data count: " + tclm.Count());
			foreach (int tIndex, array<ref ExpansionP2PMarketCategoryListings> tlc: tclm)
			{
			    P2PDebugPrint("----- [TRADER] Category Index: " + tIndex + " | Data count: " + tlc.Count());
				foreach (ExpansionP2PMarketCategoryListings tcle: tlc)
				{
					 P2PDebugPrint("---------- [CATEGORY] Category Index: " + tcle.m_CategoryIndex + " | Sub catgory index: " + tcle.m_SubCategoryIndex + " | Count: " + tcle.m_Count);
				}
			}
		}
	}

	protected void UpdateListingsCategoriesData()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		int i;
		ExpansionP2PMarketCategoryListings cd;
		//! Reset all counts
		foreach (int categoryIndex, array<ref ExpansionP2PMarketCategoryListings> categoryDataArray: m_CategoryListings)
		{
			for (i = 0; i < categoryDataArray.Count(); ++i)
			{
				cd = categoryDataArray[i];
				if (!cd) 
					continue;
				
				cd.m_Count = 0;
			}
		}
		
		foreach (int traderID, map<int, ref array<ref ExpansionP2PMarketCategoryListings>> traderCategories: m_TraderCategoryListings)
		{
			foreach (int tCategoryIndex, array<ref ExpansionP2PMarketCategoryListings> tCategoryDataArray: traderCategories)
			{
				for (i = 0; i < tCategoryDataArray.Count(); ++i)
				{
					cd = tCategoryDataArray[i];
					if (!cd) 
						continue;
					
					cd.m_Count = 0;
				}
			}
		}

		foreach (int lTraderID, array<ref ExpansionP2PMarketListing> listings: m_ListingsData)
		{
			UpdateTraderListingsCategoriesData(lTraderID, listings);
		}
	}
	
	protected void UpdateTraderListingsCategoriesData(int traderID, array<ref ExpansionP2PMarketListing> listings)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		//! We go through all listings and determinine its category and sub-category and count them.
		for (int i = 0; i < listings.Count(); ++i)
		{
			ExpansionP2PMarketListing listing = listings[i];
			if (!listing || listing.GetListingState() != ExpansionP2PMarketListingState.LISTED)
				continue;
			
			if(!UpdateListingCategoryData(listing))
			{
				EXError.Warn(this, "::UpdateTraderListingsCategoriesData - Could not find any matching P2P market category for listing item: " + listing.GetClassName());
			}
		}
	}
	
	protected bool UpdateListingCategoryData(ExpansionP2PMarketListing listing, bool removed = false)
	{
		P2PDebugPrint("UpdateListingCategoryData called for listing: " + listing.GetClassName() + " | Trader ID: " + listing.GetTraderID());

		int traderID = listing.GetTraderID();
		if (traderID <= 0)
		{
			EXError.Error(this, "::UpdateListingCategoryData - Listing data has invalid trader ID " + traderID + "!");
			return false;
		}

		bool updatedTrader = ExUpdateListingCategoryData(listing, removed, m_CategoryListings);
		bool updatedGlobal = ExUpdateListingCategoryData(listing, removed, m_TraderCategoryListings[traderID], false);

		return (updatedGlobal && updatedTrader);
	}
	
	protected bool ExUpdateListingCategoryData(ExpansionP2PMarketListing listing, bool removed, map<int, ref array<ref ExpansionP2PMarketCategoryListings>> categoryMap, bool updateListing = true)
	{
		if (!m_P2PMarketSettings)
		{
			EXError.Error(this, "::ExUpdateListingCategoryData - Could not get P2P market settings!");
			return false;
		}
		
		if (!m_P2PMarketSettings.MenuCategories)
		{
			ErrorEx("Could not get menu categories from settings!", ErrorExSeverity.ERROR);
			return false;
		}

		if (!categoryMap)
		{
			ErrorEx("Could not get category map!", ErrorExSeverity.ERROR);
			return false;
		}
		
		string className = listing.GetClassName();
		foreach (ExpansionP2PMarketMenuCategory menuCategory: m_P2PMarketSettings.MenuCategories)
		{
			if (!menuCategory)
				continue;
			
			int categoryIndex = menuCategory.GetCategoryIndex();
			string categoryName = menuCategory.GetDisplayName();

			array<ref ExpansionP2PMarketCategoryListings> categoryDataArray = categoryMap[categoryIndex];
			if (!categoryDataArray)
			{
				ErrorEx("Could not get category data array for index " + categoryIndex, ErrorExSeverity.ERROR);
				continue;
			}
			else
			{
				P2PDebugPrint("Found category data array for index " + categoryIndex + " ...");
			}
			
			// Pre-check inclusion/exclusion logic once
			TStringArray included = menuCategory.GetIncluded();
			TStringArray excluded = menuCategory.GetExcluded();

			bool isIncluded = !included || included.Count() == 0 || ExpansionStatic.IsAnyOf(className, included);
			bool isExcluded = excluded && excluded.Count() > 0 && ExpansionStatic.IsAnyOf(className, excluded);

			if (!isIncluded || isExcluded)
			{
				P2PDebugPrint("Skipping " + className + " for category " + categoryIndex + " | Name: " + categoryName);
				continue;
			}
			else
			{
				P2PDebugPrint("Adding " + className + " to category " + categoryIndex + " | Name: " + categoryName);
			}

			bool foundData = false;
			bool foundSubData = false;

			foreach (ExpansionP2PMarketCategoryListings categoryData: categoryDataArray)
			{
				if (!categoryData || categoryData.m_CategoryIndex != categoryIndex || categoryData.m_SubCategoryIndex != -1)
					continue;
				
				P2PDebugPrint("Before update - categoryIndex: " + categoryIndex + " count: " + categoryData.m_Count);
				
				if (removed)
				{
					categoryData.m_Count = Math.Max(0, categoryData.m_Count - 1);
				}
				else
				{
					categoryData.m_Count++;
				}
				
				P2PDebugPrint("After update - categoryIndex: " + categoryIndex + " count: " + categoryData.m_Count);

				foundData = true;
				break;
			}
			
			if (updateListing)
				listing.SetCategoryIndex(categoryIndex);

			// Process subcategories using direct lookup
			array<ref ExpansionP2PMarketMenuSubCategory> subCategories = menuCategory.GetSubCategories();
			foreach (ExpansionP2PMarketMenuSubCategory menuSubCategory: subCategories)
			{
				if (!menuSubCategory)
					continue;

				int subCategoryIndex = menuSubCategory.GetSubCategoryIndex();
				string subCategoryName = menuSubCategory.GetDisplayName();

				// Pre-check inclusion/exclusion logic once
				TStringArray subIncluded = menuSubCategory.GetIncluded();
				TStringArray subExcluded = menuSubCategory.GetExcluded();

				bool isSubIncluded = !subIncluded || ExpansionStatic.IsAnyOf(className, subIncluded);
				bool isSubExcluded = subExcluded && ExpansionStatic.IsAnyOf(className, subExcluded);

				if (!isSubIncluded || isSubExcluded)
				{
					P2PDebugPrint("Skipping " + className + " for sub category " + subCategoryIndex + " | Name: " + subCategoryName);
					continue;
				}
				else
				{
					P2PDebugPrint("Adding " + className + " to sub category " + subCategoryIndex + " | Name: " + subCategoryName);
				}
				
				foreach (ExpansionP2PMarketCategoryListings subCategoryData: categoryDataArray)
				{
					if (!subCategoryData || subCategoryData.m_SubCategoryIndex != subCategoryIndex)
						continue;
					
					P2PDebugPrint("Before update - subCategoryIndex: " + subCategoryIndex + " count: " + subCategoryData.m_Count);
					
					if (removed)
					{
						subCategoryData.m_Count = Math.Max(0, subCategoryData.m_Count - 1);
					}
					else
					{
						subCategoryData.m_Count++;
					}
					
					P2PDebugPrint("After update - subCategoryIndex: " + subCategoryIndex + " count: " + subCategoryData.m_Count);

					foundSubData = true;
					break;
				}
				
				if (updateListing)
					listing.SetSubCategoryIndex(subCategoryIndex);

				if (foundSubData)
					break;
			}

			if (foundData)
				break;
		}

		return (foundData || foundSubData);
	}

	protected void CreateDefaultP2PTraderConfig()
	{
		string worldname;
		g_Game.GetWorldName(worldname);
		worldname.ToLower();

		vector mapPos = GetDayZGame().GetWorldCenterPosition();
		bool addToConfigArray = false;
		ExpansionP2PMarketTraderConfig bmTrader01;
		
		bmTrader01 = new ExpansionP2PMarketTraderConfig();
		bmTrader01.SetID(1);
		#ifdef EXPANSIONMODAI
		bmTrader01.SetClassName("ExpansionP2PTraderAIIrena");
		#else
		bmTrader01.SetClassName("ExpansionP2PTraderIrena");
		#endif
		bmTrader01.SetLoadoutFile("YellowKingLoadout");
		bmTrader01.AddCurrency("expansionbanknotehryvnia");

		bmTrader01.m_RequiredFaction = "";
		bmTrader01.m_UseReputation = false;
		bmTrader01.m_MinRequiredReputation = 0;
		bmTrader01.m_MaxRequiredReputation = int.MAX;
		bmTrader01.m_RequiredCompletedQuestID = -1;
		
		if (worldname.IndexOf("chernarus") > -1)
		{
			bmTrader01.SetPosition(Vector(3697.77, 402.012, 5971.12));
			bmTrader01.SetOrientation(Vector(150.132, 0, 0));
			bmTrader01.SetVehicleSpawnPosition(Vector(3728.44, 401.666, 6011.51));
			addToConfigArray = true;
		}
		else if (worldname.IndexOf("namalsk") > -1)
		{
			bmTrader01.SetPosition(Vector(3696.6, 402.012, 5970.54));
			bmTrader01.SetOrientation(Vector(156.132, 0, -0));
			bmTrader01.SetVehicleSpawnPosition(Vector(3741.68, 402.833, 5996.14));
			addToConfigArray = true;
		}
		else
		{
			//! @note: NPC entity is not spawned here as its just a config template.
			bmTrader01.SetPosition(mapPos);
			bmTrader01.SetOrientation(Vector(0, 0, 0));
			bmTrader01.SetVehicleSpawnPosition(mapPos);
		}
		
		ExpansionP2PMarketTraderConfig.Save(bmTrader01);
		if (addToConfigArray)
			m_P2PTraderConfig.Insert(1, bmTrader01);
	}

	protected void LoadP2PMarketTraderData(string fileName, string path)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		//! Load and cache trader config data
		ExpansionP2PMarketTraderConfig traderConfig = ExpansionP2PMarketTraderConfig.Load(path + fileName);
		if (!traderConfig)
			return;

		if (m_P2PTraderConfig.Contains(traderConfig.GetID()))
		{
			EXError.Error(this, "::LoadP2PMarketTraderData - WARNING - There is already trader data loaded for a trader with ID=" + traderConfig.GetID() + ". Skip adding data for trader config file=" + path + fileName);
			return;
		}

		m_P2PTraderConfig.Insert(traderConfig.GetID(), traderConfig);
		
		//! Load listing data (moves also listing and related entity storage files from old to new locations)
		int traderID = traderConfig.GetID();
		bool loadedFromOldLoc = false;
		string traderListingsPath = GetP2PMarketDataDirectory() + "P2PTrader_" + traderID + "_Listings\\"; //! Old trader listings path
		if (!FileExist(traderListingsPath))
		{
			//! Load from new location
			traderListingsPath = GetP2PMarketDataDirectory() + "traderID_" + traderID + "\\listings\\"; //! New trader listings path
			if (!FileExist(traderListingsPath))
				return;
		}
		else
		{
			loadedFromOldLoc = true;
		}
		
		array<string> traderListings = ExpansionStatic.FindFilesInLocation(traderListingsPath, ".json");
		if (traderListings && traderListings.Count() > 0)
		{
			foreach (string listingFileName: traderListings)
			{
				string filePath = (traderListingsPath + listingFileName);
				if (!FileExist(filePath))
				{
					EXError.Error(this, "::LoadP2PMarketTraderData - Could not find and load P2P trader listing file=" + filePath);
					continue;
				}

				LoadListingData(traderID, listingFileName, traderListingsPath);
			}
		}
		
		//! Delete old trader listings folder
		if (loadedFromOldLoc)
		{
			ExpansionStatic.DeleteFiles(traderListingsPath, traderListings);
			DeleteFile(traderListingsPath);
		}
	}

	protected void LoadListingData(int traderID, string fileName, string path)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 

		string filePath = (path + fileName);
		ExpansionP2PMarketListing listingData = ExpansionP2PMarketListing.Load(filePath, traderID);
		if (!listingData)
		{
			EXError.Error(this, "::LoadListingData - Could not load listing data file with path=" + filePath);
			return;
		}

		ExpansionP2PMarketCounters counters;

		array<ref ExpansionP2PMarketListing> listings;
		ExpansionP2PMarketListingState listingState = listingData.GetListingState(); 
		switch (listingState)
		{
			case ExpansionP2PMarketListingState.LISTED:
			{
				//! We only want to check and load the entity storage .bin file when the listing state is on LISTED and not on SOLD
				string listingESFilePath = listingData.GetEntityStorageFileName();
				if (!FileExist(listingESFilePath))
				{
					EXError.Error(this, "::LoadListingData - Entity storage file " + listingESFilePath + " does not exist anymore! Deleting listing JSON");
					if (FileExist(filePath))
						DeleteFile(filePath); //! Delete the listing JSON file.
					return;
				}

				counters = GetPlayerDataCounters(listingData.GetOwnerUID());

				if (!m_ListingsData.Find(traderID, listings))
				{
					listings = {};
					m_ListingsData.Insert(traderID, listings);
				}

				listings.Insert(listingData);
				m_ListingsCount++;
				counters.m_OwnedListingsCount++;

				break;
			}
			case ExpansionP2PMarketListingState.SOLD:
			{
				counters = GetPlayerDataCounters(listingData.GetOwnerUID());

				if (!m_SoldListingsData.Find(traderID, listings))
				{
					listings = {};
					m_SoldListingsData.Insert(traderID, listings);
				}

				listings.Insert(listingData);
				counters.m_SoldListingsCount++;
				counters.m_SoldTotalIncome += listingData.GetPrice();

				break;
			}
			default:
			{
				EXError.Error(this, "::LoadListingData - Listing state of loaded listing is invalid! Path=" + filePath);
				return;
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------
	//! RPC Event classes
	// ------------------------------------------------------------------------------------------------------------------------
	//! Server
	void AddTradingPlayer(int traderID, string playerUID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		m_TradingPlayers.Set(playerUID, traderID);
		
		ExpansionP2PMarketCounters counters = GetPlayerDataCounters(playerUID);
	}

	//! Client
	void RemoveTradingPlayer(string playerUID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		auto rpc = Expansion_CreateRPC("RPC_RemoveTradingPlayer");
		rpc.Write(playerUID);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RemoveTradingPlayer(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		string playerUID;
		if (!ctx.Read(playerUID))
			return;

		int traderID;
		if (m_TradingPlayers.Find(playerUID, traderID))
			m_TradingPlayers.Remove(playerUID);
	}
	
	//! Client
	void GetSaleFromListing(TIntArray globalID, int traderID, int pageIndex)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		auto rpc = Expansion_CreateRPC("RPC_RequestSaleFromListing");
		rpc.Write(traderID);
		rpc.Write(globalID);
		rpc.Write(pageIndex);
		rpc.Expansion_Send(true);
	}
	
	//! Server
	protected void RPC_RequestSaleFromListing(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		int traderID;
		if (!ctx.Read(traderID))
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Couldn't read trader ID!");
			return;
		}

		TIntArray globalID;
		if (!ctx.Read(globalID))
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Couldn't read global ID!");
			return;
		}
		
		ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(traderID);
		if (!traderConfig)
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Could not get P2P trader data for ID " + traderID);
			return;
		}
		
		ExpansionP2PMarketListing listing = GetSoldListingByGlobalID(traderID, globalID, traderConfig.IsGlobalTrader());
		string globalIDText = ExpansionStatic.IntToHex(globalID);
		if (!listing)
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Couldn't find listing with global ID " + globalIDText);
			ExpansionNotification("RPC_RequestSaleFromListing", "Couldn't find listing with global ID " + globalIDText).Error(identity);
			return;
		}
		
		int listingTraderID = listing.GetTraderID();
		if (listingTraderID <= -1)
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Couldn't find listing with global ID " + globalIDText);
			ExpansionNotification("RPC_RequestSaleFromListing", "Couldn't find listing with global ID " + globalIDText).Error(identity);
			return;
		}
		
		string playerUID = identity.GetId();
		if (listing.GetOwnerUID() != playerUID)
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - player " + playerUID + " tried to retrieve profits from a listing that is not his own at trader ID " + traderID);
			ExpansionNotification("RPC_RequestSaleFromListing", "You can't retrieve another player's profits").Error(identity);
			return;
		}
		
		int pageIndex;
		if (!ctx.Read(pageIndex))
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Couldn't read page index!");
			return;
		}

		PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(identity);
		if (!player)
			return;
		
		ExpansionP2PMarketCounters counters = GetPlayerDataCounters(playerUID);

		int price = listing.GetPrice();
		if (price <= 0)
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - player " + playerUID + " tried to retrieve profits from a listing that has a price of 0. Price: " + price);
			ExpansionNotification("RPC_RequestSaleFromListing", "You can't retrieve profits from a listing with a price of zero.").Error(identity);
			return;
		}
	
		string className = listing.GetClassName();
		if (className == "")
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - player " + playerUID + " tried to retrieve profits from a listing that has no type name. Class name: " + className);
			ExpansionNotification("RPC_RequestSaleFromListing", "You can't retrieve profits from a listing with no associated item(s).").Error(identity);
			return;
		}

		if (!RemoveSoldListingByGlobalID(listingTraderID, globalID, traderConfig.IsGlobalTrader(), false, true))
		{
			EXError.Error(this, "::RPC_RequestSaleFromListing - Could not remove sold listing " + globalIDText);
			ExpansionNotification("RPC_RequestSaleFromListing", "Could not remove sold listing " + globalIDText).Error(identity);
			return;
		}
		
		EntityAI playerEntity = player;
		TStringArray currencies = traderConfig.GetCurrencies();
		m_MarketModule.SpawnMoneyInCurrency(player, playerEntity, price, currencies, false);
		
		if (counters.m_SoldListingsCount > 0)
		{
			ExpansionP2PMarketRequestData data = new ExpansionP2PMarketRequestData();
			data.m_TraderID = traderID;
			data.m_PageIndex = pageIndex;
			data.m_CallbackType = ExpansionP2PMarketModuleCallback.SaleRetrieved;
			data.m_MessageTypeName = className;
			data.m_MessagePrice = price;
			
			SendBasicSoldListingData(data, identity);
		}
		else
		{
			Callback(identity, ExpansionP2PMarketModuleCallback.SaleRetrieved, className, price);
		}

		if (GetExpansionSettings().GetLog().Market)
		{
			string priceStringLog = string.Format("%1 (%2)", price, GetDisplayPrice(traderConfig, price, false, false, true));
			GetExpansionSettings().GetLog().PrintLog("[P2P Market] Player \"" + identity.GetName() + "\" (id=" + identity.GetId() + ")" + " has retrieved the sale of \"" + className + "\" for a price of " + priceStringLog + " (globalID=" + globalIDText + ")");
		}
	}
	
	//! Client
	void RequestAllPlayerSales(int traderID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		auto rpc = Expansion_CreateRPC("RPC_RequestAllPlayerSales");
		rpc.Write(traderID);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestAllPlayerSales(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		int traderID;
		if (!ctx.Read(traderID))
			return;

		if (traderID == -1)
		{
			EXError.Error(this, "::RPC_RequestAllPlayerSales - Trader ID is -1 (configuration error)");
			ExpansionNotification("RPC_RequestAllPlayerSales", "Trader ID is -1 (configuration error)").Error(identity);
			return;
		}
	
		string playerUID = identity.GetId();
		PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
		if (!player)
			return;
		
		ExpansionP2PMarketCounters counters = GetPlayerDataCounters(playerUID);
		
		ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(traderID);
		if (!traderConfig)
		{
			EXError.Error(this, "::RPC_RequestAllPlayerSales - Could not get P2P trader data for ID " + traderID);
			return;
		}

		map<int, ref array<ref ExpansionP2PMarketListing>> listingsData;
		if (!traderConfig.IsGlobalTrader())
		{
			//! If it's not a global trader, we have one entry for this trader + its listings
			listingsData = new map<int, ref array<ref ExpansionP2PMarketListing>>;
			listingsData[traderID] = m_SoldListingsData[traderID];
			if (!listingsData[traderID])
			{
				EXError.Error(this, "::RPC_RequestAllPlayerSales - No listings for trader ID " + traderID);
				ExpansionNotification("RPC_RequestAllPlayerSales", "No listings for trader ID " + traderID).Error(identity);
				return;
			}
		}
		else
		{
			//! If it's a global trader, we have an entry for each trader + its listings
			listingsData = m_SoldListingsData;
		}

		int sold;
		int price;
		string globalIDText;

		foreach (int listingsTraderID, array<ref ExpansionP2PMarketListing> traderListings: listingsData)
		{
			for (int j = traderListings.Count() - 1; j >= 0; --j)
			{
				ExpansionP2PMarketListing listing = traderListings[j];
				globalIDText = ExpansionStatic.IntToHex(listing.GetGlobalID());
				if (listing.GetListingState() == ExpansionP2PMarketListingState.SOLD && listing.GetOwnerUID() == playerUID)
				{
					sold++;
					price += listing.GetPrice();
					if (!RemoveListing(listing, traderListings, j, false, true, true))
					{
						EXError.Error(this, "::RPC_RequestAllPlayerSales - Could not remove listing data for listing " + globalIDText + " at trader ID " + listingsTraderID);
					}
				}
			}
		}

		if (sold == 0)
		{
			EXError.Error(this, "::RPC_RequestAllPlayerSales - No listings in SOLD state for player " + playerUID + " at trader ID " + traderID);
			ExpansionNotification("RPC_RequestAllPlayerSales", "You have no sold listings at this trader").Error(identity);
			return;
		}

		EntityAI playerEntity = player;
		TStringArray currencies = traderConfig.GetCurrencies();
		m_MarketModule.SpawnMoneyInCurrency(player, playerEntity, price, currencies, false);
		
		Callback(identity, ExpansionP2PMarketModuleCallback.AllSalesRetrieved, "", price);
		
		if (GetExpansionSettings().GetLog().Market)
		{
			string priceStringLog = string.Format("%1 (%2)", price, GetDisplayPrice(traderConfig, price, false, false, true));
			GetExpansionSettings().GetLog().PrintLog("[P2P Market] Player \"" + identity.GetName() + "\" (id=" + identity.GetId() + ")" + " has retrieved " + sold + " sales for a total of " + priceStringLog + ".");
		}
	}
	
	//! Server
	void SendUpdatedTraderData(ExpansionP2PMarketRequestData data, string senderUID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		data.Debug();
		
		foreach (string playerUID, int id: m_TradingPlayers)
		{
			ErrorEx("Player UID: " + playerUID + " | Trader ID: " + id, ErrorExSeverity.INFO);		
			ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(id);
			if (!traderConfig)
			{
				EXError.Error(this, "::SendUpdatedTraderData - Could not get P2P trader data for ID " + id);
				return;
			}

			if (traderConfig.IsGlobalTrader() || id == data.m_TraderID || data.m_ListingTraderID > -1 && id == data.m_ListingTraderID)
			{
				if (playerUID != senderUID)
					data.m_CallbackType = ExpansionP2PMarketModuleCallback.Silent;
				
				PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
				if (!player)
					continue;
				
				if (playerUID == senderUID)
				{
					Callback(player.GetIdentity(), data.m_CallbackType, data.m_MessageTypeName, data.m_MessagePrice, data.m_MessagePriceString);
				}
	
				ListingDataChanged(player.GetIdentity(), data.m_GlobalIDText);
				ErrorEx("Send updated player data to player! Player UID: " + playerUID + " | Trader ID: " + id, ErrorExSeverity.INFO);
			}
			#ifdef DIAG_DEVELOPER
			else
			{
				ErrorEx("Skipped sending updated player data to player! Player UID: " + playerUID + " | Trader ID: " + id, ErrorExSeverity.INFO);
			}
			#endif
		}
	}
	
	//! Client
	void RequestBasicListingData(int traderID, int pageIndex, bool soldListings, int categoryIndex = -1, int subCategoryIndex = -1, array<string> searchTypeNames = null, bool ownedListings = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		auto rpc = Expansion_CreateRPC("RPC_RequestBasicListingData");
		rpc.Write(traderID);
		rpc.Write(pageIndex);
		rpc.Write(soldListings);
		rpc.Write(categoryIndex);
		rpc.Write(subCategoryIndex);
		rpc.Write(searchTypeNames);
		rpc.Write(ownedListings);
		rpc.Expansion_Send(true);
	}
	
	//! Server
	protected void RPC_RequestBasicListingData(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		int traderID = -1;
		if (!ctx.Read(traderID))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read trader ID!");
			return;
		}
		
		int pageIndex = 0;
		if (!ctx.Read(pageIndex))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read page index!");
			return;
		}
		
		bool soldListings = false;
		if (!ctx.Read(soldListings))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read sold listings boolean!");
			return;
		}
		
		int categoryIndex = -1;
		if (!ctx.Read(categoryIndex))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read category index!");
			return;
		}
		
		int subCategoryIndex = -1;
		if (!ctx.Read(subCategoryIndex))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read sub-category index!");
			return;
		}
		
		array<string> searchTypeNames = new array<string>;
		if (!ctx.Read(searchTypeNames))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read seach type names!");
			return;
		}
		
		bool ownedListings = false;
		if (!ctx.Read(ownedListings))
		{
			EXError.Error(this, "::RPC_RequestBasicListingData - Couldn't read owned listings bolean!");
			return;
		}

		ExpansionP2PMarketRequestData data = new ExpansionP2PMarketRequestData();
		data.m_TraderID = traderID;
		data.m_PageIndex = pageIndex;
		data.m_CategoryIndex = categoryIndex;
		data.m_SubCategoryIndex = subCategoryIndex;
		data.m_SearchTypeNames = searchTypeNames;
		data.m_OwnedListings = ownedListings;
		
		SendCategoryListingsData(traderID, identity);
		
		if (!soldListings)
		{
			SendBasicListingData(data, identity);
		}
		else
		{
			SendBasicSoldListingData(data, identity);
		}
	}
	
	//! Server
	void SendCategoryListingsData(int traderID, PlayerIdentity identity)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(traderID);
		if (!traderConfig)
		{
			EXError.Error(this, "::SendCategoryListingsData - Could not get P2P trader data for ID " + traderID);
			return;
		}
		
		bool isGlobal = traderConfig.IsGlobalTrader();
		map<int, ref array<ref ExpansionP2PMarketCategoryListings>> dataMap;
		if (isGlobal)
		{
			dataMap = m_CategoryListings;
		}
		else
		{
			dataMap = m_TraderCategoryListings.Get(traderID);
		}
		
		if (!dataMap)
		{
			EXError.Error(this, "::SendCategoryListingsData - Could not get P2P category data!");
			return;
		}
		
		auto rpc = Expansion_CreateRPC("RPC_SendCategoryListingsData");
		rpc.Write(traderID);
		rpc.Write(isGlobal);
		int mapCount = dataMap.Count();
		rpc.Write(mapCount);
		
		foreach (int categoryIndex, array<ref ExpansionP2PMarketCategoryListings> dataArray: dataMap)
		{
			rpc.Write(categoryIndex);
			int dataCount = dataArray.Count();
			rpc.Write(dataCount);
			foreach (ExpansionP2PMarketCategoryListings data: dataArray)
			{
				data.OnSend(rpc);
			}
		}
				
		rpc.Expansion_Send(true, identity);
	}
	
	//! Client
	protected void RPC_SendCategoryListingsData(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		int traderID;
		if (!ctx.Read(traderID))
		{
			EXError.Error(this, "::RPC_SendCategoryListingsData - Couldn't read trader ID!");
			return;
		}

		bool isGlobal;
		if (!ctx.Read(isGlobal))
		{
			EXError.Error(this, "::RPC_SendCategoryListingsData - Couldn't read global trader boolean!");
			return;
		}

		int mapCount;
		if (!ctx.Read(mapCount))
		{
			EXError.Error(this, "::RPC_SendCategoryListingsData - Couldn't read category map data elements count!");
			return;
		}

		if (isGlobal)
		{
			if (!m_CategoryListings)
			{
				m_CategoryListings = new map<int, ref array<ref ExpansionP2PMarketCategoryListings>>;
			}
			else
			{
				m_CategoryListings.Clear();
			}
		}
		else
		{
			if (!m_TraderCategoryListings)
			{
				m_TraderCategoryListings = new map<int, ref map<int, ref array<ref ExpansionP2PMarketCategoryListings>>>;
			}
		}

		for (int i = 0; i < mapCount; ++i)
		{
			int categoryIndex = -1;
			if (!ctx.Read(categoryIndex))
			{
				EXError.Error(this, "::RPC_SendCategoryListingsData - Couldn't read category index!");
				return;
			}

			int dataCount;
			if (!ctx.Read(dataCount))
			{
				EXError.Error(this, "::RPC_SendCategoryListingsData - Couldn't read data count!");
				return;
			}

			array<ref ExpansionP2PMarketCategoryListings> dataArray = new array<ref ExpansionP2PMarketCategoryListings>;
			for (int j = 0; j < dataCount; ++j)
			{
				ExpansionP2PMarketCategoryListings data = new ExpansionP2PMarketCategoryListings();
				if (!data.OnRecieve(ctx))
				{
					EXError.Error(this, "::RPC_SendCategoryListingsData - Couldn't read ExpansionP2PMarketCategoryListings data! Index: " + categoryIndex);
					return;
				}

				dataArray.Insert(data);
			}

			if (isGlobal)
			{
				m_CategoryListings.Set(categoryIndex, dataArray);
			}
			else
			{
				//! Preserve existing trader categories instead of overwriting them
				if (!m_TraderCategoryListings.Contains(traderID))
				{
					m_TraderCategoryListings.Set(traderID, new map<int, ref array<ref ExpansionP2PMarketCategoryListings>>);
				}

				map<int, ref array<ref ExpansionP2PMarketCategoryListings>> traderMap = m_TraderCategoryListings.Get(traderID);
				traderMap.Set(categoryIndex, dataArray);
			}
		}
	}
	
	protected bool IsValidSearchListing(array<string> searchTypeNames, ExpansionP2PMarketListing listing)
	{
		string classNameLower;
		bool hasChildWithName, hasName;
		array<ref ExpansionP2PMarketContainerItem> containerItems = listing.GetContainerItems();
		if (containerItems && containerItems.Count() > 0)
		{
			for (int i = 0; i < containerItems.Count(); ++i)
			{
				ExpansionP2PMarketContainerItem containerItem = containerItems[i];
				classNameLower = containerItem.GetClassName();
				classNameLower.ToLower();

				foreach(int index, string cTN: searchTypeNames)
				{
					if (classNameLower.IndexOf(cTN) > -1)
						hasChildWithName = true;
				}
			}
		}

		classNameLower = listing.GetClassName();
		classNameLower.ToLower();

		foreach(string tN: searchTypeNames)
		{
			if (classNameLower.IndexOf(tN) > -1)
				hasName = true;
		}

		return (hasName || hasChildWithName);
	}
	
	//! Server
	void SendBasicListingData(ExpansionP2PMarketRequestData data, PlayerIdentity identity)
	{
	    #ifdef EXTRACE
	    auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
	    #endif
	    
	    data.Debug();
	
	    ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(data.m_TraderID);
	    if (!traderConfig)
	    {
	        EXError.Error(this, "::SendBasicListingData - Could not get P2P trader data for ID " + data.m_TraderID);
	        return;
	    }
	
	    ExpansionP2PMarketCounters counters;
	    m_PlayerDataCounters.Find(identity.GetId(), counters);
	
	    array<ref ExpansionP2PMarketListing> listingsToSend = new array<ref ExpansionP2PMarketListing>;
	
	    bool isGlobal = traderConfig.IsGlobalTrader();
	    
	    if (data.m_PageIndex > 0)
	    {
	        CheckPageIndex(data.m_PageIndex, data.m_TraderID, isGlobal, data.m_CategoryIndex, data.m_SubCategoryIndex, data.m_OwnedListings, identity.GetId());
	    }
	    
		int listingStartIndex = data.m_PageIndex * LISTINGS_PER_PAGE_COUNT; // Where to start fetching listings
		int listingsCount = 0; // Tracks how many valid listings we've considered so far
		int traderListingsCount = 0;
		int validListingsCount = 0;
	
	    P2PDebugPrint("Listing start index: " + listingStartIndex);
	    
	    //! Iterate through all available listings and collect the next 14 based on conditions
	    foreach (int traderID, array<ref ExpansionP2PMarketListing> listingData: m_ListingsData)
	    {
	        P2PDebugPrint("Trader ID: " + traderID + " | Listings count: " + listingData.Count());	
	        if (!isGlobal && traderID != data.m_TraderID)
	            continue; // Skip traders that aren't the requested trader
	
	        if (traderID == data.m_TraderID)
	            traderListingsCount = listingData.Count();
	
	        // Loop through the trader's listings and filter them
	        for (int i = 0; i < listingData.Count(); ++i)
	        {
	            ExpansionP2PMarketListing listing = listingData[i];
	
	            // Skip invalid listings
	            if (!listing)
	            {
					P2PDebugPrint("[S1] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
	                continue;
	            }

				if (data.m_OwnedListings && listing.GetOwnerUID() != identity.GetId())
				{
					P2PDebugPrint("[S2] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
					continue;
				}

				if ((data.m_CategoryIndex > -1 && listing.GetCategoryIndex() != data.m_CategoryIndex) || (data.m_SubCategoryIndex > -1 && listing.GetSubCategoryIndex() != data.m_SubCategoryIndex))
				{
					P2PDebugPrint("[S3] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
					continue;
				}

				if ((data.m_SearchTypeNames.Count() > 0 && !IsValidSearchListing(data.m_SearchTypeNames, listing)))
	            {
					P2PDebugPrint("[S4] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
	                continue;
	            }
				
				validListingsCount++;
				
	            // Only count valid listings
	            if (listingsCount < listingStartIndex)
	            {
					P2PDebugPrint("[S5] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
	                listingsCount++;
	                continue; // Skip listings from previous pages
	            }
	
	            // Add valid listings to the response
	            if (listingsToSend.Find(listing) == -1 && listingsToSend.Count() < LISTINGS_PER_PAGE_COUNT)
	            {
					P2PDebugPrint("Send listing: " + listing + " | Item: " + listing.GetClassName() + " | Current listings count: " + listingsCount);
	                listingsToSend.Insert(listing);
	            }
	        }
	    }
		
		int currentTotalListings;
        if (isGlobal)
        {
            currentTotalListings = m_ListingsCount;
            P2PDebugPrint("Global listings count: " + currentTotalListings);
        }
        else
        {
            currentTotalListings = traderListingsCount;
            P2PDebugPrint("Trader listings count: " + currentTotalListings);
        }
	
	    TStringArray currencies = traderConfig.GetCurrencies();
	    
	    auto rpc = Expansion_CreateRPC("RPC_SendBasicListingData");
	    rpc.Write(false);
	    rpc.Write(data.m_Init);
	    rpc.Write(isGlobal);
	
	    if (data.m_Init)
	    {
	        rpc.Write(data.m_TraderID);
	        rpc.Write(traderConfig.m_DisplayName);
	        rpc.Write(traderConfig.m_DisplayIcon);
	        rpc.Write(currencies);
	        rpc.Write(traderConfig.m_DisplayCurrencyValue);
	        rpc.Write(traderConfig.m_DisplayCurrencyName);
	    }
	
	    rpc.Write(data.m_CallbackType);
	    rpc.Write(data.m_MessageTypeName);
	    rpc.Write(data.m_MessagePrice);
	    rpc.Write(counters.m_SoldListingsCount);
	    rpc.Write(counters.m_OwnedListingsCount);
	    rpc.Write(currentTotalListings);
	    rpc.Write(listingsToSend.Count());
		rpc.Write(validListingsCount);
	
	    foreach (ExpansionP2PMarketListing listingToSend: listingsToSend)
	    {
	        listingToSend.OnSendBasic(rpc);
	    }
	
	    rpc.Expansion_Send(true, identity);
	}
	
	//! Server	
	void SendBasicSoldListingData(ExpansionP2PMarketRequestData data, PlayerIdentity identity)
	{
	    #ifdef EXTRACE
	    auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
	    #endif
	    
	    data.Debug();
	
	    ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(data.m_TraderID);
	    if (!traderConfig)
	    {
	        EXError.Error(this, "::SendBasicSoldListingData - Could not get P2P trader data for ID " + data.m_TraderID);
	        return;
	    }
	
	    ExpansionP2PMarketCounters counters;
	    m_PlayerDataCounters.Find(identity.GetId(), counters);
	
	    array<ref ExpansionP2PMarketListing> listingsToSend = new array<ref ExpansionP2PMarketListing>;
	
	    bool isGlobal = traderConfig.IsGlobalTrader();
	    
	    if (data.m_PageIndex > 0)
	    {
	        CheckPageIndex(data.m_PageIndex, data.m_TraderID, isGlobal, data.m_CategoryIndex, data.m_SubCategoryIndex, data.m_OwnedListings, identity.GetId());
	    }
	    
	    int listingStartIndex = data.m_PageIndex * LISTINGS_PER_PAGE_COUNT; // Where to start fetching listings
	    int listingsCount = 0; // Tracks how many valid listings we've considered so far
	    int traderListingsCount = 0;
		int validListingsCount = 0;
	
	    P2PDebugPrint("Listing start index: " + listingStartIndex);
	    
	    //! Iterate through all available listings and collect the next 14 based on conditions
	    foreach (int traderID, array<ref ExpansionP2PMarketListing> listingData: m_SoldListingsData)
	    {
	        P2PDebugPrint("Trader ID: " + traderID + " | Listings count: " + listingData.Count());	
	        if (!isGlobal && traderID != data.m_TraderID)
	            continue; // Skip traders that aren't the requested trader
	
	        if (traderID == data.m_TraderID)
	            traderListingsCount = listingData.Count();
	
	        // Loop through the trader's listings and filter them
	        for (int i = 0; i < listingData.Count(); ++i)
	        {
	            ExpansionP2PMarketListing listing = listingData[i];
	
	            // Skip invalid listings
	            if (!listing)
	            {
					P2PDebugPrint("[S1] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
	                continue;
	            }

				if (listing.GetOwnerUID() != identity.GetId())
				{
					P2PDebugPrint("[S2] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
					continue;
				}
	
	            // Only count valid listings
	            if (listingsCount < listingStartIndex)
	            {
					P2PDebugPrint("[S5] - Skipping listing: " + listing.GetClassName() + " | Current count: " + listingsCount + " | Start index: " + listingStartIndex);
	                listingsCount++;
	                continue; // Skip listings from previous pages
	            }
				
				validListingsCount++;
				
	            // Add valid listings to the response
	            if (listingsToSend.Find(listing) == -1 && listingsToSend.Count() < LISTINGS_PER_PAGE_COUNT)
	            {
					P2PDebugPrint("Send listing: " + listing + " | Item: " + listing.GetClassName() + " | Current listings count: " + listingsCount);
	                listingsToSend.Insert(listing);
	            }
	        }
	    }
		
		int currentTotalListings;
        if (isGlobal)
        {
            currentTotalListings = m_ListingsCount;
            P2PDebugPrint("Global listings count: " + currentTotalListings);
        }
        else
        {
            currentTotalListings = traderListingsCount;
            P2PDebugPrint("Trader listings count: " + currentTotalListings);
        }
	
	    TStringArray currencies = traderConfig.GetCurrencies();
	    
	    auto rpc = Expansion_CreateRPC("RPC_SendBasicListingData");
		rpc.Write(true);
		rpc.Write(false);
		rpc.Write(isGlobal);
		rpc.Write(data.m_CallbackType);
		rpc.Write(data.m_MessageTypeName);
		rpc.Write(data.m_MessagePrice);
		rpc.Write(counters.m_SoldListingsCount);
		rpc.Write(counters.m_SoldTotalIncome);
		rpc.Write(counters.m_OwnedListingsCount);
		rpc.Write(currentTotalListings);
		rpc.Write(listingsToSend.Count());
		rpc.Write(validListingsCount);
		
		foreach (ExpansionP2PMarketListing listingToSend: listingsToSend)
		{
			listingToSend.OnSendBasic(rpc);
		}

		rpc.Expansion_Send(true, identity);
	}
	
	//! Check if given page index is not to high after receiving a silent update and the page the player is currently on in the menu is not valid anymore because the amount of pages has changed (e.g., listing was brought).
	protected bool CheckPageIndex(inout int pageIndex, int traderID, bool isGlobal, int categoryIndex = -1, int subCategoryIndex = -1, bool ownedListings = false, string playerUID = "")
	{
		P2PDebugPrint("Page index:" + pageIndex + " | Trader ID: " + traderID + " | Is global: " + isGlobal + " | Category index: " + categoryIndex + " | Sub-Category index: " + subCategoryIndex + " | Owned listings: " + ownedListings);
		
		int allListingsCount;
		if (categoryIndex > -1 || subCategoryIndex > -1)
		{
			array<ref ExpansionP2PMarketCategoryListings> categoryListings;
			if (isGlobal)
			{
				categoryListings = m_CategoryListings[categoryIndex];
			}
			else
			{
				map<int, ref array<ref ExpansionP2PMarketCategoryListings>> dataMap = m_TraderCategoryListings[traderID];
				categoryListings = dataMap[categoryIndex];
			}
			
			if (subCategoryIndex > -1)
			{
				foreach(ExpansionP2PMarketCategoryListings cl: categoryListings)
				{
					if (cl.m_SubCategoryIndex == subCategoryIndex)
					{
						allListingsCount = cl.m_Count;
						break;
					}
				}
			}
			else
			{
				allListingsCount = categoryListings[0].m_Count;
			}
		}
		else if (ownedListings && playerUID != "")
		{
			ExpansionP2PMarketCounters counters;
			m_PlayerDataCounters.Find(playerUID, counters);
			allListingsCount = counters.m_OwnedListingsCount;
		}
		else
		{
			if (isGlobal)
			{
				allListingsCount = m_ListingsCount;
			}
			else
			{
				allListingsCount = m_ListingsData[traderID].Count();
			}
		}
		
		P2PDebugPrint("All listings count: " + allListingsCount);
		if (allListingsCount > 0)
		{
			int pages = allListingsCount / LISTINGS_PER_PAGE_COUNT;
			if ((pages * LISTINGS_PER_PAGE_COUNT) < allListingsCount)
				pages += 1;

			P2PDebugPrint("Pages: " + pages + " | Page index: " + pageIndex);
			if (pages < (pageIndex + 1))
			{
				pageIndex = Math.Max(0, pages - 1);
				P2PDebugPrint("Modified page index: " + pageIndex);
				return true;
			}
		}
		
		return false;
	}

	//! Client
	protected void RPC_SendBasicListingData(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		bool soldListings = false;
		if (!ctx.Read(soldListings))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read sold listings boolean!");
			return;
		}
		
		bool init = false;
		if (!ctx.Read(init))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read init boolean!");
			return;
		}
		
		bool isGlobal = false;
		if (!ctx.Read(isGlobal))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read global bolean!");
			return;
		}
		
		int traderID = -1;
		string traderName;
		string iconName;
		TStringArray currencies;
		int displayCurrencyValue;
		string displayCurrencyName;

		if (init)
		{
			if (!ctx.Read(traderID))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read trader ID!");
				return;
			}
			
			if (!ctx.Read(traderName))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read trader name!");
				return;
			}
			
			if (!ctx.Read(iconName))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read menu icon path!");
				return;
			}
			
			if (!ctx.Read(currencies))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read currencies!");
				return;
			}
	
			if (!ctx.Read(displayCurrencyValue))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read trader currency value!");
				return;
			}
	
			if (!ctx.Read(displayCurrencyName))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read trader currency name!");
				return;
			}
		}
		
		int callbackType;
		if (!ctx.Read(callbackType))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read callback type!");
			return;
		}

		string messageTypeName;
		if (!ctx.Read(messageTypeName))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read message type name!");
			return;
		}

		int messagePrice;
		if (!ctx.Read(messagePrice))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read message price!");
			return;
		}
		
		int soldListingsCount;
		if (!ctx.Read(soldListingsCount))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read sold listings count!");
			return;
		}
		
		int soldTotalIncome;
		if (soldListings)
		{
			if (!ctx.Read(soldTotalIncome))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read sold total icome!");
				return;
			}
		}
		
		int ownedListingsCount;
		if (!ctx.Read(ownedListingsCount))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read owned listings count!");
			return;
		}
		
		int currentTotalListings;
		if (!ctx.Read(currentTotalListings))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read current total listings count!");
			return;
		}

		int recivedListingsCount;
		if (!ctx.Read(recivedListingsCount))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read listings to send count!");
			return;
		}
		
		int validListingsCount;
		if (!ctx.Read(validListingsCount))
		{
			EXError.Error(this, "::RPC_SendBasicListingData - Couldn't read valid listings count!");
			return;
		}
		
		P2PDebugPrint("Current total listings count:" + currentTotalListings + " | Recived listings count: " + recivedListingsCount);

		array<ref ExpansionP2PMarketListing> listings = new array<ref ExpansionP2PMarketListing>;
		for (int i = 0; i < recivedListingsCount; ++i)
		{
			ExpansionP2PMarketListing listing = new ExpansionP2PMarketListing();
			if (!listing.OnRecieveBasic(ctx))
			{
				EXError.Error(this, "::RPC_SendBasicListingData - Couldn't receive listing " + i);
				return;
			}

			listings.Insert(listing);
		}

		ExpansionP2PMarketRecivedData data = new ExpansionP2PMarketRecivedData();
		data.m_Listings = listings;
		if (init)
		{
			data.m_TraderID = traderID;
			data.m_IsGlobal = isGlobal;
			data.m_TraderName = traderName;
			data.m_IconName = iconName;
			data.m_Currencies = currencies;
			data.m_DisplayCurrencyValue = displayCurrencyValue;
			data.m_DisplayCurrencyName = displayCurrencyName;
		}
		
		data.m_ListingsCount = currentTotalListings;
		data.m_SoldListingsCount = soldListingsCount;
		data.m_SoldTotalIncome = soldTotalIncome;
		data.m_OwnedListingsCount = ownedListingsCount;
		data.m_ValidListingsCount = validListingsCount;
		
		m_ListingsInvoker.Invoke(data, soldListings);
		if (callbackType != ExpansionP2PMarketModuleCallback.None)
			m_CallbackInvoker.Invoke(callbackType, messageTypeName, messagePrice, 0, null);
	}

	//! Client
	void RequestListItem(int traderID, Entity item, int price)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		P2PDebugPrint("Trader ID: " + traderID + " | Item: " + item + " | Price " + price);
		
		auto rpc = Expansion_CreateRPC("RPC_RequestListItem");
		rpc.Write(traderID);
		rpc.Write(price);
		rpc.Expansion_Send(item, true);
	}

	//! Server
	protected void RPC_RequestListItem(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		int traderID;
		if (!ctx.Read(traderID))
		{
			EXError.Error(this, "::RPC_RequestListItem - Could not read traderID.");
			return;
		}

		int price;
		if (!ctx.Read(price))
		{
			EXError.Error(this, "::RPC_RequestListItem - Could not read price.");
			return;
		}

		if (!CanListItem(identity, target, price))
		{
			EXError.Error(this, "::RPC_RequestListItem - Listing conditions failed!");
			return;
		}
		
		ListItem(identity, target, traderID, price);
	}

	protected bool CanListItem(PlayerIdentity identity, Object target, int price)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		int playerListingsCount = GetPlayerListingsCount(identity.GetId());
		if (m_P2PMarketSettings.MaxListings != -1 && playerListingsCount >= m_P2PMarketSettings.MaxListings)
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_MAX_LISTING_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_MAX_LISTING_DESC", playerListingsCount.ToString(), m_P2PMarketSettings.MaxListings.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
			CallbackError(identity);
			return false;
		}
		
		EntityAI objEntity;
		if (!Class.CastTo(objEntity, target))
		{
			EXError.Error(this, "::RPC_RequestListItem - Could not get target object.");
			CallbackError(identity);
			return false;
		}

		if (objEntity.IsSetForDeletion())
		{
			EXError.Error(this, "::RPC_RequestListItem - target is about to be deleted.");
			CallbackError(identity);
			return false;
		}

		int doorsCount;
		int doorsRequiredAmount;
		int wheelsCount;
		int wheelsRequiredAmount;
		array<EntityAI> slotItems;
		string type = objEntity.GetType();
		
		//! If the object is a vehicle we check for paired keys and unpair/delete them all and remove the pairing from the vehicle.
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, objEntity))
		{
			#ifdef EXPANSIONMODVEHICLE
			if (vehicle.GetLockState() == ExpansionVehicleLockState.FORCEDUNLOCKED)
			{
				CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorVehicleLockpicked, type);
				return false;
			}
			#endif

			slotItems = GetSlotItems(vehicle.GetEntity(), doorsRequiredAmount, wheelsRequiredAmount);
			foreach (EntityAI slotItemCar: slotItems)
			{
				if (slotItemCar.IsInherited(CarDoor))
				{
					doorsCount++;
				}
				else if (slotItemCar.IsInherited(CarWheel))
				{
					wheelsCount++;
				}
			}

			if (doorsCount < doorsRequiredAmount || wheelsCount < wheelsRequiredAmount || !vehicle.IsFunctional(true))
			{
				CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorVehicleMissingAttachment, type);
				return false;
			}

			if (!CheckItemsCondition(slotItems))
			{
				CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorVehicleRuinedAttachment, type);
				return false;
			}

			#ifdef EXPANSIONMODVEHICLE
			if (vehicle.HasKey())
			{
				array<ExpansionCarKey> carKeys = new array<ExpansionCarKey>;				
				bool hasCarKey = false;
				ExpansionCarKey carKey;
				int i;
				
				ExpansionCarKey.GetKeysForVehicle(vehicle, carKeys);
				
				for (i = 0; i < carKeys.Count(); ++i)
				{
					carKey = carKeys[i];
					if (!carKey)
						continue;

					if (carKey.GetHierarchyRootPlayer() && carKey.GetHierarchyRootPlayer() == identity.GetPlayer())
					{
						hasCarKey = true;
					}
				}
				
				if (!hasCarKey)
				{
					CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorVehicleKeysMissing, type);
					return false;
				}
				
				for (i = 0; i < carKeys.Count(); ++i)
				{
					carKey = carKeys[i];
					if (!carKey)
						continue;

					carKey.Unpair(true);
					g_Game.ObjectDelete(carKey);
				}

				vehicle.ResetKeyPairing();
			}
			#endif
		}

		#ifdef SERVER
		if (m_P2PMarketSettings.DisallowUnpersisted && !ExpansionWorld.IsStoreLoaded(objEntity) && !ExpansionWorld.IsStoreSaved(objEntity))
		{
			CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorPersistency, type);
			return false;
		}
		#endif
		
		return true;
	}
	
	protected void ListItem(PlayerIdentity identity, Object target, int traderID, int price, bool ignoreListingPrice = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		EntityAI objEntity;
		if (!Class.CastTo(objEntity, target))
		{
			EXError.Error(this, "::ListItem - Could not get target object.");
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
		if (!player)
		{
			EXError.Error(this, "::ListItem - Could not get player entity.");
			return;
		}

		ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(traderID);
		if (!traderConfig)
		{
			EXError.Error(this, "::ListItem - Could not get P2P trader data for ID " + traderID);
			return;
		}
		
		ExpansionP2PMarketListing newListing = new ExpansionP2PMarketListing();
		newListing.SetFromItem(objEntity, player);
		if (!newListing.IsGlobalIDValid())
		{
			EXError.Error(this, "::ListItem - Global ID for new listing is invalid! Global ID: " + newListing.GetGlobalID());
			return;
		}

		newListing.SetPrice(price);
		newListing.SetListingTime();
		newListing.SetTraderID(traderID);
		newListing.SetListingState(ExpansionP2PMarketListingState.LISTED);
		
		if (!ignoreListingPrice)
		{
			string type = objEntity.GetType();
			int listingPrice = Math.Ceil(price * GetListingPricePercent(traderID, player) / 100);
			string listingPriceString = GetDisplayPrice(traderConfig, listingPrice, false, true, true);
			if (listingPrice <= 0)
			{
				CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorListingPriceTooLow, type, price, listingPrice);
				return;
			}
	
			array<int> monies = new array<int>;
			TStringArray currencies = traderConfig.GetCurrencies();
			int playerWorth = m_MarketModule.GetPlayerWorth(player, monies, currencies);
			if (playerWorth <= 0 || playerWorth < listingPrice)
			{
				CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorNotEnoughMoneyToList, type, price, listingPrice);
				return;
			}
			
			if (!m_MarketModule.RemoveMoney(listingPrice, player, currencies))
			{
				EXError.Error(this, "::ListItem - Could not remove money from player!");
				return;
			}
		}
		
		if (!StoreItem(newListing, objEntity))
		{
			EXError.Error(this, "::ListItem - Could not store listing item!");
			return;
		}

		AddListing(traderID, newListing);
		newListing.Save();
		
		UpdateListingCategoryData(newListing, false); //! Update global/trader p2p market category data

		ExpansionP2PMarketRequestData data = new ExpansionP2PMarketRequestData();
		data.m_TraderID = traderID;
		data.m_CallbackType = ExpansionP2PMarketModuleCallback.ItemListed;
		data.m_MessageTypeName = type;
		data.m_MessagePrice = price;

		SendUpdatedTraderData(data, identity.GetId()); //! Update listing data for all trading players at the same trader.

		if (GetExpansionSettings().GetLog().Market)
		{
			string globalIDText = ExpansionStatic.IntToHex(newListing.GetGlobalID());
			string priceStringLog = string.Format("%1 (%2)", price, GetDisplayPrice(traderConfig, price, false, false, true));
			GetExpansionSettings().GetLog().PrintLog("[P2P Market] Player \"" + identity.GetName() + "\" (id=" + identity.GetId() + ")" + " has listed \"" + newListing.GetClassName() + "\" for a price of " + priceStringLog + " (globalID=" + globalIDText + ")");
		}
	}

	protected void AddListing(int traderID, ExpansionP2PMarketListing listing)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		ExpansionP2PMarketCounters counters = GetPlayerDataCounters(listing.GetOwnerUID());
		
		array<ref ExpansionP2PMarketListing> listings;
		if (m_ListingsData.Find(traderID, listings))
		{
			if (listings.Find(listing) == -1)
			{
				listings.Insert(listing);
				m_ListingsCount++;
				counters.m_OwnedListingsCount++;
			}
		}
		else
		{
			listings = new array<ref ExpansionP2PMarketListing>;
			listings.Insert(listing);
			m_ListingsData.Insert(traderID, listings);
			m_ListingsCount++;
			counters.m_OwnedListingsCount++;
		}
	}
	
	protected void AddSoldListing(int traderID, ExpansionP2PMarketListing listing)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		array<ref ExpansionP2PMarketListing> listings;
		if (m_SoldListingsData.Find(traderID, listings))
		{
			if (listings.Find(listing) == -1)
			{
				listings.Insert(listing);
			}
		}
		else
		{
			listings = new array<ref ExpansionP2PMarketListing>;
			listings.Insert(listing);
			m_SoldListingsData.Insert(traderID, listings);
		}
	}
	
	//! Server
	void CallbackError(PlayerIdentity identity, ExpansionP2PMarketModuleCallback error = ExpansionP2PMarketModuleCallback.Error, string type = string.Empty, int price = 0, int listingPriceString = 0, Object blockingObject = null)
	{
		Callback(identity, error, type, price, listingPriceString, blockingObject);
	}

	void Callback(PlayerIdentity identity, ExpansionP2PMarketModuleCallback callback, string type = string.Empty, int price = 0, int listingPriceString = 0, Object blockingObject = null)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		auto rpc = Expansion_CreateRPC("RPC_Callback");
		rpc.Write(callback);
		rpc.Write(type);
		rpc.Write(price);
		rpc.Write(listingPriceString);
		rpc.Write(blockingObject);
		rpc.Expansion_Send(true, identity);
	}

	//!Server
	protected void ListingDataChanged(PlayerIdentity identity, string globalIDText = "")
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		auto rpc = Expansion_CreateRPC("RPC_UpdateClientRequest");
		rpc.Write(globalIDText);
		rpc.Expansion_Send(true, identity);
	}
	
	//!Client
	void RPC_UpdateClientRequest(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		if (!GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			return;
		
		string globalIDText;
		if (!ctx.Read(globalIDText))
		{
			EXError.Error(this, "::RPC_RequestListingDetails - Could not read globalIDText string.");
			return;
		}
		
		m_UpdateInvoker.Invoke(globalIDText);
	}

	//!Client
	void RequestListingDetails(int traderID, TIntArray globalID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		auto rpc = Expansion_CreateRPC("RPC_RequestListingDetails");
		rpc.Write(traderID);
		rpc.Write(globalID);
		rpc.Expansion_Send(true);
	}
	
	//! Server
	protected void RPC_RequestListingDetails(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		int traderID;
		if (!ctx.Read(traderID))
		{
			EXError.Error(this, "::RPC_RequestListingDetails - Could not read trader ID.");
			CallbackError(identity);
			return;
		}
		
		TIntArray globalID;
		if (!ctx.Read(globalID))
		{
			EXError.Error(this, "::RPC_RequestListingDetails - Could not read global ID.");
			CallbackError(identity);
			return;
		}
		
		ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(traderID);
		if (!traderConfig)
		{
			EXError.Error(this, "::RPC_RequestListingDetails - Could not get P2P trader data for ID " + traderID);
			return;
		}
		
		ExpansionP2PMarketListing listingToSend = GetListingByGlobalID(traderID, globalID, traderConfig.IsGlobalTrader());
		if (!listingToSend)
		{
			string globalIDText = ExpansionStatic.IntToHex(globalID);
			EXError.Error(this, "::RPC_RequestListingDetails - Could not get listing to send! Trader ID: " + traderID + " | Global ID: " + globalIDText + " | Is global: " + traderConfig.IsGlobalTrader());
			return;
		}
		
		auto rpc = Expansion_CreateRPC("RPC_SendListingDetails");
		listingToSend.OnSendDetails(rpc);
		rpc.Expansion_Send(true, identity);
	}
	
	//! Client
	protected void RPC_SendListingDetails(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		ExpansionP2PMarketListing listing = new ExpansionP2PMarketListing();
		if (!listing.OnRecieveDetails(ctx))
		{
			EXError.Error(this, "::RPC_SendListingDetails - Could not read detailded listing data!");
			return;
		}
		
		m_ListingDetailsInvoker.Invoke(listing);
	}
	
	//! Client
	void RequestPurchaseItem(int traderID, ExpansionP2PMarketListing listing, int pageIndex, int categoryIndex, int subCategoryIndex, bool ownedListings)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		if (!listing.IsGlobalIDValid())
		{
			EXError.Error(this, "::RequestPurchaseItem - Listing global ID is invalid!");
			return;
		}

		int listingTraderID = listing.GetTraderID();
		TIntArray globalID = listing.GetGlobalID();
		string globalIDText = listing.GetEntityStorageBaseName();	//! @note: For logging purposes only
		P2PDebugPrint("Trader ID: " + traderID + " | Listing trader ID: " + listingTraderID + " | Global ID: " + globalIDText);
		
		auto rpc = Expansion_CreateRPC("RPC_RequestPurchaseItem");
		rpc.Write(listingTraderID);
		rpc.Write(traderID);
		rpc.Write(globalID);
		rpc.Write(pageIndex);
		rpc.Write(categoryIndex);
		rpc.Write(subCategoryIndex);
		rpc.Write(ownedListings);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestPurchaseItem(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		int listingTraderID;
		if (!ctx.Read(listingTraderID))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read listing trader ID.");
			CallbackError(identity);
			return;
		}
		
		int traderID;
		if (!ctx.Read(traderID))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read trader ID.");
			CallbackError(identity);
			return;
		}

		TIntArray globalID = new TIntArray;
		if (!ctx.Read(globalID))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read global ID.");
			CallbackError(identity);
			return;
		}
		
		int pageIndex;
		if (!ctx.Read(pageIndex))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read page Index.");
			CallbackError(identity);
			return;
		}
		
		int categoryIndex;
		if (!ctx.Read(categoryIndex))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read category Index.");
			CallbackError(identity);
			return;
		}
		
		int subCategoryIndex;
		if (!ctx.Read(subCategoryIndex))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read sub-category Index.");
			CallbackError(identity);
			return;
		}
		
		bool ownedListings;
		if (!ctx.Read(ownedListings))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read owned listings boolean.");
			CallbackError(identity);
			return;
		}

		ExpansionP2PMarketTraderConfig traderConfig = GetP2PTraderConfigByID(listingTraderID);
		if (!traderConfig)
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not read traderConfig for ID: " + listingTraderID);
			CallbackError(identity);
			return;
		}

		ExpansionP2PMarketListing listing = GetListingByGlobalID(listingTraderID, globalID, traderConfig.IsGlobalTrader());
		if (!listing)
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not get listing for ID: " + globalID + " | Trader ID: " + listingTraderID);
			CallbackError(identity);
			return;
		}

		string listingOwnerUID = listing.GetOwnerUID();
		string playerUID = identity.GetId();
		if (!identity || playerUID == "")
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not get player UID.");
			CallbackError(identity);
			return;
		}

		PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
		if (!player)
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not get player.");
			CallbackError(identity);
			return;
		}
		
		ExpansionP2PMarketCounters counters = GetPlayerDataCounters(listing.GetOwnerUID());

		bool isOwner;
		if (listingOwnerUID == playerUID)
			isOwner = true;

		int price = listing.GetPrice();
		string typeName = listing.GetClassName();
		string priceString = GetDisplayPrice(traderConfig, price, false, true, true);
		int ownerDiscount = ((float) price / 100) * m_P2PMarketSettings.ListingOwnerDiscountPercent;
		int ownerPrice = price - ownerDiscount;
		string ownerPriceString = GetDisplayPrice(traderConfig, ownerPrice, false, true, true);
		array<int> monies = new array<int>;
		TStringArray currencies = traderConfig.GetCurrencies();
		int playerWorth = m_MarketModule.GetPlayerWorth(player, monies, currencies);
		if (!isOwner && playerWorth < price)
		{
			CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorNotEnoughMoney, typeName, price);
			return;
		}
		else if (isOwner && playerWorth < ownerPrice)
		{
			CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorNotEnoughMoney, typeName, ownerPrice);
			return;
		}

		vector spawnPositionVehicle;
		bool isVehicle = false;

		ExpansionVehicleType vehicleType = ExpansionVehicle.GetVehicleType(typeName);
		switch (vehicleType)
		{
			case ExpansionVehicleType.WATER:
				spawnPositionVehicle = traderConfig.GetWatercraftSpawnPosition();
				break;
			case ExpansionVehicleType.AIR:
				spawnPositionVehicle = traderConfig.GetAircraftSpawnPosition();
				break;
			case ExpansionVehicleType.LAND:
			case ExpansionVehicleType.RAILTRACK:
				spawnPositionVehicle = traderConfig.GetVehicleSpawnPosition();
				break;
		}

		if (vehicleType != ExpansionVehicleType.NONE)
		{
			isVehicle = true;
		}

		if (isVehicle && spawnPositionVehicle == vector.Zero)
		{
			CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorNoVehicleSpawnPosition, typeName);
			return;
		}

		Object blockingObject;
		if (isVehicle && !ExpansionItemSpawnHelper.IsSpawnPositionFree(spawnPositionVehicle, Vector(0, 0, 0), typeName, blockingObject))
		{
			CallbackError(identity, ExpansionP2PMarketModuleCallback.ErrorVehicleSpawnPositionBlocked, typeName, 0, 0, blockingObject);
			return;
		}
		
		string globalIDText = listing.GetEntityStorageBaseName();	//! @note: For logging purposes only
		EntityAI loadedEntity;
		if (!LoadItem(listing, player, loadedEntity))
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PURCHASE_FAILED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_CANT_RESTORE_ITEM_DESC", globalIDText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
			CallbackError(identity);
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not restore stored item for listing: " + globalIDText);
			return;
		}
		
		ItemBase item = ItemBase.Cast(loadedEntity);
		if (item)
		{
			item.Expansion_GetGlobalID().Acquire();
		}

		#ifdef EXPANSIONMODVEHICLE
		//! If the object is a vehicle we spawn a car key on the player and pair it to the vehicle
		ExpansionCarKey key;
		EntityAI playerEntity;
		Class.CastTo(playerEntity, player);
		int amount = 1;
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, loadedEntity))
		{
			EntityAI vehicleEntity = vehicle.GetEntity();
			vehicleEntity.SetPosition(spawnPositionVehicle);
			vehicleEntity.PlaceOnSurface();
			dBodyActive(vehicleEntity, ActiveState.ACTIVE);
			vehicle.Synchronize();

			key = ExpansionCarKey.Cast(ExpansionItemSpawnHelper.SpawnOnParent("ExpansionCarKey", player, playerEntity, amount));
			vehicle.PairKey(key);
			if (!vehicle.IsLocked())
				vehicle.Lock(key); //! Lock vehicle if not locked already
		}
		#endif

		if (!isOwner && !m_MarketModule.RemoveMoney(price, player, currencies))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not remove money from player!");
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PURCHASE_FAILED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_CANT_REMOVE_MONEY_DESC"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
			CallbackError(identity);
			return;
		}
		else if (isOwner && !m_MarketModule.RemoveMoney(ownerPrice, player, currencies))
		{
			EXError.Error(this, "::RPC_RequestPurchaseItem - Could not remove money from player!");
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PURCHASE_FAILED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_CANT_REMOVE_MONEY_DESC"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
			CallbackError(identity);
			return;
		}

		UpdateListingCategoryData(listing, true); //! Update global/trader p2p market category data
		
		if (!isOwner)
		{
			ExpansionP2PMarketListing soldListing = new ExpansionP2PMarketListing();
			soldListing.Copy(listing);

			if (!RemoveListingByGlobalID(listingTraderID, globalID, traderConfig.IsGlobalTrader(), false, true))
			{
				soldListing = null;
				EXError.Error(this, "::RPC_RequestPurchaseItem - Could not remove listing " + globalIDText);
				ExpansionNotification("RPC_RequestPurchaseItem", "Could not remove listing " + globalIDText).Error(identity);
				return;
			}

			soldListing.SetListingState(ExpansionP2PMarketListingState.SOLD);
			soldListing.SetListingTime();
			soldListing.Save();
			
			AddSoldListing(listingTraderID, soldListing);
			
			counters.m_SoldListingsCount++;
			counters.m_SoldTotalIncome += listing.GetPrice();
			counters.m_OwnedListingsCount--;
		}
		else
		{
			//! We remove the listing directly from the trader data as there is no need to
			//! change the listing to the sold state when it is purchased by the listing owner.
			if (!RemoveListingByGlobalID(listingTraderID, globalID, traderConfig.IsGlobalTrader(), false, true))
			{
				EXError.Error(this, "::RPC_RequestPurchaseItem - Could not remove listing " + globalIDText);
				ExpansionNotification("RPC_RequestPurchaseItem", "Could not remove listing " + globalIDText).Error(identity);
			}

			counters.m_OwnedListingsCount--;
		}

		int messagePrice;
		if (!isOwner)
		{
			messagePrice = price;
		}
		else
		{
			messagePrice = ownerPrice;
		}

		ExpansionP2PMarketRequestData data = new ExpansionP2PMarketRequestData();
		data.m_TraderID = traderID;
		data.m_ListingTraderID = listingTraderID;
		data.m_PageIndex = pageIndex;
		data.m_CategoryIndex = categoryIndex;
		data.m_SubCategoryIndex = subCategoryIndex;
		data.m_OwnedListings = ownedListings;
		data.m_CallbackType = ExpansionP2PMarketModuleCallback.ItemPurchased;
		data.m_MessageTypeName = typeName;
		data.m_MessagePrice = messagePrice;
		data.m_GlobalIDText = globalIDText;
		
		SendUpdatedTraderData(data, identity.GetId());
		
		if (!isOwner)
		{
			PlayerBase ownerPlayer = PlayerBase.GetPlayerByUID(listingOwnerUID);
			if (ownerPlayer)
			{
				Callback(ownerPlayer.GetIdentity(), ExpansionP2PMarketModuleCallback.MsgItemGotSold, typeName, price);
			}
		}
		
		if (GetExpansionSettings().GetLog().Market)
		{
			string messagePriceStringLog = string.Format("%1 (%2)", messagePrice, GetDisplayPrice(traderConfig, messagePrice, false, false, true));
			GetExpansionSettings().GetLog().PrintLog("[P2P Market] Player \"" + identity.GetName() + "\" (id=" + identity.GetId() + ")" + " has purchased \"" + typeName + "\" for a price of " + messagePriceStringLog + " (globalID=" + globalIDText + ")");
		}
		
		MissionBaseWorld missionWorld = MissionBaseWorld.Cast(g_Game.GetMission());
		if (missionWorld)
		{
			missionWorld.Expansion_OnP2PMarketPurchase(playerUID, messagePrice, loadedEntity);
		}
	}

	//! Client
	protected void RPC_Callback(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		int callback;
		if (!ctx.Read(callback))
		{
			EXError.Error(this, "::RPC_Callback - couldn't read callback");
			return;
		}

		string type;
		if (!ctx.Read(type))
		{
			EXError.Error(this, "::RPC_Callback - couldn't read type");
			return;
		}

		int price;
		if (!ctx.Read(price))
		{
			EXError.Error(this, "::RPC_Callback - couldn't read price");
			return;
		}

		int option;
		if (!ctx.Read(option))
		{
			EXError.Error(this, "::RPC_Callback - couldn't read option");
			return;
		}

		Object blockingObject;
		if (!ctx.Read(blockingObject))
		{
			EXError.Error(this, "::RPC_Callback - couldn't read price");
			return;
		}

		string priceString;
		CF_Localiser localiser;

		switch (callback)
		{
			case ExpansionP2PMarketModuleCallback.MsgTotalSold:
				priceString = ExpansionMarketModule.GetDisplayPriceEx(price);
				int salesCount = option;
				localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_MSG_TOTAL_SOLD_NOTIFIER", priceString, salesCount.ToString());
				g_Game.Chat(localiser.Format(), "colorAction");
				break;

			case ExpansionP2PMarketModuleCallback.MsgItemGotSold:
				string displayName = ExpansionStatic.GetItemDisplayNameWithType(type);
				priceString = ExpansionMarketModule.GetDisplayPriceEx(price);
				localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_MSG_ITEM_GOT_SOLD_NOTIFIER", displayName, priceString);
				g_Game.Chat(localiser.Format(), "colorAction");
				m_CallbackInvoker.Invoke(callback);
				break;

			default:
				m_CallbackInvoker.Invoke(callback, type, price, option, blockingObject);
				break;
		}
	}

	//! For 3rd-party modding override support
	float GetListingPricePercent(int traderID, PlayerBase player = null)
	{
		return m_P2PMarketSettings.ListingPricePercent;
	}

	static int GetMarketPrice(string typeName)
	{
		typeName.ToLower();
		ExpansionMarketItem marketItem = ExpansionMarketCategory.GetGlobalItem(typeName);
		if (marketItem)
			return marketItem.CalculatePrice(marketItem.MaxStockThreshold);

		return 0;
	}
	
	static string GetDisplayPrice(ExpansionP2PMarketTraderConfig trader, int price, bool shorten = false, bool format = true, bool includeDisplayCurrencyName = false)
	{
		return ExpansionMarketModule.GetDisplayPriceEx(price, shorten, format, includeDisplayCurrencyName, trader.m_DisplayCurrencyValue, trader.m_DisplayCurrencyPrecision, trader.m_DisplayCurrencyName);
	}
	
	bool CheckCanUseTrader(PlayerBase player, ExpansionP2PMarketTraderConfig trader)
	{
		#ifdef ENFUSION_AI_PROJECT
		if (g_Game.IsServer() && trader.m_RequiredFaction != "")
		{
			if (!player.GetGroup() || player.GetGroup().GetFaction().GetName() != trader.m_RequiredFaction)
			{
				string factionDisplayname;
				#ifdef EXPANSIONMODAI
				eAIFaction faction = eAIFaction.Create(trader.m_RequiredFaction);
				factionDisplayname = faction.GetDisplayName();
				#else
				factionDisplayname = trader.m_RequiredFaction;
				#endif
				ExpansionNotification("STR_EXPANSION_AI_FACTION", new StringLocaliser("STR_EXPANSION_AI_REQUIRED_FACTION_TRADER", factionDisplayname), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(player.GetIdentity());

				return false;
			}
		}
		#endif

		return true;
	}

	protected bool StoreItem(ExpansionP2PMarketListing listing, EntityAI itemEntity)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		if (!itemEntity)
		{
			EXError.Error(this, "::StoreItem - Could not get item entity!");
			return false;
		}
		
		string subDir = GetP2PMarketDataDirectory() + "traderID_" + listing.GetTraderID() + "\\entitystorage\\";
		if (!FileExist(subDir))
		{
			ExpansionStatic.MakeDirectoryRecursive(subDir);
		}
		
		bool success = ExpansionEntityStorageModule.SaveToFile(itemEntity, listing.GetEntityStorageFileName());
		if (!success)
		{
			EXError.Error(this, "::StoreVehicle - Could not store item entity!");
			return false;
		}

		ExpansionEntityStorageModule.DeleteSavedEntity(itemEntity);

		return true;
	}

	protected bool LoadItem(ExpansionP2PMarketListing listing, PlayerBase player, out EntityAI loadedEntity = null)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		if (!ExpansionEntityStorageModule.RestoreFromFile(listing.GetEntityStorageFileName(), loadedEntity, null, player))
		{
			EXError.Error(this, "::LoadItem - Could not restore item " + listing.GetClassName() + " from file " + listing.GetEntityStorageFileName());
			return false;
		}

		return true;
	}

	array<EntityAI> GetSlotItems(EntityAI entity, out int doorsRequiredAmount, out int wheelsRequiredAmount)
	{
		array<EntityAI> slotItems = new array<EntityAI>;
		for (int i = 0; i < entity.GetInventory().GetAttachmentSlotsCount(); ++i)
		{
			int slotID = entity.GetInventory().GetAttachmentSlotId(i);
			string slotName = InventorySlots.GetSlotName(slotID);

			slotName.ToLower();

			foreach (string doorSlotKeyword: s_DoorSlotKeywords)
			{
				if (slotName.IndexOf(doorSlotKeyword) > -1)
				{
					doorsRequiredAmount++;
					break;
				}
			}

			foreach (string wheelSlotKeyword: s_WheelSlotKeywords)
			{
				if (slotName.IndexOf(wheelSlotKeyword) > -1 && slotName.IndexOf("spare") < 0)
				{
					wheelsRequiredAmount++;
					break;
				}
			}

			EntityAI slotItem = entity.GetInventory().FindAttachment(slotID);
			if (slotItem)
				slotItems.Insert(slotItem);
		}

		return slotItems;
	}

	protected bool CheckItemsCondition(array<EntityAI> items)
	{
		foreach (EntityAI item: items)
		{
			if (item.IsRuined())
				return false;
		}

		return true;
	}

	ExpansionP2PMarketTraderConfig GetP2PTraderConfigByID(int id)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		ExpansionP2PMarketTraderConfig config;
		if (m_P2PTraderConfig.Find(id, config))
			return config;

		return NULL;
	}

	array<EntityAI> LocalGetEntityInventory()
	{
		return m_LocalEntityInventory.m_Inventory;
	}

	void EnumeratePlayerInventory(PlayerBase player)
	{
		m_LocalEntityInventory = new ExpansionP2PMarketPlayerInventory(player);
	}

	ScriptInvoker GetListingsSI()
	{
		return m_ListingsInvoker;
	}
	
	ScriptInvoker GetListingDetailsSI()
	{
		return m_ListingDetailsInvoker;
	}

	ScriptInvoker GetCallbackSI()
	{
		return m_CallbackInvoker;
	}
	
	ScriptInvoker GetUpdateSI()
	{
		return m_UpdateInvoker;
	}

	static int GetDiscountPrice(int price)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, ExpansionP2PMarketModule);
		#endif 
		
		int discountPrice = Math.Round((price / 100) * GetExpansionSettings().GetP2PMarket().ListingOwnerDiscountPercent);
		return (price - discountPrice);
	}

	//! Check if one of the listed items is listed longer then MaxListingTime from the settings or sold listings are saved longer then SalesDepositTime.
	//! If so we clean the listed item up form the market and entity storage.
	protected void CheckListingsTimes()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		int timedif;
		int maxListingTime = m_P2PMarketSettings.MaxListingTime;
		int salesDepositTime = m_P2PMarketSettings.SalesDepositTime;
		bool save;
		int i;

		if (!m_ListingsData || m_ListingsData.Count() == 0)
			return;

		int currentTime = CF_Date.Now(true).GetTimestamp();

		foreach (int traderID, array<ref ExpansionP2PMarketListing> listings: m_ListingsData)
		{
			if (listings && listings.Count() > 0)
			{
				for (i = listings.Count() - 1; i >= 0; --i)
				{
					ExpansionP2PMarketListing listing = listings[i];	
					if (listing && !listing.HasCooldown(maxListingTime, timedif, currentTime))
					{
						RemoveListing(listing, listings, i, true, true, false);
					}
				}
			}
		}
		
		if (!m_SoldListingsData || m_SoldListingsData.Count() == 0)
			return;
		
		foreach (int soldTraderID, array<ref ExpansionP2PMarketListing> soldListings: m_SoldListingsData)
		{
			if (soldListings && soldListings.Count() > 0)
			{
				for (i = soldListings.Count() - 1; i >= 0; --i)
				{
					ExpansionP2PMarketListing soldListing = soldListings[i];	
					if (soldListing && !soldListing.HasCooldown(salesDepositTime, timedif, currentTime))
					{
						RemoveListing(soldListing, soldListings, i, false, true, true);
					}
				}
			}
		}
	}

	protected int GetPlayerListingsCount(string playerUID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		int count;
		foreach (int traderID, array<ref ExpansionP2PMarketListing> listings: m_ListingsData)
		{
			foreach (ExpansionP2PMarketListing listing: listings)
			{
				if (listing.GetOwnerUID() == playerUID)
					count++;
			}
		}

		return count;
	}

	ExpansionP2PMarketCategoryListings GetCategoryListingsData(int categoryIndex, int subCategoryIndex = -1, int traderID = -1, bool isGlobal = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		ExpansionP2PMarketCategoryListings categoryData;
		array<ref ExpansionP2PMarketCategoryListings> dataArray;
		if (isGlobal)
		{
			dataArray = m_CategoryListings[categoryIndex];
		}
		else
		{
			map<int, ref array<ref ExpansionP2PMarketCategoryListings>> categoriesMap = m_TraderCategoryListings[traderID];
			dataArray = categoriesMap[categoryIndex];
		}
		
		foreach (ExpansionP2PMarketCategoryListings data: dataArray)
		{
			if (subCategoryIndex == -1 && data.m_SubCategoryIndex == -1 || subCategoryIndex > -1 && data.m_SubCategoryIndex == subCategoryIndex)
			{
				categoryData = data;
				break;
			}
		}
	
		return categoryData;
	}
		
	protected array<ref ExpansionP2PMarketListing> GetTraderListings(int traderID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		return m_ListingsData[traderID];
	}

	protected ExpansionP2PMarketListing GetListingByGlobalID(int traderID, TIntArray globalID, bool globalTrader = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		string globalIDText = ExpansionStatic.IntToHex(globalID);
		ExpansionP2PMarketListing listing;
		if (traderID > -1 && !globalTrader)
		{
			array<ref ExpansionP2PMarketListing> listings = m_ListingsData[traderID];
			if (listings)
				listing = ExGetListingByGlobalID(listings, globalID);
		}
		else if (globalTrader)
		{
			foreach (int listingsTraderID, array<ref ExpansionP2PMarketListing> dataListing: m_ListingsData)
			{
				listing = ExGetListingByGlobalID(dataListing, globalID);
				if (listing)
					break;
			}
		}

		return listing;
	}
	
	protected ExpansionP2PMarketListing GetSoldListingByGlobalID(int traderID, TIntArray globalID, bool globalTrader = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		string globalIDText = ExpansionStatic.IntToHex(globalID);
		ExpansionP2PMarketListing listing;
		if (traderID > -1 && !globalTrader)
		{
			array<ref ExpansionP2PMarketListing> listings = m_SoldListingsData[traderID];
			if (listings)
				listing = ExGetListingByGlobalID(listings, globalID);
		}
		else if (globalTrader)
		{
			foreach (int listingsTraderID, array<ref ExpansionP2PMarketListing> dataListing: m_SoldListingsData)
			{
				listing = ExGetListingByGlobalID(dataListing, globalID);
				if (listing)
					break;
			}
		}

		return listing;
	}

	protected ExpansionP2PMarketListing ExGetListingByGlobalID(array<ref ExpansionP2PMarketListing> listings, TIntArray globalID)
	{
		string globalIDText = ExpansionStatic.IntToHex(globalID);
		foreach (ExpansionP2PMarketListing listing: listings)
		{
			bool isValid = listing.IsGlobalIDValid();
			bool isEqual = listing.IsGlobalIDEqual(globalID);
			string listingIDText = ExpansionStatic.IntToHex(listing.GetGlobalID());

			if (isValid && isEqual)
				return listing;
		}

		return null;
	}

	protected bool RemoveListingByGlobalID(int traderID, TIntArray globalID, bool globalTrader = false, bool deleteEntityStorageFile = false, bool deleteJSONFile = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 

		if (traderID > -1 && !globalTrader)
		{
			array<ref ExpansionP2PMarketListing> listings = m_ListingsData[traderID];
			if (listings && ExRemoveListingByGlobalID(listings, globalID, deleteEntityStorageFile, deleteJSONFile))
			{
				return true;
			}
		}
		else
		{
			foreach (int listingsTraderID, array<ref ExpansionP2PMarketListing> dataListings: m_ListingsData)
			{
				if (ExRemoveListingByGlobalID(dataListings, globalID, deleteEntityStorageFile, deleteJSONFile))
				{
					return true;
				}
			}
		}

		return false;
	}

	protected bool RemoveSoldListingByGlobalID(int traderID, TIntArray globalID, bool globalTrader = false, bool deleteEntityStorageFile = false, bool deleteJSONFile = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		if (traderID > -1 && !globalTrader)
		{
			array<ref ExpansionP2PMarketListing> listings = m_SoldListingsData[traderID];
			if (listings && ExRemoveListingByGlobalID(listings, globalID, deleteEntityStorageFile, deleteJSONFile, true))
			{
				return true;
			}
		}
		else
		{
			foreach (int listingsTraderID, array<ref ExpansionP2PMarketListing> dataListings: m_SoldListingsData)
			{
				if (ExRemoveListingByGlobalID(dataListings, globalID, deleteEntityStorageFile, deleteJSONFile, true))
				{
					return true;
				}
			}
		}

		return false;
	}
	
	protected bool ExRemoveListingByGlobalID(array<ref ExpansionP2PMarketListing> listings, TIntArray globalID, bool deleteEntityStorageFile = false, bool deleteJSONFile = false, bool soldListing = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		string globalIDText = ExpansionStatic.IntToHex(globalID);
		for (int i = listings.Count() - 1; i >= 0; --i)
		{
			ExpansionP2PMarketListing listing = listings[i];
			string listingIDText = ExpansionStatic.IntToHex(listing.GetGlobalID());
			bool isValid = listing.IsGlobalIDValid();
			bool isEqual = listing.IsGlobalIDEqual(globalID);

			if (isValid && isEqual)
			{
				if (RemoveListing(listing, listings, i, deleteEntityStorageFile, deleteJSONFile, soldListing))
					return true;
			}
		}

		return false;
	}

	protected bool RemoveListing(notnull ExpansionP2PMarketListing listing, notnull array<ref ExpansionP2PMarketListing> listings, int index, bool deleteEntityStorageFile = false, bool deleteJSONFile = false, bool soldListing = false)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif 
		
		if (deleteEntityStorageFile)
		{
			string baseName = listing.GetEntityStorageBaseName();
			//! @note ExpansionEntityStorageModule::DeleteFiles does its own error logging, no need to repeat logging here
			ExpansionEntityStorageModule.DeleteFiles(baseName, listing.GetEntityStorageDirectory());
		}
		
		if (deleteJSONFile)
		{
			string filePath = listing.GetListingFileName();
			bool fileExists = FileExist(filePath);
			if (!fileExists)
			{
				EXError.Warn(this, "::RemoveListing - Failed to find listing file=" + filePath);
			}
			else
			{
				bool deletedFile = DeleteFile(filePath);
				if (!deletedFile)
				{
					EXError.Error(this, "::RemoveListing - Failed to delete listing file=" + filePath);
				}
			}
		}

		//! Sanity check that the passed in listing is indeed the listing at the passed in index in the array.
		//! This shouldn't be possible to fail unless there is an error in the calling code (wrong listing, listings array, or index passed in)
		if (listings[index] != listing)
		{
			EXError.Error(this, "::RemoveListing - Listing at index " + index + " of passed in listings array does not match passed in listing!");

			index = listings.Find(listing);

			if (index == -1)
			{
				EXError.Error(this, "::RemoveListing - Listing with ID=" + listing.GetEntityStorageBaseName() + " not found in passed in listings array!");
				//! This is the only failure state where we should return false since we have to abort processing
				return false;
			}
		}

		listings.RemoveOrdered(index);

		if (listings.Count() == 0)
		{
			int traderID = listing.GetTraderID();

			if (!soldListing)
			{
				m_ListingsData.Remove(traderID);
			}
			else
			{
				m_SoldListingsData.Remove(traderID);
			}
		}

		ExpansionP2PMarketCounters counters = GetPlayerDataCounters(listing.GetOwnerUID());

		if (soldListing)
		{
			counters.m_SoldListingsCount--;
			counters.m_SoldTotalIncome -= listing.GetPrice();
		}
		else
		{
			m_ListingsCount--;
			counters.m_OwnedListingsCount--;
		}
		
		return true;
	}

	static bool ItemCheckEx(EntityAI item)
	{
		return MiscGameplayFunctions.Expansion_ItemCheck(item, GetExpansionSettings().GetP2PMarket().ExcludedClassNames);
	}

	#ifdef SERVER
	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		super.OnUpdate(sender, args);

		if (!m_Initialized)
			return;

		auto update = CF_EventUpdateArgs.Cast(args);

		m_CheckListingsTime += update.DeltaTime;
		if (m_CheckListingsTime >= CHECK_TICK_TIME)
		{
			CheckListingsTimes();
			m_CheckListingsTime = 0.0;
		}
	}
	#endif

	static ExpansionP2PMarketModule GetModuleInstance()
	{
		return s_Instance;
	}

	static string GetP2PMarketDataDirectory()
	{
		int instanceID = g_Game.ServerConfigGetInt("instanceId");
		return "$mission:storage_" + instanceID + "\\expansion\\p2pmarket\\";
	}
	
	/*static string GetP2PMarketEntityStorageDirectory()
	{
		return GetP2PMarketDataDirectory() + "entitystorage\\";
	}*/

	void P2PDebugPrint(string text)
	{
		#ifdef EXPANSIONMODP2PMARKET_DEBUG
		ErrorEx(text, ErrorExSeverity.INFO);
		#endif
	}
};
