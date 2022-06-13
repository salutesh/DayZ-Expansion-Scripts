class ExpansionPrefabObject : Managed
{
	// Config class name of the item
	string ClassName = "";

	// Number of items in the stack
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

	ExpansionPrefabObject SetQuantity(int quantityMin, int quantityMax = 0)
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

	Object Spawn(Object self, bool ignoreCargo = false)
	{
		if (!self)
		{
			return self;
		}

		foreach (ExpansionHealth health : Health)
		{
			float maxHealth = self.GetMaxHealth(health.Zone, "");
			float healthModifier = Math.RandomFloatInclusive(health.Min, health.Max);
			self.SetHealth(health.Zone, "", maxHealth * healthModifier);
		}

		Weapon_Base weapon;
		bool isWeapon = Class.CastTo(weapon, self);

		EntityAI entity;
		if (Class.CastTo(entity, self))
		{
			Car car;
			if (Class.CastTo(car, entity))
			{
				FillCar(car, CarFluid.FUEL);
				FillCar(car, CarFluid.OIL);
				FillCar(car, CarFluid.BRAKE);
				FillCar(car, CarFluid.COOLANT);
			}

			ItemBase item;
			Magazine mag;
			if (Class.CastTo(item, entity))
			{
				if (item.HasQuantity())
				{
					float quantity = Quantity.GetRandom();

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
							quantity = item.GetQuantityInit();
					}

					if (quantity > 0)
					{
						if (mag)
						{
							mag.ServerSetAmmoCount(quantityMax * quantity);
						}
						else
						{
							item.SetQuantity(quantityMax * quantity);
						}
					}
				}
			}

			GameInventory inventory = entity.GetInventory();
			if (inventory)
			{
				HumanInventory humanInventory = HumanInventory.Cast(inventory);

				array<ref ExpansionPrefabObject> candidates();
				int index;

				//! Cargo from root of any set needs to be spawned last so that cargo containers (e.g. clothing) added via attachments are already there
				array<ref ExpansionPrefabObject> inventoryCargo();
				if (!ignoreCargo)
				{
					foreach (ExpansionPrefabObject thisCargo: InventoryCargo)
					{
						inventoryCargo.Insert(thisCargo);
					}
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
						selectedSet.Spawn(self, true);
						foreach (ExpansionPrefabObject setCargo: selectedSet.InventoryCargo)
						{
							inventoryCargo.Insert(setCargo);
						}
						if (selectedSet.ClassName)
						{
							setNames.Insert(selectedSet.ClassName);
						}
					}
				}

				foreach (auto slot : InventoryAttachments)
				{
					string slotName = slot.SlotName;
					array<ref ExpansionPrefabObject> attachments = slot.Items;

					int slotId = InventorySlots.GetSlotIdFromString(slotName);

					bool slotTaken = false;
					Object child = null;

					candidates.Clear();

					//! Select attachments by chance
					foreach (ExpansionPrefabObject attachment : attachments)
					{
						if (attachment.CanSpawn())
							candidates.Insert(attachment);
					}

					//! Spawn candidate attachments in random order. If no valid inventory slot given, spawn all candidates.
					while (candidates.Count())
					{
						index = candidates.GetRandomIndex();
						attachment = candidates[index];
						candidates.Remove(index);

						switch (slotId)
						{
						case InventorySlots.INVALID:
							child = attachment.Spawn(inventory.CreateAttachment(attachment.ClassName));
							break;
						case InventorySlots.HANDS:
							if (humanInventory)
							{
								child = attachment.Spawn(humanInventory.CreateInHands(attachment.ClassName));
							}

							slotTaken = child != null;
							break;
						case InventorySlots.MAGAZINE:
							int quantMag = attachment.Quantity.GetRandom();
							if (quantMag == 0)
							{
								quantMag = -1;
							}
							slotTaken = weapon.CF_SpawnMagazine(attachment.ClassName, quantMag);
							break;
						default:
							child = attachment.Spawn(inventory.CreateAttachmentEx(attachment.ClassName, slotId));
							slotTaken = child != null;
							break;
						}

						if (slotTaken)
						{
							break;
						}
					}
				}

				candidates.Clear();

				if (ignoreCargo)
					return self;

				//! Select cargo by chance
				foreach (ExpansionPrefabObject cargo : inventoryCargo)
				{
					if (cargo.CanSpawn())
						candidates.Insert(cargo);
				}

				//! Spawn candidate cargo in random order
				while (candidates.Count())
				{
					index = candidates.GetRandomIndex();
					cargo = candidates[index];
					candidates.Remove(index);

					cargo.Spawn(inventory.CreateInInventory(cargo.ClassName));
				}
			}
		}

		return self;
	}

	void FillCar(Car car, CarFluid fluid)
	{
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
