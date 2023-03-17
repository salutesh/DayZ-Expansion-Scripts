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
	static const int VERSION = 1;
	[NonSerialized()]
	bool m_IsStoredItem;

	int m_Version;
	int m_StorageID = -1;
	autoptr TIntArray m_GlobalID;
	string m_ItemSlotName;

	void ExpansionPersonalStorageItem()
	{
		m_Version = VERSION;

		m_GlobalID = {0, 0, 0, 0};
	}

	override void SetFromItem(EntityAI object, string playerUID)
	{
		super.SetFromItem(object, playerUID);

		ItemBase itemIB;
		if (Class.CastTo(itemIB, object))
		{
			auto globalID = new ExpansionGlobalID;
			globalID.Acquire();

			for (int j = 0; j < 4; j++)
				m_GlobalID[j] = globalID.m_ID[j];
		}
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

	string GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(ExpansionStatic.IntToHex(m_GlobalID));
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

	static ExpansionPersonalStorageItem Load(string fileName)
	{
		Print("[ExpansionPersonalStorageItem] Load existing P2P market listing file:" + fileName);

		bool save;
		ExpansionPersonalStorageItem data = new ExpansionPersonalStorageItem();
		if (data.m_Version < VERSION)
		{
			save = true;
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
		string storagePath = ExpansionPersonalStorageModule.s_PersonalStorageDataFolderPath + data.GetOwnerUID() + "\\";
		auto globalID = new ExpansionGlobalID;
		globalID.Set(data.GetGlobalID());
		string fileName = globalID.IDToHex();
		ExpansionJsonFileParser<ExpansionPersonalStorageItem>.Save(storagePath + fileName + ".json", data);
	}

	void Save()
	{
		Save(this);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_IsStoredItem);
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
		if (!ctx.Read(m_IsStoredItem))
		{
			Error(ToString() + "::OnRecieve - m_IsStoredItem");
			return false;
		}

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