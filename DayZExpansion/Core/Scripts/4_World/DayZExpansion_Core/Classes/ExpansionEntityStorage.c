/**
 * ExpansionEntityStorage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCartridgeInfo
{
	int Count;
	float Damage;
	string Type;

	void OnSave(ParamsWriteContext ctx)
	{
		ctx.Write(Count);
		ctx.Write(Damage);
		ctx.Write(Type);
	}

	bool OnLoad(ParamsReadContext ctx)
	{
		if (!ctx.Read(Count))
			return false;
		if (!ctx.Read(Damage))
			return false;
		if (!ctx.Read(Type))
			return false;

		return true;
	}
}

class ExpansionEntityStorageContext
{
	ref FileSerializer m_Context;
	string m_FileName;
	int m_FailedCount;

	void ExpansionEntityStorageContext()
	{
		m_Context = new FileSerializer();
	}

	bool Open(string fileName, int mode)
	{
		m_FileName = fileName;
		return m_Context.Open(fileName, mode);
	}

	void Close()
	{
		m_Context.Close();
	}
}

[CF_RegisterModule(ExpansionEntityStorageModule)]
class ExpansionEntityStorageModule: CF_ModuleWorld
{
	static const int VERSION = 9;
	static const string EXT = ".bin";

	static const int FAILURE = 0;
	static const int SUCCESS = 1;
	static const int SKIP = 2;

	static ref map<string, ref ExpansionEntityStorageContext> s_SubContexts = new map<string, ref ExpansionEntityStorageContext>;

	static int Now;

#ifdef SERVER
	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.GENERAL_ITEMS);

		super.OnMissionStart(sender, args);

		if (!FileExist(GetStorageDirectory()))
			ExpansionStatic.MakeDirectoryRecursive(GetStorageDirectory());
	}
#endif

	//! @brief save entity and all its children (attachments/cargo) to ctx. Will not delete the entity!
	static bool Save(EntityAI entity, ParamsWriteContext ctx, string basePath, bool inventoryOnly = false, EntityAI placeholder = null, int level = 0)
	{
		if (!level)
		{
			Reset();

			ctx.Write(VERSION);
			ctx.Write(Now);
		}

		//! @note order of operations matters! DO NOT CHANGE!

		if (!inventoryOnly || level > 0)
		{
			int result = Save_Phase1(ctx, entity, inventoryOnly, level);
			if (result == FAILURE)
				return false;
			else if (result == SKIP)
				return true;
		}

		ExpansionEntityStorageContext context;
		FileSerializer file;

		if (level > 0)
		{
			string type = entity.GetType();

			if (!s_SubContexts.Find(type, context))
			{
				string childFileName = string.Format("%1\\%2.bin", basePath, type);
				context = new ExpansionEntityStorageContext();
				if (!context.Open(childFileName, FileMode.WRITE))
					return ErrorFalse("Couldn't open file for writing " + childFileName);
				s_SubContexts[type] = context;
			}

			file = context.m_Context;
		}
		else
		{
			file = FileSerializer.Cast(ctx);
		}

		if (!Save_Phase2(file, basePath, entity, inventoryOnly, placeholder, level))
			return false;

		if (!inventoryOnly || level > 0)
			return Save_Phase3(file, entity);

		return true;
	}

	static int Save_Phase1(ParamsWriteContext ctx, EntityAI entity, bool inventoryOnly = false, int level = 0)
	{
		InventoryLocation il = new InventoryLocation();
		entity.GetInventory().GetCurrentInventoryLocation(il);
		if (!il.IsValid())
		{
			Error(entity.ToString() + ": Invalid location");
			return FAILURE;
		}
		InventoryLocationType ilt = il.GetType();
		EntityAI parent = entity.GetHierarchyParent();
		if (inventoryOnly && ilt == InventoryLocationType.ATTACHMENT && level == 1)
		{
			//! If only saving inventory, don't save attachments on hierarchy root if it is openable or attachment slot is locked
			bool isOpenable;
#ifdef EXPANSIONMODBASEBUILDING
			ItemBase itemParent;
			if (Class.CastTo(itemParent, parent))
				isOpenable = itemParent.ExpansionIsOpenable() || itemParent.IsNonExpansionOpenable();
#endif
			if (isOpenable || parent.GetInventory().GetSlotLock(il.GetSlot()) || entity.IsKindOf("CombinationLock") || entity.IsKindOf("ExpansionCodeLock"))
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Save_Phase1 - skippping " + entity.GetType() + " in locked slot");
				ctx.Write("");  //! Have to write empty entry because we already have written inventory count and no way to retroactively overwrite it
				return SKIP;
			}
		}
		EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Save_Phase1 " + entity.GetType() + " inventory location type " + typename.EnumToString(InventoryLocationType, ilt));

		//! 1a) entity type
		ctx.Write(entity.GetType());

		//! 1b) location
		ctx.Write(ilt);
		switch (ilt)
		{
			case InventoryLocationType.GROUND:
				ctx.Write(entity.GetPosition());
				ctx.Write(entity.GetOrientation());
				break;
			case InventoryLocationType.ATTACHMENT:
				int slotId = il.GetSlot();
				ctx.Write(slotId);
				ctx.Write(parent.GetInventory().GetSlotLock(slotId));
				break;
			case InventoryLocationType.HANDS:
				//! Nothing to do
				break;
			case InventoryLocationType.CARGO:
			case InventoryLocationType.PROXYCARGO:
				ctx.Write(il.GetIdx());
				ctx.Write(il.GetRow());
				ctx.Write(il.GetCol());
				ctx.Write(il.GetFlip());
				break;
			default:
				Error(entity.ToString() + ": Unknown location type " + typename.EnumToString(InventoryLocationType, ilt));
				return FAILURE;
				break;
		}

		return SUCCESS;
	}

	static bool Save_Phase2(ParamsWriteContext ctx, string basePath, EntityAI entity, bool inventoryOnly = false, EntityAI placeholder = null, int level = 0)
	{
		//! 2) attachments + cargo
		if (!level && placeholder && entity.HasAnyCargo() && !MiscGameplayFunctions.Expansion_MoveCargo(entity, placeholder))
			EXPrint("[EntityStorage] " + entity.GetType() + ": Couldn't move cargo to placeholder " + placeholder.GetType());
		int attCount = entity.GetInventory().AttachmentCount();
		int cargoItemCount;
		if (entity.GetInventory().GetCargo())
			cargoItemCount = entity.GetInventory().GetCargo().GetItemCount();
		int inventoryCount = attCount + cargoItemCount;
		ctx.Write(inventoryCount);

		if (!inventoryCount)
			return true;

		if (!level && !FileExist(basePath))
			MakeDirectory(basePath);

		bool success;

		for (int i = 0; i < inventoryCount; i++)
		{
			EntityAI item;
			if (i < attCount)
				item = entity.GetInventory().GetAttachmentFromIndex(i);
			else
				item = entity.GetInventory().GetCargo().GetItem(i - attCount);

			success = Save(item, ctx, basePath, inventoryOnly, null, level + 1);

			if (!success)
				break;
		}

		if (!level)
			Reset();

		if (!success)
			return false;

		return true;
	}

	static bool Save_Phase3(ParamsWriteContext ctx, EntityAI entity)
	{
		int i;

		//! 3) special treatment for weapons
		Weapon_Base weapon;
		if (Class.CastTo(weapon, entity))
		{
			float ammoDamage;
			string ammoTypeName;

			ctx.Write(weapon.GetMuzzleCount());
			for (int mi = 0; mi < weapon.GetMuzzleCount(); ++mi)
			{
				ctx.Write(weapon.IsChamberEmpty(mi));
				if (!weapon.IsChamberEmpty(mi))
				{
					if (weapon.GetCartridgeInfo(mi, ammoDamage, ammoTypeName))
					{
						ctx.Write(ammoDamage);
						ctx.Write(ammoTypeName);
					}
				}

				ctx.Write(weapon.GetInternalMagazineCartridgeCount(mi));
				for (int ci = 0; ci < weapon.GetInternalMagazineCartridgeCount(mi); ++ci)
				{
					if (weapon.GetInternalMagazineCartridgeInfo(mi, ci, ammoDamage, ammoTypeName))
					{
						ctx.Write(ammoDamage);
						ctx.Write(ammoTypeName);
					}
				}
			}
		}

		//! 4) storesave
		int version = GetGame().SaveVersion();
		ctx.Write(version);
		entity.OnStoreSave(ctx);

		//! 5) special treatment for mags/ammo
		Magazine mag;
		if (Class.CastTo(mag, entity))
		{
			if (mag.IsAmmoPile())
			{
				ctx.Write(mag.GetAmmoCount());
			}
			else
			{
				//! Need to store actual cartridges
				//! Try to optimize amount of data written
				auto cartridges = new array<ref ExpansionCartridgeInfo>;
				ExpansionCartridgeInfo info;
				float damage;  //! @note damage is the damage of the cartridge itself (0..1), NOT the damage it inflicts!
				string cartTypeName;
				for (i = 0; i < mag.GetAmmoCount(); i++)
				{
					mag.GetCartridgeAtIndex(i, damage, cartTypeName);
					if (!info || damage != info.Damage || cartTypeName != info.Type)
					{
						info = new ExpansionCartridgeInfo;
						info.Damage = damage;
						info.Type = cartTypeName;
						cartridges.Insert(info);
					}
					info.Count++;
				}
				ctx.Write(cartridges.Count());
				foreach (auto cartridge: cartridges)
				{
					cartridge.OnSave(ctx);
				}
			}
		}

		//! 6) Special treatment for Car (no special treatment needed for ExpansionVehicleBase since it inherits from ItemBase)
		Car car;
		if (Class.CastTo(car, entity))
		{
			int fluidCount = EnumTools.GetEnumSize(CarFluid);
			ctx.Write(fluidCount);
			for (i = 0; i < fluidCount; i++)
			{
				ctx.Write(car.GetFluidFraction(EnumTools.GetEnumValue(CarFluid, i)));
			}
		}

		//! 7) global health and damage zones
		ctx.Write(entity.GetHealth());
		TStringArray dmgZones = new TStringArray();
		entity.GetDamageZones(dmgZones);
		ctx.Write(dmgZones.Count());
		foreach (string dmgZone: dmgZones)
		{
			float dmgZoneHealth = entity.GetHealth(dmgZone, "Health");
			ctx.Write(dmgZone);
			ctx.Write(dmgZoneHealth);
		}

		return true;
	}

	static bool ErrorFalse(string message)
	{
		Error(message);
		return false;
	}

	//! @brief restore entity and all its children from ctx (creates entities)
	static bool Restore(ParamsReadContext ctx, FileSerializer file, string basePath = string.Empty, inout EntityAI entity = null, EntityAI parent = null, EntityAI placeholder = null, PlayerBase player = null, int entityStorageVersion = 0, string type = string.Empty, int level = 0, int elapsed = 0)
	{
		if (!level)
		{
			Reset();

			if (!ctx.Read(entityStorageVersion))
				return ErrorFalse("Couldn't read entity storage version");

			if (entityStorageVersion >= 5)
			{
				int timestamp;
				if (!ctx.Read(timestamp))
					return ErrorFalse("Couldn't read timestamp");

				elapsed = Now - timestamp;
			}
			else if (entityStorageVersion < 4)
			{
				return ErrorFalse("Outdated entity storage version " + entityStorageVersion);
			}
		}

		//! @note order of operations matters! DO NOT CHANGE!

		bool createEntity = level || !entity;
		if (createEntity)
		{
			int result = Restore_Phase1(ctx, entity, parent, player, entityStorageVersion, type, level);
			if (result == FAILURE)
				return false;
			else if (result == SKIP)
				return true;

			if (!parent && dBodyIsSet(entity))
				dBodyActive(entity, ActiveState.INACTIVE);
		}

		if (!file)
			file = FileSerializer.Cast(ctx);

		bool restored = Restore_Phase2(file, basePath, entity, placeholder, player, entityStorageVersion, type, level, elapsed);

		if (restored && createEntity)
			restored = Restore_Phase3(file, entity, entityStorageVersion, elapsed);

		if (!restored && createEntity)
		{
			if (!level && placeholder && entity.HasAnyCargo() && !MiscGameplayFunctions.Expansion_MoveCargo(entity, placeholder))
				Error(entity.ToString() + ": Couldn't move cargo back to placeholder");
			entity.Delete();
		}

		return restored;
	}

	static int Restore_Phase1(ParamsReadContext ctx, out EntityAI entity, EntityAI parent, PlayerBase player = null, int entityStorageVersion = 0, string type = string.Empty, int level = 0)
	{
		//! 1a) entity type
		if (!level || !type)
		{
			if (!ctx.Read(type))
				return ErrorFalse("Couldn't read type");

			if (type == string.Empty)
				return SKIP;
		}

		//! 1b) location (creates entity)
		InventoryLocationType ilt;
		if (!ctx.Read(ilt))
			return ErrorFalse(type + ": Couldn't read inventory location type");
		if (!level && !parent && player)
			parent = player;
		EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Restore_Phase1 " + type + " inventory location type " + typename.EnumToString(InventoryLocationType, ilt));
		InventoryLocation il;
		switch (ilt)
		{
			case InventoryLocationType.GROUND:
				vector position;
				if (!ctx.Read(position))
					return ErrorFalse(type + ": Couldn't read position");
				vector orientation;
				if (!ctx.Read(orientation))
					return ErrorFalse(type + ": Couldn't read orientation");
				if (Class.CastTo(entity, GetGame().CreateObjectEx(type, position, ECE_OBJECT_SWAP, RF_DEFAULT)))
				{
					entity.SetPosition(position);
					entity.SetOrientation(orientation);
				}
				break;
			case InventoryLocationType.ATTACHMENT:
				int slotId;
				if (!ctx.Read(slotId))
					return ErrorFalse(type + ": Couldn't read slot ID");
				bool slotLocked;
				if (entityStorageVersion >= 9 && !ctx.Read(slotLocked))
					return ErrorFalse(type + ": Couldn't read slot locked state");
				il = new InventoryLocation();
				il.SetAttachment(parent, null, slotId);
				entity = GameInventory.LocationCreateEntity(il, type, ECE_IN_INVENTORY, RF_DEFAULT);
				if (entity)
					parent.GetInventory().SetSlotLock(slotId, slotLocked);
				break;
			case InventoryLocationType.HANDS:
				il = new InventoryLocation();
				il.SetHands(parent, null);
				entity = GameInventory.LocationCreateEntity(il, type, ECE_IN_INVENTORY, RF_DEFAULT);
				break;
			case InventoryLocationType.CARGO:
			case InventoryLocationType.PROXYCARGO:
				int idx, row, col;
				bool flip;
				if (!ctx.Read(idx))
					return ErrorFalse(type + ": Couldn't read cargo index");
				if (!ctx.Read(row))
					return ErrorFalse(type + ": Couldn't read cargo row");
				if (!ctx.Read(col))
					return ErrorFalse(type + ": Couldn't read cargo col");
				if (!ctx.Read(flip))
					return ErrorFalse(type + ": Couldn't read cargo flip");
				entity = parent.GetInventory().CreateEntityInCargoEx(type, idx, row, col, flip);  //! Only way to get flip correct
				break;
			default:
				return ErrorFalse(type + ": Unknown inventory location type " + typename.EnumToString(InventoryLocationType, ilt));
				break;
		}

		if (!level && !entity && player)
		{
			//! Try to create in player inventory
			entity = player.GetInventory().CreateInInventory(type);
			//! Try to create on ground at player pos
			if (!entity && Class.CastTo(entity, GetGame().CreateObjectEx(type, player.GetPosition(), ECE_PLACE_ON_SURFACE, RF_DEFAULT)))
				EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Restore_Phase1 - WARNING: Couldn't create " + type + " on " + parent + ", created at player position " + player.GetPosition() + " instead");
		}

		if (!entity)
			return ErrorFalse("No entity created: " + type);

		return SUCCESS;
	}

	static bool Restore_Phase2(ParamsReadContext ctx, string basePath, EntityAI entity, EntityAI placeholder = null, PlayerBase player = null, int entityStorageVersion = 0, string type = string.Empty, int level = 0, int elapsed = 0)
	{
		//! 2) attachments + cargo
		if (!level && placeholder && placeholder.HasAnyCargo() && !MiscGameplayFunctions.Expansion_MoveCargo(placeholder, entity))
			Error("Couldn't move cargo from placeholder");
		int inventoryCount;
		if (!ctx.Read(inventoryCount))
			return ErrorFalse(entity.GetType() + ": Couldn't read inventory count");

		if (!inventoryCount)
			return true;

		string childFileName;
		ExpansionEntityStorageContext context;
		FileSerializer file;

		int restored;

		EntityAI child;
		for (int i = 0; i < inventoryCount; i++)
		{
			if (entityStorageVersion >= 7)
			{
				if (!ctx.Read(type))
					return ErrorFalse("Couldn't read type");

				if (type == string.Empty)
					continue;

				if (!s_SubContexts.Find(type, context))
				{
					childFileName = string.Format("%1\\%2.bin", basePath, type);
					context = new ExpansionEntityStorageContext();
					if (!context.Open(childFileName, FileMode.READ))
						return ErrorFalse("Couldn't open file for reading " + childFileName);
					s_SubContexts[type] = context;
				}

				file = context.m_Context;
			}

			if (!Restore(ctx, file, basePath, child, entity, null, player, entityStorageVersion, type, level + 1, elapsed))
			{
				if (entityStorageVersion < 7)
					return false;
				else
					context.m_FailedCount++;
			}
			else
			{
				restored++;
			}
		}

		if (entityStorageVersion >= 7 && !level)
		{
			if (Reset(true))
				DeleteFile(basePath);
		}

		return restored > 0;
	}

	static bool Restore_Phase3(ParamsReadContext ctx, EntityAI entity, int entityStorageVersion, int elapsed = 0)
	{
		int i;

		//! 3) special treatment for weapons
		Weapon_Base weapon;
		if (Class.CastTo(weapon, entity))
		{
			int muzzleCount;
			if (!ctx.Read(muzzleCount))
				return ErrorFalse(entity.GetType() + ": Couldn't read muzzle count");

			float ammoDamage;
			string ammoTypeName;

			for (int mi = 0; mi < muzzleCount; ++mi)
			{
				bool isChamberEmpty;
				if (!ctx.Read(isChamberEmpty))
					return ErrorFalse(entity.GetType() + ": Couldn't read chamber empty");
				if (!isChamberEmpty)
				{
					if (!ctx.Read(ammoDamage))
						return ErrorFalse(entity.GetType() + ": Couldn't read ammo damage");
					if (!ctx.Read(ammoTypeName))
						return ErrorFalse(entity.GetType() + ": Couldn't read ammo type");
					weapon.PushCartridgeToChamber(mi, ammoDamage, ammoTypeName);
				}

				int internalMagCartridgeCount;
				if (!ctx.Read(internalMagCartridgeCount))
					return ErrorFalse(entity.GetType() + ": Couldn't read internal mag cartridge count");
				for (int ci = 0; ci < internalMagCartridgeCount; ++ci)
				{
					if (!ctx.Read(ammoDamage))
						return ErrorFalse(entity.GetType() + ": Couldn't read ammo damage");
					if (!ctx.Read(ammoTypeName))
						return ErrorFalse(entity.GetType() + ": Couldn't read ammo type");
					weapon.PushCartridgeToInternalMagazine(mi, ammoDamage, ammoTypeName);
				}
			}
		}

		//! 4) storeload
		int version;
		if (!ctx.Read(version))
			return ErrorFalse(entity.GetType() + ": Couldn't read game version");
		if (!entity.OnStoreLoad(ctx, version))
			return ErrorFalse(entity.GetType() + ": Couldn't OnStoreLoad");

		//! 5) special treatment for mags/ammo
		Magazine mag;
		int ammoCount;
		if (Class.CastTo(mag, entity))
		{
			if (mag.IsAmmoPile() || entityStorageVersion < 8)
			{
				if (!ctx.Read(ammoCount))
					return ErrorFalse(entity.GetType() + ": Couldn't read ammo count");
				mag.ServerSetAmmoCount(ammoCount);
			}
			else
			{
				int count;
				if (!ctx.Read(count))
					return ErrorFalse(entity.GetType() + ": Couldn't read cartridge count");
				mag.ServerSetAmmoCount(0);
				auto cartridges = new array<ref ExpansionCartridgeInfo>;
				for (i = 0; i < count; i++)
				{
					auto info = new ExpansionCartridgeInfo;
					if (!info.OnLoad(ctx))
						return ErrorFalse(entity.GetType() + ": Couldn't read cartridge info");
					cartridges.Insert(info);
				}
				foreach (auto cartridge: cartridges)
				{
					for (i = 0; i < cartridge.Count; i++)
					{
						mag.ServerStoreCartridge(cartridge.Damage, cartridge.Type);
					}
				}
			}
		}

		//! 6) Special treatment for Car (no special treatment needed for ExpansionVehicleBase since it inherits from ItemBase)
		Car car;
		if (Class.CastTo(car, entity))
		{
			CarFluid fluid;
			float fluidFraction;
			int fluidEnumSize = EnumTools.GetEnumSize(CarFluid);
			int fluidCount;
			if (entityStorageVersion >= 6)
				ctx.Read(fluidCount);
			else
				fluidCount = fluidEnumSize;
			for (i = 0; i < fluidCount; i++)
			{
				if (i < fluidEnumSize)
					fluid = EnumTools.GetEnumValue(CarFluid, i);
				if (!ctx.Read(fluidFraction))
					return ErrorFalse(entity.GetType() + ": Couldn't read fluid fraction for " + typename.EnumToString(CarFluid, fluid));
				if (i < fluidEnumSize)
					car.Fill(fluid, fluidFraction * car.GetFluidCapacity(fluid));
			}
		}

		//! 7) global health and damage zones
		float globalHealth;
		if (!ctx.Read(globalHealth))
			return ErrorFalse(entity.GetType() + ": Couldn't read global health");
		entity.SetHealth(globalHealth);
		//TStringArray dmgZones = new TStringArray();
		//entity.GetDamageZones(dmgZones);
		int dmgZoneCount;
		if (!ctx.Read(dmgZoneCount))
			return ErrorFalse(entity.GetType() + ": Couldn't read damage zone count");
		for (i = 0; i < dmgZoneCount; i++)
		{
			string dmgZone;
			if (!ctx.Read(dmgZone))
				return ErrorFalse(entity.GetType() + ": Couldn't read damage zone");
			float dmgZoneHealth;
			if (!ctx.Read(dmgZoneHealth))
				return ErrorFalse(entity.GetType() + ": Couldn't read damage zone health");
			//if (dmgZones.Find(dmgZone) > -1)
			entity.SetHealth(dmgZone, "Health", dmgZoneHealth);
			//else
				//EXPrint("[EntityStorage] " + entity.GetType() + ": Couldn't find damage zone \"" + dmgZone + "\"");
		}

		//! 8) Process wetness/temperature/decay
		ItemBase item;
		if (elapsed > 0 && Class.CastTo(item, entity))
			item.Expansion_ProcessWTD(elapsed);

		entity.AfterStoreLoad();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(entity.EEOnAfterLoad);  //! Make sure EEOnAfterLoad gets called AFTER whole hierarchy has loaded

		return true;
	}

	//! Legacy
	static string GetFileName(int id)
	{
		return GetStorageDirectory() + id.ToString() + EXT;
	}

	static string GetFileName(string name)
	{
		return GetStorageDirectory() + name + EXT;
	}

	static string GetStorageDirectory()
	{
		int instance_id = GetGame().ServerConfigGetInt("instanceId");
		return "$mission:storage_" + instance_id + "\\expansion\\entitystorage\\";
	}

	//! @brief saves entity and all its children (attachments/cargo) to file.
	static bool SaveToFile(EntityAI entity, string fileName, bool inventoryOnly = false, EntityAI placeholder = null)
	{
		FileSerializer file = new FileSerializer();

		if (!file.Open(fileName, FileMode.WRITE))
			return ErrorFalse("Couldn't open file for writing " + fileName);

		Now = CF_Date.Now(true).DateToEpoch();

		auto hitch = EXHitch("[ExpansionEntityStorage] ");

		string basePath;
		auto exFileName = new ExpansionString(fileName);
		if (exFileName.EndsWith(EXT))
			basePath = fileName.Substring(0, fileName.Length() - EXT.Length());

		return Save(entity, file, basePath, inventoryOnly, placeholder);
	}

	//! @brief restores entity and all its children from file (creates entities). Deletes file on success.
	//! @note if you pass in an existing entity, only its children (inventory) will be restored.
	static bool RestoreFromFile(string fileName, inout EntityAI entity = null, EntityAI placeholder = null, PlayerBase player = null)
	{
		FileSerializer file = new FileSerializer();

		if (!file.Open(fileName, FileMode.READ))
			return ErrorFalse("Couldn't open file for reading " + fileName);

		bool isInventoryLocked;
		if (entity)
		{
			isInventoryLocked = entity.GetInventory().IsInventoryLockedForLockType(HIDE_INV_FROM_SCRIPT);
			if (isInventoryLocked)
				entity.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		}

		Now = CF_Date.Now(true).DateToEpoch();

		auto hitch = EXHitch("[ExpansionEntityStorage] ");

		string basePath;
		auto exFileName = new ExpansionString(fileName);
		if (exFileName.EndsWith(EXT))
			basePath = fileName.Substring(0, fileName.Length() - EXT.Length());

		bool success = Restore(file, null, basePath, entity, entity, placeholder, player);

		if (isInventoryLocked)
			entity.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);

		file.Close();

		if (!success)
			return false;

		DeleteFile(fileName);

		return true;
	}

	//! @brief saves entity and all its children (attachments/cargo) and replaces original entity with placeholder. Deletes original entity on success!
	//! @note if storeCargo is false (default), move cargo to placeholder, else save cargo to virtual storage
	static bool SaveToFileAndReplace(EntityAI entity, string fileName, string placeholderType, vector position, int iFlags = ECE_OBJECT_SWAP, out EntityAI placeholder = null, bool storeCargo = false)
	{
		Object placeholderObject = GetGame().CreateObjectEx(placeholderType, position, iFlags);

		if (!Class.CastTo(placeholder, placeholderObject))
		{
			EXTrace.Print(EXTrace.GENERAL_ITEMS, entity, "ExpansionEntityStorageModule::SaveToFileAndReplace - Couldn't cast to EntityAI " + placeholderObject);
			GetGame().ObjectDelete(placeholderObject);
			return false;
		}

		bool isInventoryLocked = entity.GetInventory().IsInventoryLockedForLockType(HIDE_INV_FROM_SCRIPT);
		if (isInventoryLocked)
			entity.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);

		bool success;

		if (storeCargo)
			success = SaveToFile(entity, fileName);
		else
			success = SaveToFile(entity, fileName, false, placeholder);

		if (!success)
		{
			if (isInventoryLocked)
				entity.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
			if (placeholder)
				GetGame().ObjectDelete(placeholder);
			return false;
		}

		vector orientation = entity.GetOrientation();

		if ((iFlags & ECE_KEEPHEIGHT) == 0)
		{
			EXTrace.Print(EXTrace.GENERAL_ITEMS, entity, "ExpansionEntityStorageModule::SaveToFileAndReplace - position " + position);

			vector surface = ExpansionStatic.GetSurfaceWaterPosition(position);
			EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "ExpansionEntityStorageModule::SaveToFileAndReplace - surface position " + surface);

			PhxInteractionLayers layerMask;
			layerMask |= PhxInteractionLayers.BUILDING;
			layerMask |= PhxInteractionLayers.DOOR;
			layerMask |= PhxInteractionLayers.VEHICLE;
			layerMask |= PhxInteractionLayers.ROADWAY;
			layerMask |= PhxInteractionLayers.TERRAIN;
			layerMask |= PhxInteractionLayers.ITEM_LARGE;
			layerMask |= PhxInteractionLayers.FENCE;
			vector hitPosition;
			if (DayZPhysics.RayCastBullet(position, surface, layerMask, entity, null, hitPosition, null, null))
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, entity, "ExpansionEntityStorageModule::SaveToFileAndReplace - hit position " + hitPosition);
				position = hitPosition;
			}
			else
			{
				position = surface;
			}
		}

		vector entityMinMax[2];
		if (!entity.GetCollisionBox(entityMinMax))
			entity.ClippingInfo(entityMinMax);

		float entityOffsetY = entityMinMax[0][1];
		if (entityOffsetY > 0)
			entityOffsetY = 0;

		EXTrace.Print(EXTrace.GENERAL_ITEMS, entity, "Collision box min Y " + entityOffsetY);

		GetGame().ObjectDelete(entity);

		if (placeholder)
		{
			vector placeholderMinMax[2];
			if (!placeholder.GetCollisionBox(placeholderMinMax))
				placeholder.ClippingInfo(placeholderMinMax);

			float placeHolderOffsetY = placeholderMinMax[0][1];
			if (placeHolderOffsetY > 0)
				placeHolderOffsetY = 0;

			EXTrace.Print(EXTrace.GENERAL_ITEMS, placeholder, "Collision box min Y " + placeHolderOffsetY);

			position[1] = position[1] + entityOffsetY - placeHolderOffsetY;

			placeholder.SetPosition(position);
			placeholder.SetOrientation(orientation);
		}

		return true;
	}

	static bool Reset(bool deleteRestored = false)
	{
		int failedCount;

		foreach (ExpansionEntityStorageContext context: s_SubContexts)
		{
			context.Close();
			failedCount += context.m_FailedCount;
			if (deleteRestored && !context.m_FailedCount)
				DeleteFile(context.m_FileName);
		}

		s_SubContexts.Clear();

		return !deleteRestored || failedCount == 0;
	}
}
