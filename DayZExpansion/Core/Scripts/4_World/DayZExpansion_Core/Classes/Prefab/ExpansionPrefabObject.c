class ExpansionPrefabObject : Managed
{
	// Config class name of the item
	string ClassName = "";

	string Include;

	float Chance = 1.0;

	// Number of items in the stack
	autoptr ExpansionMinMax Quantity = new ExpansionMinMax(0, 0);

	// Health of item per damage zone -> if empty string then it is global health
	autoptr array<ref ExpansionHealth> Health = new array<ref ExpansionHealth>;

	// Inventory attachments -> if taken, won't spawn self and contents
	autoptr array<ref ExpansionPrefabSlot> InventoryAttachments = new array<ref ExpansionPrefabSlot>();

	// Inventory Cargo -> if no free spot in cargo, won't spawn self and contents
	autoptr array<ref ExpansionPrefabObject> InventoryCargo = new array<ref ExpansionPrefabObject>();

	// Construction parts built -> only works if object is basebuilding
	autoptr array<string> ConstructionPartsBuilt = new array<string>();

	// Sets of prefab objects (for making chance apply to whole set)
	autoptr array<ref ExpansionPrefabObject> Sets = new array<ref ExpansionPrefabObject>();

	[NonSerialized()]
	string m_Name;

	ExpansionPrefabObject BeginAttachment(string className, string slotName = "")
	{
		ExpansionPrefabObject object = new ExpansionPrefabObject();
		object.ClassName = className;

		ExpansionPrefab.s_Begin.Insert(this);

		array<ref ExpansionPrefabObject> attachments;
		if (!FindAttachments(slotName, attachments))
		{
			attachments = new array<ref ExpansionPrefabObject>();
			InventoryAttachments.Insert(new ExpansionPrefabSlot(slotName, attachments));
		}

		attachments.Insert(object);
		return object;
	}

	bool FindAttachments(string slotName, out array<ref ExpansionPrefabObject> items)
	{
		foreach (auto slot: InventoryAttachments)
		{
			if (slot.SlotName == slotName)
			{
				items = slot.Items;
				return true;
			}
		}

		return false;
	}

	void InsertAttachments(string slotName, array<ref ExpansionPrefabObject> items)
	{
		array<ref ExpansionPrefabObject> existingItems;
		if (FindAttachments(slotName, existingItems))
		{
			foreach (auto object: items)
			{
				existingItems.Insert(object);
			}
		}
		else
		{
			InventoryAttachments.Insert(new ExpansionPrefabSlot(slotName, items));
		}
	}

	ExpansionPrefabObject BeginCargo(string className)
	{
		ExpansionPrefabObject object = new ExpansionPrefabObject();
		object.ClassName = className;

		ExpansionPrefab.s_Begin.Insert(this);

		InventoryCargo.Insert(object);
		return object;
	}

	ExpansionPrefabObject BeginSet(string setName = "")
	{
		ExpansionPrefabObject object = new ExpansionPrefabObject();
		object.ClassName = setName;

		ExpansionPrefab.s_Begin.Insert(this);

		Sets.Insert(object);
		return object;
	}

	ExpansionPrefabObject End()
	{
		int index = ExpansionPrefab.s_Begin.Count() - 1;

		ExpansionPrefabObject object = ExpansionPrefab.s_Begin[index];

		ExpansionPrefab.s_Begin.Remove(index);

		return object;
	}

	ExpansionPrefabObject SetQuantity(float quantityMin, float quantityMax = 0)
	{
		Quantity.Set(quantityMin, quantityMax);

		return this;
	}

	ExpansionPrefabObject SetHealth(string zone, float healthMin, float healthMax = 0.0)
	{
		ExpansionHealth health = new ExpansionHealth(healthMin, healthMax, zone);
		InsertHealth(health);

		return this;
	}

	ExpansionPrefabObject SetHealth(float healthMin, float healthMax = 0.0)
	{
		ExpansionHealth health = new ExpansionHealth(healthMin, healthMax, "");
		InsertHealth(health);

		return this;
	}

	bool FindHealth(string zone, out ExpansionHealth health)
	{
		foreach (auto existingHealth: Health)
		{
			if (existingHealth.Zone == zone)
			{
				health = existingHealth;
				return true;
			}
		}

		return false;
	}

	void InsertHealth(ExpansionHealth health)
	{
		ExpansionHealth existingHealth;
		if (FindHealth(health.Zone, existingHealth))
		{
			existingHealth.Set(health.Min, health.Max);
		}
		else
		{
			Health.Insert(health);
		}
	}

	ExpansionPrefabObject SetPartConstructed(string part)
	{
		ConstructionPartsBuilt.Insert(part);

		return this;
	}

	string GetPath()
	{
		CF_Log.Error("Not implemented.");
		return "";
	}

	void Save()
	{
		CF_Log.Error("Not implemented.");
	}

	bool CanSpawn()
	{
		if (!Chance || Chance < Math.RandomFloat(0.0, 1.0))
			return false;

		return true;
	}

	Object Spawn(Object self, array<ref ExpansionPrefabObject> inventoryCargo = null)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.LOADOUTS, this);
	#endif

		if (!self)
		{
			return self;
		}

		if (Include)
		{
			if (CF_String.EqualsIgnoreCase(Include, m_Name))
			{
				EXError.Error(this, string.Format("'%1' cannot include itself", m_Name), {});
			}
			else
			{
				string folder;

				ExpansionPrefab thisPrefab;
				if (Class.CastTo(thisPrefab, this))
					folder = ExpansionString.DirName(thisPrefab.m_Path);

				if (!folder)
					folder = EXPANSION_LOADOUT_FOLDER;

				ExpansionPrefab prefab = ExpansionPrefab.Load(Include, false, folder);
				if (prefab)
				{
					if (prefab.Include && CF_String.EqualsIgnoreCase(prefab.Include, m_Name))
						EXError.Error(this, string.Format("'%1' cannot include parent '%2'", prefab.m_Name, m_Name), {});
					else
						prefab.Spawn(self, inventoryCargo);
				}
			}
		}

		foreach (ExpansionHealth health : Health)
		{
			float maxHealth = self.GetMaxHealth(health.Zone, "");
			float healthModifier = Math.RandomFloatInclusive(health.Min, health.Max);
			self.SetHealth(health.Zone, "", maxHealth * healthModifier);
		}

		EntityAI entity;
		if (Class.CastTo(entity, self))
		{
			ExpansionVehicle vehicle;
			if (ExpansionVehicle.Get(vehicle, entity))
			{
				vehicle.FillFluids();
			}

			ItemBase item;
			Magazine mag;
			if (Class.CastTo(item, entity) && item.HasQuantity())
			{
				float quantity = Quantity.GetRandom();

				if (quantity > -1)
				{
					float quantityMin;
					float quantityMax;
					if (Class.CastTo(mag, entity))
					{
						quantityMax = mag.GetAmmoMax();
						if (mag.IsAmmoPile())
							quantityMin = 1.0 / quantityMax;  //! Make sure ammo pile has at least one bullet
					}
					else
					{
						quantityMin = item.GetQuantityMin();
						quantityMax = item.GetQuantityMax();
					}

					if (quantity == 0 || quantity < quantityMin)
					{
						if (mag)
							quantity = 1.0;
						else
							quantity = item.GetQuantityInit() / quantityMax;
					}

					if (quantity > 0)
					{
						float itemQuantity = quantityMax * quantity;
						if (item.Expansion_IsStackable() || mag)
							itemQuantity = Math.Round(itemQuantity);
						if (mag)
						{
							mag.ServerSetAmmoCount(itemQuantity);
						}
						else
						{
							item.SetQuantity(itemQuantity);
						}
					}
				}
			}

			BaseBuildingBase baseBuilding;
			if (Class.CastTo(baseBuilding, entity))
			{
				Construction construction = baseBuilding.GetConstruction();

				if (construction)
				{
					foreach (string partName: ConstructionPartsBuilt)
					{
						construction.ExpansionBuildPartFull(partName);
					}
				}
			}

			GameInventory inventory = entity.GetInventory();
			if (inventory)
			{
				HumanInventory humanInventory = HumanInventory.Cast(inventory);

			#ifdef EXPANSIONMODAI
				eAIBase ai = eAIBase.Cast(entity);
			#endif

				array<ref ExpansionPrefabObject> candidates();
				int index;

				//! Cargo from root of any set needs to be spawned last so that cargo containers (e.g. clothing) added via attachments are already there
				bool spawnCargo;

				array<ref ExpansionPrefabObject> inventoryCargoLocal;
				if (!inventoryCargo)
				{
					//! Cannot assign new array directly to inventoryCargo inside this conditional block,
					//! leads to nonsensical compile error ("Variable 'inventoryCargo' is not strong ref")
					inventoryCargoLocal = new array<ref ExpansionPrefabObject>;
					inventoryCargo = inventoryCargoLocal;
					spawnCargo = true;
				}

				foreach (ExpansionPrefabObject thisCargo: InventoryCargo)
				{
					if (thisCargo.ClassName && thisCargo.CanSpawn())
						inventoryCargo.Insert(thisCargo);
				}
				
				//! Select sets by chance
				foreach (ExpansionPrefabObject selectedSet : Sets)
				{
					if (selectedSet.CanSpawn())
					{
						candidates.Insert(selectedSet);
					}
				}

				//! Spawn sets in random order
				TStringArray setNames();  //! If set is named, will only spawn one per unique name
				while (candidates.Count())
				{
					index = candidates.GetRandomIndex();
					selectedSet = candidates[index];
					candidates.Remove(index);

					if (!selectedSet.ClassName || setNames.Find(selectedSet.ClassName) == -1)
					{
						selectedSet.Spawn(self, inventoryCargo);
						if (selectedSet.ClassName)
						{
							setNames.Insert(selectedSet.ClassName);
						}
					}
				}

				Object child;
				int fallbackSlotId = InventorySlots.INVALID;
				map<int, bool> slotTaken = new map<int, bool>;

				foreach (auto slot : InventoryAttachments)
				{
					string slotName = slot.SlotName;
					array<ref ExpansionPrefabObject> attachments = slot.Items;

					int slotId = InventorySlots.GetSlotIdFromString(slotName);
					int currentSlotId;

					child = null;

					candidates.Clear();

					//! Select attachments by chance
					foreach (ExpansionPrefabObject attachment : attachments)
					{
						if (attachment.ClassName && attachment.CanSpawn())
						{
							candidates.Insert(attachment);
						#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
							EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - selected candidate attachment " + attachment.ClassName + " for " + entity);
						#endif
						}
					#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
						else
						{
							EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - discarded candidate attachment " + attachment.ClassName + " for " + entity);
						}
					#endif
					}

					//! Spawn candidate attachments in random order. If no valid inventory slot given, spawn all candidates.
					while (candidates.Count())
					{
						index = candidates.GetRandomIndex();
						attachment = candidates[index];
						candidates.Remove(index);

						currentSlotId = slotId;
						if (slotId == InventorySlots.INVALID && entity.IsInherited(Weapon_Base))
						{
							if (g_Game.IsKindOf(attachment.ClassName, "Magazine_Base") && !g_Game.IsKindOf(attachment.ClassName, "Ammunition_Base"))
								currentSlotId = InventorySlots.MAGAZINE;
						}
					#ifdef EXPANSIONMODAI
						else if (ai)
						{
							//! Prefer hand slot for AI weapon to mitigate client sync issues
							if ((slotId == InventorySlots.SHOULDER || slotId == InventorySlots.MELEE) && !slotTaken[InventorySlots.HANDS] && g_Game.ConfigIsExisting("CfgWeapons " + attachment.ClassName))
							{
								currentSlotId = InventorySlots.HANDS;
								fallbackSlotId = slotId;
							}
							else if (slotId == InventorySlots.HANDS && fallbackSlotId != InventorySlots.INVALID && slotTaken[InventorySlots.HANDS])
							{
								currentSlotId = fallbackSlotId;
								fallbackSlotId = InventorySlots.INVALID;
							}
						}
					#endif

						if (currentSlotId != InventorySlots.INVALID && slotTaken[currentSlotId])
						{
							continue;
						}

						switch (currentSlotId)
						{
						case InventorySlots.INVALID:
							child = attachment.Spawn(inventory.CreateAttachment(attachment.ClassName));
						#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
							if (child)
								EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - created attachment " + attachment.ClassName + " on " + entity);
							else
								EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - couldn't create attachment " + attachment.ClassName + " on " + entity);
						#endif
							break;
						case InventorySlots.HANDS:
							if (humanInventory)
							{
								child = attachment.Spawn(humanInventory.CreateInHands(attachment.ClassName));
								if (child != null)
								{
									slotTaken[currentSlotId] = true;
								#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
									EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - created " + attachment.ClassName + " in hands of " + entity);
								#endif
								}
							#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
								else
								{
									EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - couldn't create " + attachment.ClassName + " in hands of " + entity);
								}
							#endif
							}
							break;
						case InventorySlots.MAGAZINE:
							child = attachment.Spawn(item.ExpansionCreateInInventory(attachment.ClassName));
							if (child != null)
							{
								slotTaken[currentSlotId] = true;
							#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
								EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - created mag " + attachment.ClassName + " on " + entity);
							#endif
							}
						#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
							else
							{
								EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - couldn't create mag " + attachment.ClassName + " on " + entity);
							}
						#endif
							break;
						default:
							child = attachment.Spawn(inventory.CreateAttachmentEx(attachment.ClassName, currentSlotId));
							if (child != null)
							{
								slotTaken[currentSlotId] = true;
							#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
								EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - created attachment " + attachment.ClassName + " on " + entity + " in slot " + slotName);
							#endif
							}
						#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
							else
							{
								EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - couldn't create attachment " + attachment.ClassName + " on " + entity + " in slot " + slotName);
							}
						#endif
							break;
						}
					}
				}

				if (!spawnCargo)
					return self;

				//! Spawn candidate cargo in random order
				ExpansionPrefabObject cargo;
				while (inventoryCargo.Count())
				{
					index = inventoryCargo.GetRandomIndex();
					cargo = inventoryCargo[index];
					inventoryCargo.Remove(index);

					child = cargo.Spawn(ExpansionItemSpawnHelper.CreateInInventoryEx(entity, cargo.ClassName));
				#ifdef EXPANSION_LOADOUTS_DEBUGSPAWN
					if (child)
						EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - created " + cargo.ClassName + " on " + entity);
					else
						EXTrace.Print(EXTrace.LOADOUTS, ExpansionPrefabObject, "::Spawn - couldn't create " + cargo.ClassName + " on " + entity);
				#endif
				}
			}
		}

		return self;
	}

	void FillCar(Car car, CarFluid fluid)
	{
		EXError.WarnOnce(this, "DEPRECATED");
		car.Fill(fluid, car.GetFluidCapacity(fluid));
	}
};

class ExpansionPrefabSlot : Managed
{
	string SlotName;
	autoptr array<ref ExpansionPrefabObject> Items = new array<ref ExpansionPrefabObject>();

	void ExpansionPrefabSlot(string slotName = "", array<ref ExpansionPrefabObject> items = null)
	{
		SlotName = slotName;
		if (items)
			Items = items;
	}
};
