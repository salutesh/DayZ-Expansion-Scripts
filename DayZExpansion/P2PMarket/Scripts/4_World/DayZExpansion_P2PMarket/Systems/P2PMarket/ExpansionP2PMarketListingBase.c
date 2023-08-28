/**
 * ExpansionP2PMarketListingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketListingBase
{
	[NonSerialized()];
	protected EntityAI m_ListingObject;
	
	int m_Version;
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
	autoptr array<ref ExpansionP2PMarketContainerItem> m_ContainerItems;

	[NonSerialized()];
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

	int GetContainerItemsCount()
	{
		return m_ContainerItemsCount;
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
			string ammoClassName = GetGame().ConfigGetTextOut("CfgAmmo " + ammoName + " spawnPileType");
			EXPrint(ToString() + "::GetAmmoForMagazine - Ammo: " + ammoClassName + ", " + "Count: " + count);

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

				EXPrint(ToString() + "::GetAmmoForMagazine - Add entry for ammo: " + ammoClassName);
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
				m_ContainerItemsCount++;
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
		
		if (!ExpansionP2PMarketModule.ItemCheck(object))
			m_IsExcluded = true;
	}

	void UpdateContainerItems()
	{
		if (!m_ListingObject)
			return;

		array< EntityAI > items = new array< EntityAI >;
		m_ListingObject.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		m_ContainerItems.Clear();
		m_ContainerItemsCount = 0;

		//! If the main item entity is a maganzine we want to get the ammo data and create a container entry for it.
		if (m_ListingObject.IsInherited(MagazineStorage))
			GetAmmoForMagazine(m_ListingObject);

		for (int i = 0; i < items.Count(); i++)
		{
			EntityAI item = items[i];
			if (item == m_ListingObject)
				continue;

			if (item.GetInventory().IsAttachment() && m_ListingObject.GetInventory().HasAttachment(item) || item.GetInventory().IsInCargo() && m_ListingObject.GetInventory().HasEntityInCargo(item))
			{
				//! Hardcoded excluded type names where the item should never get added and shown in the menu.
				if (ExpansionP2PMarketModule.m_HardcodedExcludes.Find(item.GetType()) > -1)
					continue;

				if (!ExpansionP2PMarketModule.ItemCheck(item))
					m_IsExcluded = true;

				ExpansionP2PMarketContainerItem containerItem = new ExpansionP2PMarketContainerItem();
				containerItem.SetFromItem(item);
				
				if (!containerItem.IsExcluded() && m_IsExcluded)
					containerItem.SetExcluded(true);
				else if (!m_IsExcluded && containerItem.IsExcluded())
					m_IsExcluded = true;
	
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

		if (!m_ContainerItemsCount)
		{
			//! Inventory not yet initialized on client, use netsynched cargo count
			CarScript car;
			if (Class.CastTo(car, m_ListingObject))
			{
				m_ContainerItemsCount = car.m_Expansion_CargoCount;
			}
		#ifdef EXPANSIONMODVEHICLE
			else
			{
				ExpansionVehicleBase vehicle;
				if (Class.CastTo(vehicle, m_ListingObject))
				{
					m_ContainerItemsCount = vehicle.m_Expansion_CargoCount;
				}
			}
		#endif
		}
	}
};