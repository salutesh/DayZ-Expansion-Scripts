/**
 * ExpansionP2PMarketListingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketListingBase
{
	[NonSerialized()]
	protected EntityAI m_ListingObject;
	
	int m_Version;
	string m_ClassName;
	string m_SkinName;

	[NonSerialized()]
	int m_SkinIndex;

	int m_HealthLevel;
	float m_Quantity;
	int m_QuantityType;
	int m_LiquidType = -1;
	bool m_IsBloodContainer;
	int m_FoodStageType = -1;

	#ifdef EXPANSIONMODHARDLINE
	ExpansionHardlineItemRarity m_Rarity = ExpansionHardlineItemRarity.NONE;
	#endif

	autoptr array<ref ExpansionP2PMarketContainerItem> m_ContainerItems;

	[NonSerialized()]
	protected bool m_IsExcluded;

	void ExpansionP2PMarketListingBase()
	{
		m_ContainerItems = new array<ref ExpansionP2PMarketContainerItem>;
	}

	string GetClassName()
	{
		return m_ClassName;
	}

	void SetClassName(string name)
	{
		m_ClassName = name;
	}

	array<ref ExpansionP2PMarketContainerItem> GetContainerItems()
	{
		return m_ContainerItems;
	}

	EntityAI GetItem()
	{
		return m_ListingObject;
	}

	int GetHealthLevel()
	{
		return m_HealthLevel;
	}

	void SetHealthLevel(int healthLevel)
	{
		m_HealthLevel = healthLevel;
	}

	int GetLiquidType()
	{
		return m_LiquidType;
	}

	void SetLiquidType(int liquidType)
	{
		m_LiquidType = liquidType;
	}

	bool IsBloodContainer()
	{
		return m_IsBloodContainer;
	}

	void SetLiquidIsBloodContainer(bool state)
	{
		m_IsBloodContainer = state;
	}

	int GetFoodStageType()
	{
		return m_FoodStageType;
	}

	void SetFoodStageType(int foodStage)
	{
		m_FoodStageType = foodStage;
	}

	float GetQuantity()
	{
		return m_Quantity;
	}

	void SetQuantity(float quantity)
	{
		m_Quantity = quantity;
	}

	int GetQuantityType()
	{
		return m_QuantityType;
	}

	void SetQuantityType(int type)
	{
		m_QuantityType = type;
	}

	#ifdef EXPANSIONMODHARDLINE
	void SetRarity(ExpansionHardlineItemRarity rarity)
	{
		m_Rarity = rarity;
	}

	ExpansionHardlineItemRarity GetRarity()
	{
		return m_Rarity;
	}
	#endif

	string GetSkinName()
	{
		return m_SkinName;
	}

	int GetSkinIndex()
	{
		return m_SkinIndex;
	}

	void SetExcluded(bool state)
	{
		m_IsExcluded = state;
	}

	bool IsExcluded()
	{
		return m_IsExcluded;
	}

	void GetAmmoForMagazine(EntityAI entity)
	{
		//! If the object entity is a maganzine we want to get the ammo data and create a container entry for it.
		MagazineStorage magazine;
		if (!Class.CastTo(magazine, entity))
			return;

		map<string, int> ammoTypes = new map<string, int>;
		for (int j = 0; j < magazine.GetAmmoCount(); j++)
		{
			float damage;
			string cartTypeName;
			magazine.GetCartridgeAtIndex(j, damage, cartTypeName);

			int current;
			if (!ammoTypes.Find(cartTypeName, current))
			{
				ammoTypes.Insert(cartTypeName, 1);
			}
			else
			{
				current += 1;
				ammoTypes.Set(cartTypeName, current);
			}
		}

		foreach (string ammoName, int count: ammoTypes)
		{
			string ammoClassName = g_Game.ConfigGetTextOut("CfgAmmo " + ammoName + " spawnPileType");
			EXTrace.Print(EXTrace.P2PMARKET, this, "::GetAmmoForMagazine - Ammo: " + ammoClassName + ", " + "Count: " + count);

			while (count > 0)
			{
				Object obj = g_Game.CreateObjectEx(ammoClassName, "0 0 0", ECE_LOCAL);
				Ammunition_Base ammo;
				if (!Class.CastTo(ammo, obj))
				{
					g_Game.ObjectDelete(obj);
					continue;
				}

				ammo.LocalSetAmmoCount(count);

				int quantity = ammo.GetAmmoCount();
				if (quantity < count)
				{
					count -= quantity;
				}
				else
				{
					count -= count;
				}

				EXTrace.Print(EXTrace.P2PMARKET, this, "::GetAmmoForMagazine - Add entry for ammo: " + ammoClassName);
				ExpansionP2PMarketContainerItem ammoItem = new ExpansionP2PMarketContainerItem();
				ammoItem.SetFromItem(ammo);
				ammoItem.SetExcluded(true);

				#ifdef EXPANSIONMODHARDLINE
				auto settings = GetExpansionSettings().GetHardline();
				if (settings.EnableItemRarity)
				{
					ExpansionHardlineItemRarity rarity = ammo.Expansion_GetRarity();
					if (rarity > ExpansionHardlineItemRarity.NONE)
						ammoItem.SetRarity(rarity);
				}
				#endif

				m_ContainerItems.Insert(ammoItem);
			}
		}
	}

	void SetFromItem(EntityAI object, PlayerBase owner = null)
	{
		m_ListingObject = object;
		m_ClassName = object.GetType();

		UpdateContainerItems();

		ItemBase itemIB;
		if (Class.CastTo(itemIB, object))
		{
			ExpansionItemQuantityType quantityType;
			m_Quantity = itemIB.Expansion_GetQuantity(quantityType);
			m_QuantityType = quantityType;

			if (itemIB.ExpansionGetCurrentSkinName() != string.Empty)
			{
				m_SkinName = itemIB.ExpansionGetCurrentSkinName();
				m_SkinIndex = itemIB.ExpansionGetCurrentSkinIndex();
			}

			m_HealthLevel = itemIB.GetHealthLevel();

			if (itemIB.GetQuantity() > 0 && itemIB.IsBloodContainer())
			{
				m_IsBloodContainer = true;
				BloodContainerBase blood_container = BloodContainerBase.Cast(itemIB);
				if (blood_container && blood_container.GetBloodTypeVisible())
					m_LiquidType = blood_container.GetLiquidType();
			}
			else if (itemIB.GetQuantity() > 0 && itemIB.IsLiquidContainer())
			{
				m_LiquidType = itemIB.GetLiquidType();
			}

			Edible_Base foodItem = Edible_Base.Cast(itemIB);
			if (foodItem && foodItem.HasFoodStage())
			{
				FoodStage foodStage = foodItem.GetFoodStage();
				FoodStageType foodStageType = foodStage.GetFoodStageType();
				m_FoodStageType = foodStageType;
			}

			#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().EnableItemRarity)
			{
				ExpansionHardlineItemRarity rarity = itemIB.Expansion_GetRarity();
				if (rarity > ExpansionHardlineItemRarity.NONE)
					m_Rarity = rarity;
			}
			#endif
		}
		
		if (!ExpansionP2PMarketModule.ItemCheckEx(object))
			m_IsExcluded = true;
	}

	void UpdateContainerItems()
	{
		if (!m_ListingObject)
			return;

		array<EntityAI> items = MiscGameplayFunctions.Expansion_GetItems(m_ListingObject);

		m_ContainerItems.Clear();

		//! If the main item entity is a maganzine we want to get the ammo data and create a container entry for it.
		if (m_ListingObject.IsInherited(MagazineStorage))
			GetAmmoForMagazine(m_ListingObject);

		InventoryLocation lcn = new InventoryLocation();

		foreach (EntityAI item: items)
		{
			lcn.Reset();
			item.GetInventory().GetCurrentInventoryLocation(lcn);

			if (lcn.GetType() == InventoryLocationType.ATTACHMENT)
			{
				//! Skip items in hidden attachment slots
				if (!InventorySlots.GetShowForSlotId(lcn.GetSlot()))
					continue;
			}

			//! Hardcoded excluded type names where the item should never get added and shown in the menu.
			if (ExpansionP2PMarketModule.m_HardcodedExcludes.Find(item.GetType()) > -1)
				continue;

			if (!ExpansionP2PMarketModule.ItemCheckEx(item))
				m_IsExcluded = true;

			ExpansionP2PMarketContainerItem containerItem = new ExpansionP2PMarketContainerItem();
			containerItem.SetFromItem(item);
			
			if (!m_IsExcluded && containerItem.IsExcluded())
				m_IsExcluded = true;
			
			//! If item is a BB kit then make sure we cant deposit the attached rope..
			KitBase kitBase;
			if (Class.CastTo(kitBase, m_ListingObject) && item.GetType() == "Rope")
				containerItem.SetExcluded(true);

			m_ContainerItems.Insert(containerItem);

			#ifdef EXPANSIONMODHARDLINE
			ItemBase itemIB;
			if (Class.CastTo(itemIB, item))
			{
				auto settings = GetExpansionSettings().GetHardline();
				if (settings.EnableItemRarity)
				{
					ExpansionHardlineItemRarity rarity = itemIB.Expansion_GetRarity();
					if (rarity != ExpansionHardlineItemRarity.NONE)
					{
						containerItem.SetRarity(rarity);
					}
				}
			}
			#endif
		}
	}

	void OnSendClassName(ParamsWriteContext ctx)
	{
		//! TODO: Use ExpansionBitStreamWriter
		if (m_ClassName.Length() > 0)
		{
			ExpansionString className = m_ClassName;
			className.ToLower();

			int hashA;
			int hashB;
			ExpansionItemNameTable.Hash(className, hashA, hashB);

			if (ExpansionItemNameTable.GetTypeByHash(hashA, hashB) && !ExpansionItemNameTable.IsHashColliding(className))
			{
				//! 8 bytes (4 bytes bool written as int + 6 bytes hash + 2 bytes null)
				ctx.Write(true);
				ExpansionItemNameTable.WriteHash(ctx, hashA, hashB);

				return;
			}
		}

		//! 4 bytes bool written as int + 4 bytes string length + 1 byte per char
		ctx.Write(false);
		ctx.Write(m_ClassName);
	}

	bool OnReceiveClassName(ParamsReadContext ctx)
	{
		//! TODO: Use ExpansionBitStreamReader
		bool isHashed;

		if (!ctx.Read(isHashed))
			return false;

		if (isHashed)
		{
			int hashA;
			int hashB;
			if (!ExpansionItemNameTable.ReadHash(ctx, hashA, hashB))
				return false;

			m_ClassName = ExpansionItemNameTable.GetTypeByHash(hashA, hashB);
		}
		else
		{
			if (!ctx.Read(m_ClassName))
				return false;
		}

		return true;
	}
};