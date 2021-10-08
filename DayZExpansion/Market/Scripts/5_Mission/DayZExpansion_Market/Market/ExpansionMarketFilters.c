/**
 * ExpansionMarketFilters.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionMarketAttachType
{
	OPTIC,
	WRAP,
	SWITCH,
	BUTTSTOCK,
	HANDGUARD,
	MAGAZINE,
	SUPPRESSOR,
	BAYONET,
	RAIL,
	LIGHT,
	BULLET,
	POUCHES,
	PATCHES,
	HOLSTERS,
	CANTEENS,
	GRENADES,
	OTHER,
}

class ExpansionMarketFilters
{
	protected ref map<string, ref TStringArray> m_AttachmentsMap;
	protected ref ExpansionMarketOutputs m_MarketOutputs;
	protected ref ExpansionMarketModule m_MarketModule;
	// ------------------------------------------------------------
	// ExpansionMarketFilters Constructor
	// ------------------------------------------------------------		
	void ExpansionMarketFilters()
	{
		if (!m_MarketModule)
			m_MarketModule = ExpansionMarketModule.Cast(GetModuleManager().GetModule(ExpansionMarketModule));

		if (!m_AttachmentsMap)
		{
			m_AttachmentsMap = new map<string, ref TStringArray>;
			GenerateAttachmentsMap();
		}
		
		if (!m_MarketOutputs)
		{
			m_MarketOutputs = new ExpansionMarketOutputs;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters Destructor
	// ------------------------------------------------------------	
	void ~ExpansionMarketFilters()
	{
		delete m_MarketOutputs;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetPrimaryWeaponMagazines
	// ------------------------------------------------------------
	TStringArray GetPrimaryWeaponMagazines()
	{
		return GetWeaponMagazines(GetGame().GetPlayer().FindAttachmentBySlotName("Shoulder"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetSecondaryWeaponMagazines
	// ------------------------------------------------------------
	TStringArray GetSecondaryWeaponMagazines()
	{
		return GetWeaponMagazines(GetGame().GetPlayer().FindAttachmentBySlotName("Melee"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetHandWeaponMagazines
	// ------------------------------------------------------------
	TStringArray GetHandWeaponMagazines()
	{
		return GetWeaponMagazines(GetGame().GetPlayer().GetHumanInventory().GetEntityInHands());
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetPrimaryWeaponBullets
	// ------------------------------------------------------------
	TStringArray GetPrimaryWeaponBullets()
	{
		return GetWeaponBullets(GetGame().GetPlayer().FindAttachmentBySlotName("Shoulder"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetSecondaryWeaponBullets
	// ------------------------------------------------------------
	TStringArray GetSecondaryWeaponBullets()
	{
		return GetWeaponBullets(GetGame().GetPlayer().FindAttachmentBySlotName("Melee"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetHandWeaponBullets
	// ------------------------------------------------------------	
	TStringArray GetHandWeaponBullets()
	{
		return GetWeaponBullets(GetGame().GetPlayer().GetHumanInventory().GetEntityInHands());
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetPrimaryWeaponAttachments
	// ------------------------------------------------------------
	TStringArray GetPrimaryWeaponAttachments()
	{
		return GetWeaponAttachments(GetGame().GetPlayer().FindAttachmentBySlotName("Shoulder"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetSecondaryWeaponAttachments
	// ------------------------------------------------------------
	TStringArray GetSecondaryWeaponAttachments()
	{
		return GetWeaponAttachments(GetGame().GetPlayer().FindAttachmentBySlotName("Melee"));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetHandWeaponAttachments
	// ------------------------------------------------------------
	TStringArray GetHandWeaponAttachments()
	{
		return GetWeaponAttachments(GetGame().GetPlayer().GetHumanInventory().GetEntityInHands());
	}
	
	ExpansionMarketWeapon GetMarketWeapon(EntityAI item)
	{
		if (!item || !item.IsWeapon())
			return NULL;
		
		return m_MarketOutputs.Weapons.Get(item.GetType());
	}
	
	TStringArray GetWeaponMagazines(EntityAI item)
	{
		ExpansionMarketWeapon weapon = GetMarketWeapon(item);
		if (weapon)
			return FilterItems(weapon.magazines);
		
		return {};
	}
	
	TStringArray GetWeaponBullets(EntityAI item)
	{
		ExpansionMarketWeapon weapon = GetMarketWeapon(item);
		TStringArray bullets = new TStringArray;

		if (weapon)
		{
			bullets.Copy(weapon.bullets);

			foreach (string bullet : bullets)
			{
				string boxName = GetExpansionSettings().GetMarket().GetAmmoBoxWithAmmoName(bullet);
				bullets.Insert(boxName);
			}
		}

		return FilterItems(bullets);
	}
	
	TStringArray GetWeaponAttachments(EntityAI item)
	{
		ExpansionMarketWeapon weapon = GetMarketWeapon(item);
		TStringArray attachments = new TStringArray;

		if (weapon)
		{
			attachments.InsertAll(weapon.attachments);
			attachments.InsertAll(weapon.optics);
			attachments.InsertAll(weapon.suppressors);
			attachments.InsertAll(weapon.ghillies);
			attachments.InsertAll(weapon.switchables);
			attachments.InsertAll(weapon.buttstocks);
			attachments.InsertAll(weapon.handguards);
			attachments.InsertAll(weapon.bayonets);
			attachments.InsertAll(weapon.rails);
			attachments.InsertAll(weapon.lights);
		}
		
		return FilterItems(attachments);
	}
	
	TStringArray FilterItems(TStringArray candidates)
	{
		ExpansionMarketTrader trader = m_MarketModule.GetTrader().GetTraderMarket();
		TStringArray items = new TStringArray;

		foreach (string item: candidates)
		{
			if (trader.ItemExists(item) && !trader.IsAttachmentBuySell(item))
				items.Insert(item);
		}

		return items;
	}

	// ------------------------------------------------------------
	// ExpansionMarketFilters GenerateAttachmentsMap
	// ------------------------------------------------------------	
	void GenerateAttachmentsMap()
	{
		if (m_AttachmentsMap)
		{
			GenerateAttachmentsMapFromPath(m_AttachmentsMap, "CfgVehicles");
			GenerateAttachmentsMapFromPath(m_AttachmentsMap, "CfgMagazines");
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GenerateAttachmentsMap
	// ------------------------------------------------------------		
	void GenerateAttachmentsMapFromPath(out map<string, ref TStringArray> currentMap, string path)
	{
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) 
		{
			string item_name;
			GetGame().ConfigGetChildName(path, i, item_name);
			switch (GetGame().ConfigGetType(path + " " + item_name + " inventorySlot")) 
			{
				case CT_ARRAY: 
				{
					TStringArray inventory_slots = {};
					GetGame().ConfigGetTextArray(path + " " + item_name + " inventorySlot", inventory_slots);
					foreach (string inv_slot: inventory_slots) 
					{
						inv_slot.ToLower();
						if (!currentMap[inv_slot]) 
						{
							currentMap[inv_slot] = new TStringArray();
						}
						currentMap[inv_slot].Insert(item_name);
					}
					break;
				}
				case CT_STRING: 
				{
					string inventory_slot;
					GetGame().ConfigGetText(path + " " + item_name + " inventorySlot", inventory_slot);
					inventory_slot.ToLower();
					if (!currentMap[inventory_slot]) 
					{
						currentMap[inventory_slot] = new TStringArray();
					}
					currentMap[inventory_slot].Insert(item_name);
					break;
				}
			}	
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GenerateWeaponAttachmentDenom
	// ------------------------------------------------------------	
	void GenerateWeaponAttachmentDenom(string className)
	{
		if (!m_MarketOutputs.Weapons)
			m_MarketOutputs.Weapons = new map<string, ref ExpansionMarketWeapon>;

		if (IsWeapon(className) && !m_MarketOutputs.Weapons.Contains(className))
		{
			ExpansionMarketWeapon weapon = new ExpansionMarketWeapon();
			weapon.name = className;
			GetGame().ConfigGetTextArray("CfgWeapons " + className + " chamberableFrom", weapon.bullets);
			GetGame().ConfigGetTextArray("CfgWeapons " + className + " magazines", weapon.magazines);
			
			AddAttachmentDenom(weapon, "CfgWeapons");

			m_MarketOutputs.Weapons.Insert(className, weapon);
		}
	}
			
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetSellables
	// ------------------------------------------------------------
	TStringArray GetSellables()
	{
		TStringArray sellables = new TStringArray;

		if (m_MarketModule)
		{
			ExpansionTraderObjectBase trader = m_MarketModule.GetTrader();
			if (trader)
			{
				array<EntityAI> items = m_MarketModule.LocalGetEntityInventory();

				foreach (EntityAI item : items) 
				{
					string type = item.GetType();
					type.ToLower();

					if (trader.GetTraderMarket().CanSellItem(type) && sellables.Find(type) == -1)
						sellables.Insert(type);
				}
			}
		}
		
		return sellables;
	}

	// ------------------------------------------------------------
	// ExpansionMarketFilters GetPurchasables
	// ------------------------------------------------------------
	TStringArray GetPurchasables()
	{
		TStringArray purchasables = new TStringArray;
		
		if (m_MarketModule)
		{
			ExpansionTraderObjectBase trader = m_MarketModule.GetTrader();
			if (trader)
			{
				ExpansionMarketCurrency worth = m_MarketModule.GetPlayerWorth();
				TStringArray items = trader.GetTraderMarket().Items.GetKeyArray();
				foreach (string itemName : items) 
				{
					if (!trader.GetTraderMarket().CanBuyItem(itemName))
						continue;

					int itemStock = m_MarketModule.GetClientZone().GetStock(itemName, true);
					if (itemStock < 1)
						continue;

					ExpansionMarketItem marketItem = GetExpansionSettings().GetMarket().GetItem(itemName);
					if (!marketItem)
						continue;

					ExpansionMarketCurrency price = 0;  //! Important: Make sure it's zero before calling FindPriceOfPurchase
					if (m_MarketModule.FindPriceOfPurchase(marketItem, m_MarketModule.GetClientZone(), trader.GetTraderMarket(), 1, price, false) && worth >= price)
						purchasables.Insert(itemName);
				}
			}
		}
		
		return purchasables;
	}
	
	TStringArray GetAttachmentsByType(ExpansionMarketOutput output, ExpansionMarketAttachType attachType)
	{
		if (output.IsInherited(ExpansionMarketVest))
			return GetVestAttachmentsByType(ExpansionMarketVest.Cast(output), attachType);
		else
			return GetWeaponAttachmentsByType(ExpansionMarketWeapon.Cast(output), attachType);
	}

	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponAttachmentsByType
	// ------------------------------------------------------------	
	TStringArray GetWeaponAttachmentsByType(ExpansionMarketWeapon weapon, ExpansionMarketAttachType attachType)
	{
		switch (attachType)
		{
			case ExpansionMarketAttachType.BULLET:
				return weapon.bullets;
			case ExpansionMarketAttachType.MAGAZINE:
				return weapon.magazines;
			case ExpansionMarketAttachType.OPTIC:
				return weapon.optics;
			case ExpansionMarketAttachType.SUPPRESSOR:
				return weapon.suppressors;
			case ExpansionMarketAttachType.WRAP:
				return weapon.ghillies;
			case ExpansionMarketAttachType.SWITCH:
				return weapon.switchables;
			case ExpansionMarketAttachType.BUTTSTOCK:
				return weapon.buttstocks;
			case ExpansionMarketAttachType.HANDGUARD:
				return weapon.handguards;
			case ExpansionMarketAttachType.BAYONET:
				return weapon.bayonets;
			case ExpansionMarketAttachType.RAIL:
				return weapon.rails;
			case ExpansionMarketAttachType.LIGHT:
				return weapon.lights;
		}

		//! ExpansionMarketAttachType.OTHER
		return weapon.attachments;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetAttachmentsByClassNameAndTypes
	// ------------------------------------------------------------	
	TStringArray GetAttachmentsByClassNameAndTypes(string className, array<ExpansionMarketAttachType> attachTypes)
	{
		TStringArray attachments = new TStringArray;

		if (IsVest(className))
		{
			GetAttachmentsByClassNameAndTypesEx(m_MarketOutputs.Vests.Get(className), attachTypes, attachments);
		}
		else
		{
			GetAttachmentsByClassNameAndTypesEx(m_MarketOutputs.Weapons.Get(className), attachTypes, attachments);
		}

		return attachments;
	}
	
	void GetAttachmentsByClassNameAndTypesEx(ExpansionMarketOutput output, array<ExpansionMarketAttachType> attachTypes, TStringArray attachments)
	{
		if (output)
		{
			ExpansionMarketTrader trader = m_MarketModule.GetTrader().GetTraderMarket();
			ExpansionMarketTraderZone zone = m_MarketModule.GetClientZone();
			foreach (ExpansionMarketAttachType attachType: attachTypes)
			{
				TStringArray candidates = GetAttachmentsByType(output, attachType);
				foreach (string name: candidates)
				{
					if (trader.ItemExists(name) && trader.CanBuyItem(name))
					{
						int stock = zone.GetStock(name);
						if (stock > 0)
							attachments.Insert(name);
						else if (stock == -3)
							EXPrint(ToString() + "::GetAttachmentsByClassNameAndTypesEx - " + name + " does not exist in trader zone!");
					}
				}
			}
		}
	}
	
	static bool IsVest(string className)
	{
		return GetGame().IsKindOf(className, "Clothing_Base") && ClassNameHierarchyContains(className, "CfgVehicles", {"vest", "chestrig"}, "Clothing_Base");
	}
	
	static bool IsWeapon(string className)
	{
		return GetGame().ConfigIsExisting("CfgWeapons " + className);
	}

	//! Fucking SLOW. Use with care.
	static bool ClassNameHierarchyContains(string className, string cfgPath, TStringArray validBaseNames, string stopAt = "Inventory_Base")
	{
		string baseName = className;
		string baseNameLower;

		//! Traverse max 10 levels up the inheritance tree
		for (int i = 0; i < 10; i++)
		{
			baseNameLower = baseName;
			baseNameLower.ToLower();

			foreach (string validBase: validBaseNames)
			{
				if (baseNameLower.Contains(validBase))
					return true;
			}

			GetGame().ConfigGetBaseName(cfgPath + " " + className, baseName);

			if (baseName == "All" || baseName == stopAt)
				return false;

			className = baseName;
		}

		return false;
	}
	
	//! Fucking SLOW. Use with care. Not used atm.
	static string GetRootClassName(string className, string cfgPath, string stopAt = "Inventory_Base")
	{
		string baseName;

		//! Traverse max 10 levels up the inheritance tree
		for (int i = 0; i < 10; i++)
		{
			GetGame().ConfigGetBaseName(cfgPath + " " + className, baseName);

			if (baseName == "All" || baseName == stopAt)
				break;

			className = baseName;
		}

		return className;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponMagazinesByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponMagazinesByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.MAGAZINE});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponOpticsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponOpticsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.OPTIC});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponSuppressorsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponSuppressorsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.SUPPRESSOR});
	}

	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponGhilliesByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponGhilliesByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.WRAP});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponSwitchablesByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponSwitchablesByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.SWITCH});
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponAttachmentsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponAttachmentsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.OTHER});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponButtstocksByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponButtstocksByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.BUTTSTOCK});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponHandguardsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponHandguardsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.HANDGUARD});
	}
	
		// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponBayonetsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponBayonetsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.BAYONET});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponRailsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponRailsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.RAIL});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponLightsByClassName
	// ------------------------------------------------------------
	TStringArray GetWeaponLightsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.LIGHT});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetWeaponAttachmentAttachType
	// ------------------------------------------------------------	
	static ExpansionMarketAttachType GetWeaponAttachmentAttachType(string className, string slotName = "")
	{
		string baseClassName;
		string baseNameToLower;
		string classNameToLower = className;
		classNameToLower.ToLower();
		string slotNameToLower = slotName;
		slotNameToLower.ToLower();
		if (GetGame().ConfigIsExisting("CfgVehicles " + className))
			GetGame().ConfigGetBaseName("CfgVehicles " + className, baseClassName);
		else if (GetGame().ConfigIsExisting("CfgMagazines " + className))
			GetGame().ConfigGetBaseName("CfgMagazines " + className, baseClassName);
		baseNameToLower = baseClassName;
		baseNameToLower.ToLower();
		
		switch (baseClassName)	
		{
			case "ItemOptics":
			{
				return ExpansionMarketAttachType.OPTIC;
			}
			case "ItemSuppressor":
			case "SuppressorBase":
			{
				return ExpansionMarketAttachType.SUPPRESSOR;
			}
			case "GhillieAtt_ColorBase":
			{
				return ExpansionMarketAttachType.WRAP;
			}
			case "Switchable_Base":
			{
				return ExpansionMarketAttachType.SWITCH;
			}
			case "Magazine_Base":
			{
				return ExpansionMarketAttachType.MAGAZINE;
			}
		}
		
		if (slotNameToLower.Contains("buttstock") || baseNameToLower.Contains("buttstock") || classNameToLower.Contains("buttstock"))
		{
			return ExpansionMarketAttachType.BUTTSTOCK;
		}
		else if (slotNameToLower.Contains("handguard") || baseNameToLower.Contains("handguard") || classNameToLower.Contains("handguard"))
		{
			return ExpansionMarketAttachType.HANDGUARD;
		}
		else if (slotNameToLower.Contains("bayonet") || baseNameToLower.Contains("bayonet") || classNameToLower.Contains("bayonet"))
		{
			return ExpansionMarketAttachType.BAYONET;
		}
		else if (slotNameToLower.Contains("suppressor") || baseNameToLower.Contains("suppressor") || classNameToLower.Contains("suppressor"))
		{
			return ExpansionMarketAttachType.SUPPRESSOR;
		}
		else if (slotNameToLower.Contains("optic") || baseNameToLower.Contains("optic") || classNameToLower.Contains("optic"))
		{
			return ExpansionMarketAttachType.OPTIC;
		}
		else if (slotNameToLower.Contains("wrap") || baseNameToLower.Contains("wrap") || classNameToLower.Contains("wrap"))
		{
			return ExpansionMarketAttachType.WRAP;
		}
		else if (slotNameToLower.Contains("magazine") || baseNameToLower.Contains("magazine") || classNameToLower.Contains("magazine"))
		{
			return ExpansionMarketAttachType.MAGAZINE;
		}
		else if (slotNameToLower.Contains("rail") || baseNameToLower.Contains("rail") || classNameToLower.Contains("rail"))
		{
			return ExpansionMarketAttachType.RAIL;
		}
		else if (slotNameToLower.Contains("light") || baseNameToLower.Contains("light") || classNameToLower.Contains("light"))
		{
			return ExpansionMarketAttachType.LIGHT;
		}
		
		//Print(slotNameToLower);
		//Print(baseNameToLower);
		//Print(classNameToLower);
		return ExpansionMarketAttachType.OTHER;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters HasAttachments
	// ------------------------------------------------------------	
	bool HasWeaponAttachments(string className)
	{
		if (GetWeaponButtstocksByClassName(className).Count() > 0 ) return true;
		if (GetWeaponHandguardsByClassName(className).Count() > 0 ) return true;
		if (GetWeaponAttachmentsByClassName(className).Count() > 0)  return true;
		if (GetWeaponSwitchablesByClassName(className).Count() > 0 ) return true;
		if (GetWeaponGhilliesByClassName(className).Count() > 0) return true;
		if (GetWeaponSuppressorsByClassName(className).Count() > 0) return true;
		if (GetWeaponOpticsByClassName(className).Count() > 0) return true;
		if (GetWeaponMagazinesByClassName(className).Count() > 0) return true;
		if (GetWeaponRailsByClassName(className).Count() > 0) return true;
		if (GetWeaponBayonetsByClassName(className).Count() > 0) return true;
		if (GetWeaponLightsByClassName(className).Count() > 0) return true;
		
		 return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GenerateVestAttachmentDenom
	// ------------------------------------------------------------	
	void GenerateVestAttachmentDenom(string className)
	{
		if (!m_MarketOutputs.Vests)
			m_MarketOutputs.Vests = new map<string, ref ExpansionMarketVest>;	
		
		TStringArray inventory_slots = {};
		string inventory_slot;
		
		if (IsVest(className) && !m_MarketOutputs.Vests.Contains(className))
		{
			switch (GetGame().ConfigGetType("CfgVehicles " + className + " inventorySlot"))
			{
				case CT_ARRAY: 
				{
					GetGame().ConfigGetTextArray("CfgVehicles " + className + " inventorySlot", inventory_slots);
					break;
				}
				case CT_STRING: 
				{
					GetGame().ConfigGetText("CfgVehicles " + className + " inventorySlot", inventory_slot);
					break;
				}
			}

			if (inventory_slots.Count() > 0 || inventory_slot != string.Empty)
			{
				ExpansionMarketVest vest = new ExpansionMarketVest();
				vest.name = className;
				
				AddAttachmentDenom(vest, "CfgVehicles");

				m_MarketOutputs.Vests.Insert(className, vest);
			}
		}
	}
	
	void AddAttachmentDenom(ExpansionMarketOutput output, string cfgPath)
	{
		TStringArray attachmentSlotNames = {};
		GetGame().ConfigGetTextArray(cfgPath + " " + output.name + " attachments", attachmentSlotNames);

		if (m_AttachmentsMap.Count() > 0)
		{
			for (int l = 0; l < attachmentSlotNames.Count(); l++) 
			{
				string attachmentSlotName = attachmentSlotNames[l];
				attachmentSlotName.ToLower();
				
				if (m_AttachmentsMap[attachmentSlotName])
				{
					foreach (string attachment: m_AttachmentsMap[attachmentSlotName]) 
					{
						ExpansionMarketAttachType attachType;
						if (output.IsInherited(ExpansionMarketVest))
							attachType = GetVestAttachmentAttachType(attachment, attachmentSlotName);
						else
							attachType = GetWeaponAttachmentAttachType(attachment, attachmentSlotName);
						TStringArray attachments = GetAttachmentsByType(output, attachType);
						attachments.Insert(attachment);
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestAttachmentAttachType
	// ------------------------------------------------------------	
	static ExpansionMarketAttachType GetVestAttachmentAttachType(string className, string slotName = "")
	{
		string baseClassName;
		string baseNameToLower;
		string classNameToLower = className;
		classNameToLower.ToLower();
		GetGame().ConfigGetBaseName("CfgVehicles " + className, baseClassName);
		baseNameToLower = baseClassName;
		baseNameToLower.ToLower();
		string slotNameToLower = slotName;
		slotNameToLower.ToLower();
				
		switch (baseClassName)	
		{
			case "Grenade_Base":
			{
				return ExpansionMarketAttachType.GRENADES;
			}
		}
		
		if (slotNameToLower.Contains("pouch") || baseNameToLower.Contains("pouch") || classNameToLower.Contains("pouch"))
		{
			return ExpansionMarketAttachType.POUCHES;
		}
		else if (slotNameToLower.Contains("holster") || baseNameToLower.Contains("holster") || classNameToLower.Contains("holster"))
		{
			return ExpansionMarketAttachType.HOLSTERS;
		}
		else if (slotNameToLower.Contains("canteen") || baseNameToLower.Contains("canteen") || classNameToLower.Contains("canteen"))
		{
			return ExpansionMarketAttachType.CANTEENS;
		}
		else if (slotNameToLower.Contains("grenade") || baseNameToLower.Contains("grenade") || classNameToLower.Contains("grenade"))
		{
			return ExpansionMarketAttachType.GRENADES;
		}
		else if (slotNameToLower.Contains("patch") || baseNameToLower.Contains("patch") || classNameToLower.Contains("patch"))
		{
			return ExpansionMarketAttachType.PATCHES;
		}

		//Print(slotNameToLower);
		//Print(baseNameToLower);
		//Print(classNameToLower);
		return ExpansionMarketAttachType.OTHER;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters HasAttachments
	// ------------------------------------------------------------		
	bool HasVestAttachments(string className)
	{
		if (GetVestPouchesByClassName(className).Count() > 0 ) return true;
		if (GetVestHolstersByClassName(className).Count() > 0 ) return true;
		if (GetVestCanteensByClassName(className).Count() > 0)  return true;
		if (GetVestGrenadesByClassName(className).Count() > 0)  return true;
		if (GetVestPatchesByClassName(className).Count() > 0)  return true;
		if (GetVestAttachmentsByClassName(className).Count() > 0)  return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestAttachmentsByType
	// ------------------------------------------------------------	
	TStringArray GetVestAttachmentsByType(ExpansionMarketVest vest, ExpansionMarketAttachType attachType)
	{
		switch (attachType)
		{
			case ExpansionMarketAttachType.POUCHES:
				return vest.pouches;
			case ExpansionMarketAttachType.HOLSTERS:
				return vest.holsters;
			case ExpansionMarketAttachType.CANTEENS:
				return vest.canteens;
			case ExpansionMarketAttachType.GRENADES:
				return vest.grenades;
			case ExpansionMarketAttachType.PATCHES:
				return vest.patches;
		}
		
		//! ExpansionMarketAttachType.OTHER
		return vest.attachments;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestAttachmentsByClassName
	// ------------------------------------------------------------
	TStringArray GetVestAttachmentsByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.OTHER});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestPouchesByClassName
	// ------------------------------------------------------------
	TStringArray GetVestPouchesByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.POUCHES});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestHolstersByClassName
	// ------------------------------------------------------------
	TStringArray GetVestHolstersByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.HOLSTERS});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestCanteensByClassName
	// ------------------------------------------------------------
	TStringArray GetVestCanteensByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.CANTEENS});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestGrenadesByClassName
	// ------------------------------------------------------------
	TStringArray GetVestGrenadesByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.GRENADES});
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketFilters GetVestPatchesByClassName
	// ------------------------------------------------------------
	TStringArray GetVestPatchesByClassName(string className)
	{
		return GetAttachmentsByClassNameAndTypes(className, {ExpansionMarketAttachType.PATCHES});
	}
}