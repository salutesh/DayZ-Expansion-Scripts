/**
 * ExpansionPersonalStorageContainerItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageContainerItem: ExpansionPersonalStorageItemBase
{
	protected bool m_IsWeapon = false;
	protected bool m_IsMagazine = false;
	protected bool m_IsAttached = false;
	
	override void SetFromItem(EntityAI object, string playerUID)
	{
		Print(ToString() + "::SetFromItem - Start");
		Print(ToString() + "::SetFromItem - Item: " + object.GetType());

		super.SetFromItem(object, playerUID);

		if (object.IsWeapon())
			m_IsWeapon = true;

		if (object.GetInventory().IsAttachment())
		{
			if (!object.GetHierarchyParent().IsInherited(SurvivorBase))
				m_IsAttached = true;
		}

		if (object.IsInherited(MagazineStorage))
			m_IsMagazine = true;
		
		Print(ToString() + "::SetFromItem - End");
	}

	bool IsAttached()
	{
		return m_IsAttached;
	}

	bool IsMagazine()
	{
		return m_IsMagazine;
	}

	bool IsWeapon()
	{
		return m_IsWeapon;
	}
	
	void OnLoad()
	{
		if (m_SkinName)
			m_SkinIndex = ExpansionSkinModule.s_Instance.GetSkinIndex(m_ClassName, m_SkinName);

		foreach (auto containerItem: m_ContainerItems)
		{
			containerItem.OnLoad();
		}
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_ClassName);
		ctx.Write(m_Quantity);
		ctx.Write(m_SkinIndex);
		ctx.Write(m_QuantityType);
		ctx.Write(m_HealthLevel);
		ctx.Write(m_LiquidType);
		ctx.Write(m_IsBloodContainer);
		ctx.Write(m_FoodStageType);
		ctx.Write(m_IsWeapon);
		ctx.Write(m_IsMagazine);
		ctx.Write(m_IsAttached);
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(m_Rarity);
	#endif
		ctx.Write(m_ContainerItemsCount);
		int containerItemsCount = m_ContainerItems.Count();
		ctx.Write(containerItemsCount);

		for (int i = 0; i < containerItemsCount; i++)
		{
			ExpansionPersonalStorageContainerItem containerItem = m_ContainerItems[i];
			containerItem.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_ClassName))
			return false;

		if (!ctx.Read(m_Quantity))
			return false;

		if (!ctx.Read(m_SkinIndex))
			return false;

		if (!ctx.Read(m_QuantityType))
			return false;

		if (!ctx.Read(m_HealthLevel))
			return false;

		if (!ctx.Read(m_LiquidType))
			return false;

		if (!ctx.Read(m_IsBloodContainer))
			return false;

		if (!ctx.Read(m_FoodStageType))
			return false;

		if (!ctx.Read(m_IsWeapon))
			return false;

		if (!ctx.Read(m_IsMagazine))
			return false;

		if (!ctx.Read(m_IsAttached))
			return false;
		
	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(m_Rarity))
			return false;
	#endif
		if (!ctx.Read(m_ContainerItemsCount))
			return false;

		int containerItemsCount;
		if (!ctx.Read(containerItemsCount))
			return false;

		if (m_ContainerItems.Count())
			m_ContainerItems.Clear();

		for (int i = 0; i < containerItemsCount; i++)
		{
			ExpansionPersonalStorageContainerItem containerItem = new ExpansionPersonalStorageContainerItem();
			if (!containerItem.OnRecieve(ctx))
				return false;

			m_ContainerItems.Insert(containerItem);
		}

		return true;
	}
};

/*class ExpansionPersonalStorageContainerItem
{
	[NonSerialized()];
	protected EntityAI m_Item;

	protected string m_ClassName;
	protected float m_Quantity;
	protected string m_SkinName;
	protected int m_SkinIndex;
	protected int m_QuantityType;
	protected int m_HealthLevel;
	protected int m_LiquidType = -1;
	protected bool m_IsBloodContainer;
	protected int m_FoodStageType = -1;
	protected bool m_IsWeapon = false;
	protected bool m_IsMagazine = false;
	protected bool m_IsAttached = false;
#ifdef EXPANSIONMODHARDLINE
	protected ExpansionHardlineItemRarity m_Rarity = ExpansionHardlineItemRarity.NONE;
#endif

	protected autoptr array<ref ExpansionPersonalStorageContainerItem> m_ContainerItems;
	protected int m_ContainerItemsCount;
	protected bool m_IsExcluded;

	void ExpansionPersonalStorageContainerItem()
	{
		if (!m_ContainerItems)
			m_ContainerItems = new array<ref ExpansionPersonalStorageContainerItem>;
	}

	void SetDataFromItem(EntityAI item)
	{
		m_Item = item;
		m_ClassName = item.GetType();

		ItemBase itemIB;
		if (Class.CastTo(itemIB, item))
		{
			ExpansionItemQuantityType quantityType;
			m_Quantity = itemIB.Expansion_GetQuantity(quantityType);
			m_QuantityType = quantityType;

			if (itemIB.ExpansionGetCurrentSkinName() != string.Empty)
			{
				m_SkinName = itemIB.ExpansionGetCurrentSkinName();
				m_SkinIndex = itemIB.ExpansionGetCurrentSkinIndex();
			}

			UpdateContainerItems(itemIB);

			m_HealthLevel = itemIB.GetHealthLevel();

			if (itemIB.GetQuantity() > 0 && itemIB.IsBloodContainer())
			{
				m_IsBloodContainer = true;
				BloodContainerBase blood_container = BloodContainerBase.Cast(itemIB);
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
			auto settings = GetExpansionSettings().GetHardline();
			if (settings.EnableItemRarity)
			{
				ExpansionHardlineItemRarity rarity = itemIB.Expansion_GetRarity();
				if (rarity > ExpansionHardlineItemRarity.NONE)
					m_Rarity = rarity;
			}
		#endif
		}

		if (item.IsWeapon())
			m_IsWeapon = true;

		if (item.GetInventory().IsAttachment())
		{
			if (!item.GetHierarchyParent().IsInherited(SurvivorBase))
				m_IsAttached = true;
		}

		if (item.IsInherited(MagazineStorage))
			m_IsMagazine = true;
	}
	
	protected void GetAmmoForMagazine(EntityAI entity)
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
				ammoItem.SetDataFromItem(ammo);				
				ammoItem.SetExcluded(true);

			#ifdef EXPANSIONMODHARDLINE
				auto settings = GetExpansionSettings().GetHardline();
				if (settings.EnableItemRarity)
				{
					ExpansionHardlineItemRarity rarity = ammo.Expansion_GetRarity();
					string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
					Print(ToString() + "::GetAmmoForMagazine - Got rarity for ammo entry: " + ammoClassName + " | Rarity: " + rarityName);
					if (rarity > ExpansionHardlineItemRarity.NONE)
					{
						Print(ToString() + "::GetAmmoForMagazine - Set rarity for ammo entry: " + ammoClassName + " | Rarity: " + rarityName);
						ammoItem.SetRarity(rarity);
					}
				}
			#endif

				m_ContainerItems.Insert(ammoItem);
				m_ContainerItemsCount++;
			}
		}
	}

	void UpdateContainerItems(EntityAI item)
	{
		array<EntityAI> items = new array<EntityAI>;
		item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		if (!m_ContainerItems)
			m_ContainerItems = new array<ref ExpansionPersonalStorageContainerItem>;
		else
			m_ContainerItems.Clear();
		
		//! If the main item entity is a maganzine we want to get the ammo data and create a container entry for it.
		if (item.IsInherited(MagazineStorage))
			GetAmmoForMagazine(item);

		for (int i = 0; i < items.Count(); i++)
		{
			EntityAI containerItem = items[i];
			if (containerItem == item)
				continue;
			
			if (containerItem.GetInventory().IsAttachment() && m_Item.GetInventory().HasAttachment(containerItem) || containerItem.GetInventory().IsInCargo() && m_Item.GetInventory().HasEntityInCargo(containerItem))
			{
				ExpansionPersonalStorageContainerItem containerItemEntry = new ExpansionPersonalStorageContainerItem();
				containerItemEntry.SetDataFromItem(containerItem);
				m_ContainerItems.Insert(containerItemEntry);
				m_ContainerItemsCount++;
			}
		}
	}

	string GetClassName()
	{
		return m_ClassName;
	}

	void SetClassName(string name)
	{
		m_ClassName = name;
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

	string GetSkinName()
	{
		return m_SkinName;
	}

	int GetSkinIndex()
	{
		return m_SkinIndex;
	}

	void SetHealthLevel(int healthLevel)
	{
		m_HealthLevel = healthLevel;
	}

	int GetHealthLevel()
	{
		return m_HealthLevel;
	}

	void SetLiquidType(int liquidType)
	{
		m_LiquidType = liquidType;
	}

	int GetLiquidType()
	{
		return m_LiquidType;
	}

	void SetLiquidIsBloodContainer(bool state)
	{
		m_IsBloodContainer = state;
	}

	bool IsBloodContainer()
	{
		return m_IsBloodContainer;
	}

	void SetFoodStageType(int foodStage)
	{
		m_FoodStageType = foodStage;
	}

	int GetFoodStageType()
	{
		return m_FoodStageType;
	}

	bool IsAttached()
	{
		return m_IsAttached;
	}

	bool IsMagazine()
	{
		return m_IsMagazine;
	}

	bool IsWeapon()
	{
		return m_IsWeapon;
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
		return m_Item;
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
	
	void SetExcluded(bool state)
	{
		m_IsExcluded = state;
	}
	
	bool IsExcluded()
	{
		return m_IsExcluded;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_ClassName);
		ctx.Write(m_Quantity);
		ctx.Write(m_SkinName);
		ctx.Write(m_SkinIndex);
		ctx.Write(m_QuantityType);
		ctx.Write(m_HealthLevel);
		ctx.Write(m_LiquidType);
		ctx.Write(m_IsBloodContainer);
		ctx.Write(m_FoodStageType);
		ctx.Write(m_IsWeapon);
		ctx.Write(m_IsMagazine);
		ctx.Write(m_IsAttached);
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(m_Rarity);
	#endif
		ctx.Write(m_ContainerItemsCount);
		int containerItemsCount = m_ContainerItems.Count();
		ctx.Write(containerItemsCount);

		for (int i = 0; i < containerItemsCount; i++)
		{
			ExpansionPersonalStorageContainerItem containerItem = m_ContainerItems[i];
			containerItem.OnSend(ctx);
		}
		
		ctx.Write(m_IsExcluded);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_ClassName))
			return false;

		if (!ctx.Read(m_Quantity))
			return false;

		if (!ctx.Read(m_SkinName))
			return false;

		if (!ctx.Read(m_SkinIndex))
			return false;

		if (!ctx.Read(m_QuantityType))
			return false;

		if (!ctx.Read(m_HealthLevel))
			return false;

		if (!ctx.Read(m_LiquidType))
			return false;

		if (!ctx.Read(m_IsBloodContainer))
			return false;

		if (!ctx.Read(m_FoodStageType))
			return false;

		if (!ctx.Read(m_IsWeapon))
			return false;

		if (!ctx.Read(m_IsMagazine))
			return false;

		if (!ctx.Read(m_IsAttached))
			return false;

	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(m_Rarity))
			return false;
	#endif
		if (!ctx.Read(m_ContainerItemsCount))
			return false;

		int containerItemsCount;
		if (!ctx.Read(containerItemsCount))
			return false;

		if (!m_ContainerItems)
			m_ContainerItems = new array<ref ExpansionPersonalStorageContainerItem>;
		else
			m_ContainerItems.Clear();

		for (int i = 0; i < containerItemsCount; i++)
		{
			ExpansionPersonalStorageContainerItem containerItem = new ExpansionPersonalStorageContainerItem();
			if (!containerItem.OnRecieve(ctx))
				return false;

			m_ContainerItems.Insert(containerItem);
		}
		
		if (!ctx.Read(m_IsExcluded))
			return false;

		return true;
	}
};*/