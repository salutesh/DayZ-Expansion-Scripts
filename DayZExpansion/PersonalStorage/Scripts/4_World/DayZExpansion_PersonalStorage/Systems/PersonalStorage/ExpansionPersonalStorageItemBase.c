/**
 * ExpansionPersonalStorageItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageItemBase
{
	[NonSerialized()];
	protected EntityAI m_Object;
	
	int m_Version;
	
	string m_OwnerUID;
	string m_ClassName;
	string m_SkinName;

	[NonSerialized()];
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

	int m_ContainerItemsCount;
	autoptr array<ref ExpansionPersonalStorageContainerItem> m_ContainerItems;

	[NonSerialized()];
	protected bool m_IsExcluded;

	void ExpansionPersonalStorageItemBase()
	{
		m_ContainerItems = new array<ref ExpansionPersonalStorageContainerItem>;
	}

	string GetClassName()
	{
		return m_ClassName;
	}

	void SetClassName(string name)
	{
		m_ClassName = name;
	}

	int GetContainerItemsCount()
	{
		return m_ContainerItemsCount;
	}

	array<ref ExpansionPersonalStorageContainerItem> GetContainerItems()
	{
		return m_ContainerItems;
	}

	EntityAI GetItem()
	{
		return m_Object;
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
			string ammoClassName = GetGame().ConfigGetTextOut("CfgAmmo " + ammoName + " spawnPileType");
			Print(ToString() + "::GetAmmoForMagazine - Ammo: " + ammoClassName + ", " + "Count: " + count);

			while (count > 0)
			{
				Object obj = GetGame().CreateObjectEx(ammoClassName, "0 0 0", ECE_LOCAL);
				Ammunition_Base ammo;
				if (!Class.CastTo(ammo, obj))
				{
					GetGame().ObjectDelete(obj);
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

				Print(ToString() + "::GetAmmoForMagazine - Add entry for ammo: " + ammoClassName);
				ExpansionPersonalStorageContainerItem ammoItem = new ExpansionPersonalStorageContainerItem();
				ammoItem.SetFromItem(ammo, m_OwnerUID);
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
				m_ContainerItemsCount++;
			}
		}
	}

	void SetFromItem(EntityAI object, string playerUID)
	{
		m_Object = object;
		m_ClassName = object.GetType();
		m_OwnerUID = playerUID;

		if (!ExpansionPersonalStorageModule.ItemCheck(object))
			m_IsExcluded = true;
		
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
	}

	void UpdateContainerItems()
	{
		if (!m_Object)
			return;

		array<EntityAI> items = new array<EntityAI>;
		m_Object.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		m_ContainerItems.Clear();
		m_ContainerItemsCount = 0;

		//! If the main item entity is a maganzine we want to get the ammo data and create a container entry for it.
		if (m_Object.IsInherited(MagazineStorage))
			GetAmmoForMagazine(m_Object);

		for (int i = 0; i < items.Count(); i++)
		{
			EntityAI item = items[i];
			if (item == m_Object)
				continue;

			bool excluded = false;
			if (item.GetInventory().IsAttachment() && m_Object.GetInventory().HasAttachment(item) || item.GetInventory().IsInCargo() && m_Object.GetInventory().HasEntityInCargo(item))
			{
				//! Skip attachments without cargo on vehicles
				if (item.GetInventory().IsAttachment() && !MiscGameplayFunctions.Expansion_HasAnyCargo(item))
				{
					if (item.GetHierarchyParent().IsInherited(CarScript))
						continue;
				#ifdef EXPANSIONMODVEHICLE
					else if (item.GetHierarchyParent().IsInherited(ExpansionVehicleBase))
						continue;
				#endif
				}

				//! Hardcoded excluded type names where the item should never get added and shown in the menu.
				if (ExpansionPersonalStorageModule.m_HardcodedExcludes.Find(item.GetType()) > -1)
					continue;

				if (!ExpansionPersonalStorageModule.ItemCheck(item))
				{
					excluded = true;
					m_IsExcluded = true;
				}

				ExpansionPersonalStorageContainerItem containerItem = new ExpansionPersonalStorageContainerItem();
				containerItem.SetFromItem(item, m_OwnerUID);
				containerItem.SetExcluded(excluded);
				m_ContainerItems.Insert(containerItem);
				m_ContainerItemsCount++;

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
	}
};