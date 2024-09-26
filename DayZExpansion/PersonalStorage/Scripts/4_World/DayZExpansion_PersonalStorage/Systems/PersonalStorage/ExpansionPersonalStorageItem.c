/**
 * ExpansionPersonalStorageItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageItem: ExpansionPersonalStorageItemBase
{
	[NonSerialized()];
	static const int VERSION = 2;
	[NonSerialized()]
	bool m_IsStoredItem;

	int m_StorageID = -1;
	autoptr TIntArray m_GlobalID;
	string m_ItemSlotName;
	int m_StoreTime = -1;

	void ExpansionPersonalStorageItem()
	{
		m_Version = VERSION;

		m_GlobalID = {0, 0, 0, 0};
	}

	override void SetFromItem(EntityAI object, string playerUID)
	{
		super.SetFromItem(object, playerUID);

#ifdef SERVER
		ItemBase itemIB;
		if (Class.CastTo(itemIB, object))
		{
			if (!itemIB.m_Expansion_GlobalID)
				itemIB.m_Expansion_GlobalID = new ExpansionGlobalID;

			if (!itemIB.m_Expansion_GlobalID.m_IsSet)
				itemIB.m_Expansion_GlobalID.Acquire();

			for (int j = 0; j < 4; j++)
				m_GlobalID[j] = itemIB.m_Expansion_GlobalID.m_ID[j];
		}
#endif
	}

	void SetFromContainerItem(ExpansionPersonalStorageContainerItem item, int storageID, TIntArray globalID, string playerUID, bool isStored)
	{
		m_Object = item.GetItem();
		m_StorageID = storageID;
		m_GlobalID = globalID;
		m_OwnerUID = playerUID;
		m_ClassName = item.GetClassName();
		m_Quantity = item.GetQuantity();
		m_QuantityType = item.GetQuantityType();
		m_SkinName = item.GetSkinName();
		m_SkinIndex = item.GetSkinIndex();
		m_ContainerItems = item.GetContainerItems();
		m_ContainerItemsCount = item.GetContainerItems().Count();
		m_HealthLevel = item.GetHealthLevel();
		m_LiquidType = item.GetLiquidType();
		m_IsBloodContainer = item.IsBloodContainer();
		m_FoodStageType = item.GetFoodStageType();
		m_IsStoredItem = isStored;

	#ifdef EXPANSIONMODHARDLINE
		m_Rarity = item.GetRarity();
	#endif

		m_IsExcluded = item.IsExcluded();
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

	string GetEntityStorageBaseName()
	{
		return ExpansionStatic.IntToHex(m_GlobalID);
	}

	string GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(GetEntityStorageBaseName());
	}

	void SetIsStored(bool state)
	{
		m_IsStoredItem = state;
	}

	bool IsStored()
	{
		return m_IsStoredItem;
	}

	void SetStorageID(int storageID)
	{
		m_StorageID = storageID;
	}

	int GetStorageID()
	{
		return m_StorageID;
	}

	void SetSlotName(string slotName)
	{
		m_ItemSlotName = slotName;
	}

	string GetSlotName()
	{
		return m_ItemSlotName;
	}
	
	void SetStoreTime()
	{
		m_StoreTime = CF_Date.Now(true).GetTimestamp();
	}

	int GetStoreTime()
	{
		return m_StoreTime;
	}
	
	void CopyFromBaseClass(ExpansionPersonalStorageItemBase base)
	{
		m_OwnerUID = base.m_OwnerUID;
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

	static ExpansionPersonalStorageItem Load(string fileName)
	{
		CF_Log.Info("[ExpansionPersonalStorageItem] Load existing personal storage item:" + fileName);
		ExpansionPersonalStorageItemBase dataBase;
		if (!ExpansionJsonFileParser<ExpansionPersonalStorageItemBase>.Load(fileName, dataBase))
			return NULL;
		
		bool save;
		ExpansionPersonalStorageItem data = new ExpansionPersonalStorageItem();
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
			if (!ExpansionJsonFileParser<ExpansionPersonalStorageItem>.Load(fileName, data))
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

	static void Save(ExpansionPersonalStorageItem data)
	{
		string storagePath = ExpansionPersonalStorageModule.GetPersonalStorageDataDirectory() + data.GetOwnerUID() + "\\";
		if (!FileExist(storagePath) && !ExpansionStatic.MakeDirectoryRecursive(storagePath))
			return;
		string fileName = ExpansionStatic.IntToHex(data.GetGlobalID());
		ExpansionJsonFileParser<ExpansionPersonalStorageItem>.Save(storagePath + fileName + ".json", data);
	}

	void Save()
	{
		Save(this);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_StorageID);

		ctx.Write(m_GlobalID);

		ctx.Write(m_OwnerUID);
		ctx.Write(m_ClassName);

		ctx.Write(m_Quantity);
		ctx.Write(m_QuantityType);

		ctx.Write(m_SkinIndex);

		ctx.Write(m_HealthLevel);

		ctx.Write(m_LiquidType);
		ctx.Write(m_IsBloodContainer);
		ctx.Write(m_FoodStageType);

		ctx.Write(m_ContainerItemsCount);

	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(m_Rarity);
	#endif
		
		ctx.Write(m_StoreTime);

		int containerItemsCount = m_ContainerItems.Count();
		ctx.Write(containerItemsCount);

		for (int j = 0; j < containerItemsCount; j++)
		{
			ExpansionPersonalStorageContainerItem containerItem = m_ContainerItems[j];
			containerItem.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_StorageID))
		{
			Error(ToString() + "::OnRecieve - m_StorageID");
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

		if (!ctx.Read(m_ClassName))
		{
			Error(ToString() + "::OnRecieve - m_ClassName");
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

	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(m_Rarity))
			return false;
	#endif
		
		if (!ctx.Read(m_StoreTime))
		{
			Error(ToString() + "::OnRecieve - m_StoreTime");
			return false;
		}

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
			ExpansionPersonalStorageContainerItem containerItem = new ExpansionPersonalStorageContainerItem();
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