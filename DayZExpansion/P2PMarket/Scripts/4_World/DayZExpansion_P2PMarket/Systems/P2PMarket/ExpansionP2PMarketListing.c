/**
 * ExpansionP2PMarketListing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionP2PMarketListingState
{
	INVALID = 0,
	LISTED = 1,
	SOLD = 2
};

class ExpansionP2PMarketListing: ExpansionP2PMarketListingBase
{
	[NonSerialized()]
	static const int VERSION = 2;

	[NonSerialized()]
	protected int m_TraderID = -1;
	
	[NonSerialized()]
	protected int m_CategoryIndex = -1;
	[NonSerialized()]
	protected int m_SubCategoryIndex = -1;

	autoptr TIntArray m_GlobalID;
	string m_OwnerUID;
	int m_Price = -1;
	int m_ListingTime = -1;
	ExpansionP2PMarketListingState m_ListingState = ExpansionP2PMarketListingState.INVALID;

	string m_OwnerName;

	void ExpansionP2PMarketListing()
	{
		m_Version = VERSION;
		m_GlobalID = {0, 0, 0, 0};
	}

	override void SetFromItem(EntityAI object, PlayerBase owner = null)
	{
		super.SetFromItem(object, owner);

		if (!owner)
			owner = PlayerBase.Cast(object.GetHierarchyRootPlayer());

		if (owner && owner.GetIdentity())
		{
			m_OwnerUID = owner.GetIdentity().GetId();
			m_OwnerName = owner.GetIdentity().GetName();
		}

		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, object))
		{
			if (!vehicle.GetGlobalID().m_IsSet)
				vehicle.GetGlobalID().Acquire();

			for (int i = 0; i < 4; ++i)
				m_GlobalID[i] = vehicle.GetGlobalID().m_ID[i];

			#ifdef EXPANSIONMODVEHICLE
			if (vehicle.GetCurrentSkinName() != string.Empty)
			{
				m_SkinName = vehicle.GetCurrentSkinName();
				m_SkinIndex = vehicle.GetCurrentSkinIndex();
			}
			#endif
		}

		ItemBase itemIB;
		if (Class.CastTo(itemIB, object))
		{
			if (!itemIB.m_Expansion_GlobalID)
				itemIB.m_Expansion_GlobalID = new ExpansionGlobalID;

			if (!itemIB.m_Expansion_GlobalID.m_IsSet)
				itemIB.m_Expansion_GlobalID.Acquire();

			for (int j = 0; j < 4; j++)
			{
				m_GlobalID[j] = itemIB.m_Expansion_GlobalID.m_ID[j];
			}
		}
	}

	void ResetOwner()
	{
		m_OwnerUID = "";
	}

	TIntArray GetGlobalID()
	{
		return m_GlobalID;
	}

	string GetOwnerUID()
	{
		return m_OwnerUID;
	}

	string GetOwnerName()
	{
		return m_OwnerName;
	}

	void SetPrice(int price)
	{
		m_Price = price;
	}

	int GetPrice()
	{
		return m_Price;
	}

	bool IsGlobalIDValid()
	{
		if (m_GlobalID.Count() != 4)
			return false;

		foreach (int id: m_GlobalID)
		{
			if (id == 0)
				return false;
		}

		return true;
	}

	bool IsGlobalIDEqual(TIntArray id)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_GlobalID[i] != id[i])
				return false;
		}

		return true;
	}

	bool IsGlobalIDEqual(ExpansionP2PMarketListing listing)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_GlobalID[i] != listing.m_GlobalID[i])
				return false;
		}

		return true;
	}

	string GetEntityStorageBaseName()
	{
		return ExpansionStatic.IntToHex(m_GlobalID);
	}
	
	string GetListingDirectory()
	{
		return ExpansionP2PMarketModule.GetP2PMarketDataDirectory() + "traderID_" + m_TraderID + "\\listings\\";
	}

	string GetListingFileName()
	{
		return GetListingDirectory() + GetEntityStorageBaseName() + ".json";
	}

	string GetEntityStorageDirectory()
	{
		return ExpansionP2PMarketModule.GetP2PMarketDataDirectory() + "traderID_" + m_TraderID + "\\entitystorage\\";
	}

	string GetEntityStorageFileName()
	{
		return GetEntityStorageDirectory() + GetEntityStorageBaseName() + ExpansionEntityStorageModule.EXT;
	}

	void SetListingTime()
	{
		m_ListingTime = CF_Date.Now(true).GetTimestamp();
	}

	bool HasCooldown(int cooldown, out int timedif, int currentTime = 0)
	{
		if (m_ListingTime == -1)
			return false;

		if (currentTime == 0)
			currentTime = CF_Date.Now(true).GetTimestamp();
		timedif = (currentTime - m_ListingTime);
		if (timedif < cooldown)
			return true;

		return false;
	}

	int GetListingTime()
	{
		return m_ListingTime;
	}

	ExpansionP2PMarketListingState GetListingState()
	{
		return m_ListingState;
	}

	void SetListingState(ExpansionP2PMarketListingState state)
	{
		m_ListingState = state;
	}

	void SetTraderID(int traderID)
	{
		m_TraderID = traderID;
	}

	int GetTraderID()
	{
		return m_TraderID;
	}
	
	void SetCategoryIndex(int index)
	{
		m_CategoryIndex = index;
	}

	int GetCategoryIndex()
	{
		return m_CategoryIndex;
	}
	
	void SetSubCategoryIndex(int index)
	{
		m_SubCategoryIndex = index;
	}

	int GetSubCategoryIndex()
	{
		return m_SubCategoryIndex;
	}

	void CopyFromBaseClass(ExpansionP2PMarketListingBase base)
	{
		m_ClassName = base.m_ClassName;
		m_SkinName = base.m_SkinName;
	
		m_HealthLevel = base.m_HealthLevel;
		m_Quantity = base.m_Quantity;
		m_QuantityType = base.m_QuantityType;
		m_LiquidType = base.m_LiquidType;
		m_IsBloodContainer = base.m_IsBloodContainer;
		m_FoodStageType = base.m_FoodStageType;
	
		#ifdef EXPANSIONMODHARDLINE
		m_Rarity = base.m_Rarity;
		#endif			

		m_ContainerItems = base.m_ContainerItems;
	}
	
	void Copy(ExpansionP2PMarketListing listing)
	{
		CopyFromBaseClass(listing);
		
		m_TraderID = listing.m_TraderID;
		m_GlobalID.Copy(listing.m_GlobalID);
		m_OwnerUID = listing.m_OwnerUID;
		m_Price = listing.m_Price;
		m_ListingTime = listing.m_ListingTime;
		m_ListingState = listing.m_ListingState;
		m_OwnerName = listing.m_OwnerName;
	}

	static ExpansionP2PMarketListing Load(string fileName, int traderID)
	{
		ErrorEx("[P2P Market] Load existing P2P market listing file=" + fileName + " | Trader ID=" + traderID, ErrorExSeverity.INFO);
		
		ExpansionP2PMarketListing data;
		if (!ExpansionJsonFileParser<ExpansionP2PMarketListing>.Load(fileName, data))
			return NULL;
		
		data.SetTraderID(traderID);
		
		bool save;
		if (data.m_Version < VERSION)
		{
			ErrorEx("[P2P Market] Convert existing P2P market listing file=" + fileName + " | File version=" + data.m_Version + " | New version=" + VERSION, ErrorExSeverity.INFO);
			save = true;
			
			if (data.m_Version < 2)
			{
				//! Migrate older listing and entity storage files from old to new location
				string baseName = data.GetEntityStorageBaseName();
				string extES = ExpansionEntityStorageModule.EXT;
				int instanceID = g_Game.ServerConfigGetInt("instanceId");
				string srcPathES = ExpansionEntityStorageModule.GetStorageDirectory() + baseName;  //! old path
				string srcES = srcPathES + extES;
				
				string subDir = data.GetEntityStorageDirectory(); //! new sub dir
				string dstPathES = subDir + baseName;  //! new path
				string dstES = dstPathES + extES;

				bool foundESOldFile = FileExist(srcES);
				bool foundESNewFile = FileExist(dstES);

				ErrorEx("[P2P Market] Old file=" + srcES + " | Found=" + foundESOldFile, ErrorExSeverity.INFO);
				ErrorEx("[P2P Market] New file=" + dstES + " | Found=" + foundESNewFile, ErrorExSeverity.INFO);

				if (!foundESNewFile && foundESOldFile)
				{
					ErrorEx("[P2P Market] Found file=" + srcES, ErrorExSeverity.INFO);
					ErrorEx("[P2P Market] New destination file is missing at=" + dstES, ErrorExSeverity.INFO);
					
					if (!FileExist(subDir))
					{
						ErrorEx("[P2P Market] Create new entity storage sub dir=" + subDir, ErrorExSeverity.INFO);
						ExpansionStatic.MakeDirectoryRecursive(subDir);
					}
					
					if (CopyFile(srcES, dstES))
					{
						ErrorEx("[P2P Market] Copied file=" + srcES + " to destination=" + dstES, ErrorExSeverity.INFO);
						
						//! If the .bin file has an accompanying folder, move it and delete old .bin file after folder was successfully moved.
						//! If the .bin file has no accompanying folder, just delete the old .bin file.
						if (!FileExist(srcPathES) || ExpansionStatic.CopyDirectoryTree(srcPathES, dstPathES, extES, true))
						{
							ErrorEx("[P2P Market] Delete old file=" + srcES, ErrorExSeverity.INFO);
							DeleteFile(srcES);
						}
					}
					else
					{
						EXError.Warn(null, string.Format("[P2P Market] Couldn't copy \"%1\" to \"%2\"", srcES, dstES), {});
					}
				}
				else if (!foundESOldFile && data.m_ListingState != ExpansionP2PMarketListingState.SOLD)
				{
					//! If this listing is not sold and has no accompanying .bin file, DON'T save it to new location and return null
					EXError.Error(null, "ExpansionP2PMarketListing::Load - Entity storage file " + srcES + " does not exist anymore!", {});
					return null;
				}
			}
			
			data.m_Version = VERSION;
						
			if (save)
				Save(data);
		}

		//! Make sure stored global IDs are registered. This needs to happen before loading storage!
		if (!data.IsGlobalIDValid())
		{
			EXError.Error(null, "ExpansionP2PMarketListing::Load - " + data.m_ClassName + ": Invalid global ID " + ExpansionStatic.IntToHex(data.m_GlobalID));
			return NULL;
		}

		auto globalID = new ExpansionGlobalID;
		globalID.Set(data.m_GlobalID);

		if (data.m_SkinName)
			data.m_SkinIndex = ExpansionSkinModule.s_Instance.GetSkinIndex(data.m_ClassName, data.m_SkinName);

		foreach (auto containerItem: data.m_ContainerItems)
		{
			containerItem.OnLoad();
		}

		return data;
	}

	static void Save(ExpansionP2PMarketListing listingData)
	{
		string listingsPath = listingData.GetListingDirectory();
		if (!FileExist(listingsPath) && !ExpansionStatic.MakeDirectoryRecursive(listingsPath))
		{
			return;
		}

		string baseName = listingData.GetEntityStorageBaseName();
		ExpansionJsonFileParser<ExpansionP2PMarketListing>.Save(listingsPath + baseName + ".json", listingData);
	}

	void Save()
	{
		Save(this);
	}
	
	void OnSendBasic(ParamsWriteContext ctx, bool writeContainerItems = true)
	{
		ctx.Write(m_TraderID);
		ctx.Write(m_CategoryIndex);
		ctx.Write(m_SubCategoryIndex);
		ctx.Write(m_GlobalID);
		ctx.Write(m_OwnerUID);
		ctx.Write(m_OwnerName);

		OnSendClassName(ctx);

		ctx.Write(m_Price);
		ctx.Write(m_ListingTime);
		ctx.Write(m_SkinIndex);
		ctx.Write(m_FoodStageType);
		ctx.Write(m_ListingState);
		ctx.Write(m_Quantity);
		ctx.Write(m_QuantityType);
		ctx.Write(m_HealthLevel);
		ctx.Write(m_LiquidType);
		ctx.Write(m_IsBloodContainer);
		
		#ifdef EXPANSIONMODHARDLINE
		ctx.Write(m_Rarity);
		#endif
		
		if (writeContainerItems)
		{
			int containerItemsCount = m_ContainerItems.Count();
			ctx.Write(containerItemsCount);
	
			for (int j = 0; j < containerItemsCount; j++)
			{
				ExpansionP2PMarketContainerItem containerItem = m_ContainerItems[j];
				containerItem.OnSendBasic(ctx);
			}
		}
	}
	
	bool OnRecieveBasic(ParamsReadContext ctx, bool readContainerItems = true)
	{
		if (!ctx.Read(m_TraderID))
		{
			Error(ToString() + "::OnRecieveBasic - m_TraderID");
			return false;
		}
		
		if (!ctx.Read(m_CategoryIndex))
		{
			Error(ToString() + "::OnRecieveBasic - m_CategoryIndex");
			return false;
		}
		
		if (!ctx.Read(m_SubCategoryIndex))
		{
			Error(ToString() + "::OnRecieveBasic - m_SubCategoryIndex");
			return false;
		}
		
		if (!ctx.Read(m_GlobalID))
		{
			Error(ToString() + "::OnRecieveBasic - m_GlobalID");
			return false;
		}
		
		if (!ctx.Read(m_OwnerUID))
		{
			Error(ToString() + "::OnRecieveBasic - m_OwnerUID");
			return false;
		}
		
		if (!ctx.Read(m_OwnerName))
		{
			Error(ToString() + "::OnRecieveBasic - m_OwnerName");
			return false;
		}
		
		if (!OnReceiveClassName(ctx))
		{
			Error(ToString() + "::OnRecieveBasic - m_ClassName");
			return false;
		}

		if (!ctx.Read(m_Price))
		{
			Error(ToString() + "::OnRecieveBasic - m_Price");
			return false;
		}
		
		if (!ctx.Read(m_ListingTime))
		{
			Error(ToString() + "::OnRecieveBasic - m_ListingTime");
			return false;
		}
		
		if (!ctx.Read(m_SkinIndex))
		{
			Error(ToString() + "::OnRecieveBasic - m_SkinIndex");
			return false;
		}
		
		if (!ctx.Read(m_FoodStageType))
		{
			Error(ToString() + "::OnRecieveBasic - m_FoodStageType");
			return false;
		}
		
		if (!ctx.Read(m_ListingState))
		{
			Error(ToString() + "::OnRecieveBasic - m_ListingState");
			return false;
		}
		
		if (!ctx.Read(m_Quantity))
		{
			Error(ToString() + "::OnRecieveBasic - m_Quantity");
			return false;
		}
		
		if (!ctx.Read(m_QuantityType))
		{
			Error(ToString() + "::OnRecieveBasic - m_QuantityType");
			return false;
		}
		
		if (!ctx.Read(m_HealthLevel))
		{
			Error(ToString() + "::OnRecieveBasic - m_HealthLevel");
			return false;
		}
		
		if (!ctx.Read(m_LiquidType))
		{
			Error(ToString() + "::OnRecieveBasic - m_LiquidType");
			return false;
		}
		
		if (!ctx.Read(m_IsBloodContainer))
		{
			Error(ToString() + "::OnRecieveBasic - m_IsBloodContainer");
			return false;
		}

		#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(m_Rarity))
		{
			Error(ToString() + "::OnRecieveBasic - m_Rarity");
			return false;
		}
		#endif

		if (readContainerItems)
		{
			int containerItemsCount;
			if (!ctx.Read(containerItemsCount))
			{
				Error(ToString() + "::OnRecieveBasic - containerItemsCount");
				return false;
			}
			
			if (m_ContainerItems.Count())
				m_ContainerItems.Clear();
	
			for (int j = 0; j < containerItemsCount; j++)
			{
				ExpansionP2PMarketContainerItem containerItem = new ExpansionP2PMarketContainerItem();
				if (!containerItem.OnRecieveBasic(ctx))
				{
					Error(ToString() + "::OnRecieveBasic - containerItem");
					return false;
				}
	
				m_ContainerItems.Insert(containerItem);
			}
		}
		
		return true;
	}
	
	void OnSendDetails(ParamsWriteContext ctx)
	{
		OnSendBasic(ctx, false);
		
		int containerItemsCount = m_ContainerItems.Count();
		ctx.Write(containerItemsCount);

		for (int j = 0; j < containerItemsCount; j++)
		{
			ExpansionP2PMarketContainerItem containerItem = m_ContainerItems[j];
			containerItem.OnSendDetails(ctx);
		}
	}
	
	bool OnRecieveDetails(ParamsReadContext ctx)
	{
		if (!OnRecieveBasic(ctx, false))
		{
			Error(ToString() + "::OnRecieveDetails - OnRecieveBasic");
			return false;
		}
		
		int containerItemsCount;
		if (!ctx.Read(containerItemsCount))
		{
			Error(ToString() + "::OnRecieveDetails - containerItemsCount");
			return false;
		}
		
		if (m_ContainerItems.Count())
			m_ContainerItems.Clear();

		for (int j = 0; j < containerItemsCount; j++)
		{
			ExpansionP2PMarketContainerItem containerItem = new ExpansionP2PMarketContainerItem();
			if (!containerItem.OnRecieveDetails(ctx))
			{
				Error(ToString() + "::OnRecieveDetails - containerItem");
				return false;
			}

			m_ContainerItems.Insert(containerItem);
		}
		
		return true;
	}
};