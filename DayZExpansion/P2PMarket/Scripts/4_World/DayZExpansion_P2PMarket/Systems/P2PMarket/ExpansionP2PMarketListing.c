/**
 * ExpansionP2PMarketListing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	[NonSerialized()];
	static const int VERSION = 1;

	[NonSerialized()];
	protected int m_TraderID;

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

		CarScript car;
		if (Class.CastTo(car, object))
		{
			if (!car.m_Expansion_GlobalID.m_IsSet)
				car.m_Expansion_GlobalID.Acquire();

			for (int i = 0; i < 4; i++)
				m_GlobalID[i] = car.m_Expansion_GlobalID.m_ID[i];

		#ifdef EXPANSIONMODVEHICLE
			if (car.ExpansionGetCurrentSkinName() != string.Empty)
			{
				m_SkinName = car.ExpansionGetCurrentSkinName();
				m_SkinIndex = car.ExpansionGetCurrentSkinIndex();
			}
		#endif
		}

		ItemBase itemIB;
		if (Class.CastTo(itemIB, object))
		{
			auto globalID = new ExpansionGlobalID;
			globalID.Acquire();

			for (int j = 0; j < 4; j++)
				m_GlobalID[j] = globalID.m_ID[j];
		}

		//m_ListingState = ExpansionP2PMarketListingState.LISTED;
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
		for (int i = 0; i < 4; i++)
		{
			if (m_GlobalID[i] != id[i])
				return false;
		}

		return true;
	}

	bool IsGlobalIDEqual(ExpansionP2PMarketListing listing)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_GlobalID[i] != listing.m_GlobalID[i])
				return false;
		}

		return true;
	}

	string GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(ExpansionStatic.IntToHex(m_GlobalID));
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
		
		m_ContainerItemsCount = base.m_ContainerItemsCount;
		m_ContainerItems = base.m_ContainerItems;
	}
	
	static ExpansionP2PMarketListing Load(string fileName)
	{
		CF_Log.Info("[ExpansionP2PMarketListing]  Load existing P2P market listing file:" + fileName);
		ExpansionP2PMarketListingBase dataBase;
		if (!ExpansionJsonFileParser<ExpansionP2PMarketListingBase>.Load(fileName, dataBase))
			return NULL;
		
		bool save;
		ExpansionP2PMarketListing data = new ExpansionP2PMarketListing();
		if (dataBase.m_Version < VERSION)
		{
			save = true;
			data.CopyFromBaseClass(dataBase);
			data.m_Version = VERSION;
			
			if (save)
				Save(data);
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionP2PMarketListing>.Load(fileName, data))
				return NULL;
		}

		//! Make sure stored global IDs are registered. This needs to happen before loading storage!
		if (!data.IsGlobalIDValid())
		{
			Error(data.m_ClassName + ": Invalid global ID " + ExpansionStatic.IntToHex(data.m_GlobalID));
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
		string traderListingsPath = ExpansionP2PMarketModule.GetP2PMarketDataDirectory() + "P2PTrader_" + listingData.GetTraderID() + "_Listings\\";
		if (!FileExist(traderListingsPath) && !ExpansionStatic.MakeDirectoryRecursive(traderListingsPath))
			return;
		string fileName = ExpansionStatic.IntToHex(listingData.GetGlobalID());
		ExpansionJsonFileParser<ExpansionP2PMarketListing>.Save(traderListingsPath + fileName + ".json", listingData);
	}

	void Save()
	{
		Save(this);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_TraderID);

		ctx.Write(m_GlobalID);

		ctx.Write(m_OwnerUID);
		ctx.Write(m_OwnerName);
		ctx.Write(m_ClassName);
		ctx.Write(m_Price);
		ctx.Write(m_ListingTime);

		ctx.Write(m_Quantity);
		ctx.Write(m_QuantityType);

		ctx.Write(m_SkinIndex);

		ctx.Write(m_HealthLevel);

		ctx.Write(m_LiquidType);
		ctx.Write(m_IsBloodContainer);
		ctx.Write(m_FoodStageType);

		ctx.Write(m_ContainerItemsCount);

		ctx.Write(m_ListingState);

	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(m_Rarity);
	#endif

		int containerItemsCount = m_ContainerItems.Count();
		ctx.Write(containerItemsCount);

		for (int j = 0; j < containerItemsCount; j++)
		{
			ExpansionP2PMarketContainerItem containerItem = m_ContainerItems[j];
			containerItem.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_TraderID))
		{
			Error(ToString() + "::OnRecieve - m_TraderID");
			return false;
		}

		if (!ctx.Read(m_GlobalID))
		{
			Error(ToString() + "::OnRecieve - m_GlobalID");
			return false;
		}

		if (!ctx.Read(m_OwnerUID))
		{
			Error(ToString() + "::OnRecieve - m_OwnerUID");
			return false;
		}

		if (!ctx.Read(m_OwnerName))
		{
			Error(ToString() + "::OnRecieve - m_OwnerName");
			return false;
		}

		if (!ctx.Read(m_ClassName))
		{
			Error(ToString() + "::OnRecieve - m_ClassName");
			return false;
		}

		if (!ctx.Read(m_Price))
		{
			Error(ToString() + "::OnRecieve - m_Price");
			return false;
		}

		if (!ctx.Read(m_ListingTime))
		{
			Error(ToString() + "::OnRecieve - m_ListingTime");
			return false;
		}

		if (!ctx.Read(m_Quantity))
		{
			Error(ToString() + "::OnRecieve - m_Quantity");
			return false;
		}

		if (!ctx.Read(m_QuantityType))
		{
			Error(ToString() + "::OnRecieve - m_QuantityType");
			return false;
		}

		if (!ctx.Read(m_SkinIndex))
		{
			Error(ToString() + "::OnRecieve - m_SkinIndex");
			return false;
		}

		if (!ctx.Read(m_HealthLevel))
		{
			Error(ToString() + "::OnRecieve - m_HealthLevel");
			return false;
		}

		if (!ctx.Read(m_LiquidType))
		{
			Error(ToString() + "::OnRecieve - m_LiquidType");
			return false;
		}

		if (!ctx.Read(m_IsBloodContainer))
		{
			Error(ToString() + "::OnRecieve - m_IsBloodContainer");
			return false;
		}

		if (!ctx.Read(m_FoodStageType))
		{
			Error(ToString() + "::OnRecieve - m_FoodStageType");
			return false;
		}

		if (!ctx.Read(m_ContainerItemsCount))
		{
			Error(ToString() + "::OnRecieve - m_ContainerItemsCount");
			return false;
		}

		if (!ctx.Read(m_ListingState))
		{
			Error(ToString() + "::OnRecieve - m_ListingState");
			return false;
		}

	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(m_Rarity))
		{
			Error(ToString() + "::OnRecieve - m_Rarity");
			return false;
		}
	#endif

		int containerItemsCount;
		if (!ctx.Read(containerItemsCount))
		{
			Error(ToString() + "::OnRecieve - containerItemsCount");
			return false;
		}

		if (m_ContainerItems.Count())
			m_ContainerItems.Clear();

		for (int j = 0; j < containerItemsCount; j++)
		{
			ExpansionP2PMarketContainerItem containerItem = new ExpansionP2PMarketContainerItem();
			if (!containerItem.OnRecieve(ctx))
			{
				Error(ToString() + "::OnRecieve - containerItem");
				return false;
			}

			m_ContainerItems.Insert(containerItem);
		}

		return true;
	}
};