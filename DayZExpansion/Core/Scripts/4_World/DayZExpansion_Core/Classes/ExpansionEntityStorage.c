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

[CF_RegisterModule(ExpansionEntityStorageModule)]
class ExpansionEntityStorageModule: CF_ModuleWorld
{
	static const int VERSION = 5;
	static const string EXT = ".bin";

	static const int FAILURE = 0;
	static const int SUCCESS = 1;
	static const int SKIP = 2;

	static string s_StorageFolderPath;

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

		int instance_id = GetGame().ServerConfigGetInt("instanceId");
		s_StorageFolderPath = "$mission:storage_" + instance_id + "\\expansion\\entitystorage\\";
		if (!FileExist(s_StorageFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_StorageFolderPath);
	
		string oldpath = "$mission:expansion\\entitystorage\\";
		if (FileExist(oldpath))
			ExpansionStatic.CopyDirectoryTree(oldpath, s_StorageFolderPath, ".bin", true);
	}
#endif

	//! @brief save entity and all its children (attachments/cargo) to ctx. Will not delete the entity!
	static bool Save(EntityAI entity, ParamsWriteContext ctx, bool inventoryOnly = false, EntityAI placeholder = null, int level = 0)
	{
		if (!level)
		{
			ctx.Write(VERSION);
			ctx.Write(CF_Date.Now(true).DateToEpoch());
		}

		//! @note order of operations matters! DO NOT CHANGE!

		if (!inventoryOnly || level > 0)
		{
			int result = Save_Phase1(ctx, entity, inventoryOnly);
			if (result == FAILURE)
				return false;
			else if (result == SKIP)
				return true;
		}

		if (!Save_Phase2(ctx, entity, inventoryOnly, placeholder, level))
			return false;

		if (!inventoryOnly || level > 0)
			return Save_Phase3(ctx, entity);

		return true;
	}

	static int Save_Phase1(ParamsWriteContext ctx, EntityAI entity, bool inventoryOnly = false)
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
		if (inventoryOnly && ilt == InventoryLocationType.ATTACHMENT)
		{
			//! Don't save entities in locked slots if only saving inventory
			if (parent.GetInventory().GetSlotLock(il.GetSlot()) || entity.IsKindOf("CombinationLock") || entity.IsKindOf("ExpansionCodeLock"))
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Save_Phase1 - skippping " + entity.GetType() + " in locked slot");
				ctx.Write("");
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
			case InventoryLocationType.HANDS:
				ctx.Write(il.GetSlot());
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
	
	static bool Save_Phase2(ParamsWriteContext ctx, EntityAI entity, bool inventoryOnly = false, EntityAI placeholder = null, int level = 0)
	{
		//! 2) attachments + cargo
		if (!level && placeholder && entity.HasAnyCargo() && !MiscGameplayFunctions.Expansion_MoveCargo(entity, placeholder))
			EXPrint(entity.ToString() + ": Couldn't move cargo to placeholder");
		int attCount = entity.GetInventory().AttachmentCount();
		int cargoItemCount;
		if (entity.GetInventory().GetCargo())
			cargoItemCount = entity.GetInventory().GetCargo().GetItemCount();
		ctx.Write(attCount + cargoItemCount);
		for (int i = 0; i < attCount + cargoItemCount; i++)
		{
			EntityAI item;
			if (i < attCount)
				item = entity.GetInventory().GetAttachmentFromIndex(i);
			else
				item = entity.GetInventory().GetCargo().GetItem(i - attCount);
			if (!Save(item, ctx, inventoryOnly, null, level + 1))
				return false;
		}

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

		//! 5) special treatment for mags
		Magazine mag;
		if (Class.CastTo(mag, entity))
			ctx.Write(mag.GetAmmoCount());

		//! 6) Special treatment for Car (no special treatment needed for ExpansionVehicleBase since it inherits from ItemBase)
		Car car;
		if (Class.CastTo(car, entity))
		{
			for (i = 0; i < EnumTools.GetEnumSize(CarFluid); i++)
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
	static bool Restore(ParamsReadContext ctx, inout EntityAI entity = null, EntityAI parent = null, EntityAI placeholder = null, PlayerBase player = null, int level = 0, int elapsed = 0)
	{
		int entityStorageVersion;
		if (!level)
		{
			if (!ctx.Read(entityStorageVersion))
				return ErrorFalse("Couldn't read entity storage version");

			if (entityStorageVersion >= 5)
			{
				int timestamp;
				if (!ctx.Read(timestamp))
					return ErrorFalse("Couldn't read timestamp");

				elapsed = CF_Date.Now(true).DateToEpoch() - timestamp;
			}
			else if (entityStorageVersion < 4)
			{
				return ErrorFalse("Outdated entity storage version " + entityStorageVersion);
			}
		}

		//! @note order of operations matters! DO NOT CHANGE!

		if (level || !entity)
		{
			int result = Restore_Phase1(ctx, entity, parent, player);
			if (result == FAILURE)
				return false;
			else if (result == SKIP)
				return true;

			if (!parent && dBodyIsSet(entity))
				dBodyActive(entity, ActiveState.INACTIVE);
		}

		if (!Restore_Phase2(ctx, entity, placeholder, player, level, elapsed))
			return false;

		if (entity != parent)
			return Restore_Phase3(ctx, entity, elapsed);

		return true;
	}

	static int Restore_Phase1(ParamsReadContext ctx, out EntityAI entity, EntityAI parent, PlayerBase player = null)
	{
		//! 1a) entity type
		string type;
		if (!ctx.Read(type))
			return ErrorFalse("Couldn't read type");

		if (type == string.Empty)
			return SKIP;

		//! 1b) location (creates entity)
		InventoryLocationType ilt;
		if (!ctx.Read(ilt))
			return ErrorFalse("Couldn't read inventory location type");
		EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Restore_Phase1 " + type + " inventory location type " + typename.EnumToString(InventoryLocationType, ilt));
		switch (ilt)
		{
			case InventoryLocationType.GROUND:
				vector position;
				if (!ctx.Read(position))
					return ErrorFalse("Couldn't read position");
				vector orientation;
				if (!ctx.Read(orientation))
					return ErrorFalse("Couldn't read orientation");
				if (Class.CastTo(entity, GetGame().CreateObjectEx(type, position, ECE_PLACE_ON_SURFACE, RF_DEFAULT)))
				{
					entity.SetPosition(position);
					entity.SetOrientation(orientation);
				}
				break;
			case InventoryLocationType.ATTACHMENT:
			case InventoryLocationType.HANDS:
				int slotId;
				if (!ctx.Read(slotId))
					return ErrorFalse("Couldn't read slot ID");
				InventoryLocation il = new InventoryLocation();
				il.SetAttachment(parent, null, slotId);
				entity = GameInventory.LocationCreateEntity(il, type, ECE_IN_INVENTORY, RF_DEFAULT);
				break;
			case InventoryLocationType.CARGO:
			case InventoryLocationType.PROXYCARGO:
				int idx, row, col;
				bool flip;
				if (!ctx.Read(idx))
					return ErrorFalse("Couldn't read index");
				if (!ctx.Read(row))
					return ErrorFalse("Couldn't read row");
				if (!ctx.Read(col))
					return ErrorFalse("Couldn't read col");
				if (!ctx.Read(flip))
					return ErrorFalse("Couldn't read flip");
				entity = parent.GetInventory().CreateEntityInCargoEx(type, idx, row, col, flip);  //! Only way to get flip correct
				break;
			default:
				return ErrorFalse("Unknown inventory location type " + typename.EnumToString(InventoryLocationType, ilt));
				break;
		}

		if (!entity && player)
		{
			//! Create on ground at player pos
			if (Class.CastTo(entity, GetGame().CreateObjectEx(type, player.GetPosition(), ECE_PLACE_ON_SURFACE, RF_DEFAULT)))
				EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Restore_Phase1 - WARNING: Couldn't create " + type + " on " + parent + ", created at player position " + player.GetPosition() + " instead");
		}

		if (!entity)
			return ErrorFalse("No entity created: " + type);

		return SUCCESS;
	}

	static bool Restore_Phase2(ParamsReadContext ctx, EntityAI entity, EntityAI placeholder = null, PlayerBase player = null, int level = 0, int elapsed = 0)
	{
		//! 2) attachments + cargo
		if (!level && placeholder && placeholder.HasAnyCargo() && !MiscGameplayFunctions.Expansion_MoveCargo(placeholder, entity))
			Error("Couldn't move cargo from placeholder");
		int inventoryCount;
		if (!ctx.Read(inventoryCount))
			return ErrorFalse("Couldn't read inventory count");
		EntityAI child;
		for (int i = 0; i < inventoryCount; i++)
		{
			if (!Restore(ctx, child, entity, null, player, level + 1, elapsed))
				return false;
		}

		return true;
	}

	static bool Restore_Phase3(ParamsReadContext ctx, EntityAI entity, int elapsed = 0)
	{
		int i;

		//! 3) special treatment for weapons
		Weapon_Base weapon;
		if (Class.CastTo(weapon, entity))
		{
			int muzzleCount;
			if (!ctx.Read(muzzleCount))
				return ErrorFalse("Couldn't read muzzle count");

			float ammoDamage;
			string ammoTypeName;

			for (int mi = 0; mi < muzzleCount; ++mi)
			{
				bool isChamberEmpty;
				if (!ctx.Read(isChamberEmpty))
					return ErrorFalse("Couldn't read chamber empty");
				if (!isChamberEmpty)
				{
					if (!ctx.Read(ammoDamage))
						return ErrorFalse("Couldn't read ammo damage");
					if (!ctx.Read(ammoTypeName))
						return ErrorFalse("Couldn't read ammo type");
					weapon.PushCartridgeToChamber(mi, ammoDamage, ammoTypeName);
				}

				int internalMagCartridgeCount;
				if (!ctx.Read(internalMagCartridgeCount))
					return ErrorFalse("Couldn't read internal mag cartridge count");
				for (int ci = 0; ci < internalMagCartridgeCount; ++ci)
				{
					if (!ctx.Read(ammoDamage))
						return ErrorFalse("Couldn't read ammo damage");
					if (!ctx.Read(ammoTypeName))
						return ErrorFalse("Couldn't read ammo type");
					weapon.PushCartridgeToInternalMagazine(mi, ammoDamage, ammoTypeName);
				}
			}
		}

		//! 4) storeload
		int version;
		if (!ctx.Read(version))
			return ErrorFalse("Couldn't read game version");
		if (!entity.OnStoreLoad(ctx, version))
			return ErrorFalse("Couldn't OnStoreLoad");

		//! 5) special treatment for mags
		Magazine mag;
		int ammoCount;
		if (Class.CastTo(mag, entity))
		{
			if (!ctx.Read(ammoCount))
				return ErrorFalse("Couldn't read ammo count");
			mag.ServerSetAmmoCount(ammoCount);
		}

		//! 6) Special treatment for Car (no special treatment needed for ExpansionVehicleBase since it inherits from ItemBase)
		Car car;
		if (Class.CastTo(car, entity))
		{
			CarFluid fluid;
			float fluidFraction;
			for (i = 0; i < EnumTools.GetEnumSize(CarFluid); i++)
			{
				fluid = EnumTools.GetEnumValue(CarFluid, i);
				if (!ctx.Read(fluidFraction))
					return ErrorFalse("Couldn't read fluid fraction for " + typename.EnumToString(CarFluid, fluid));
				car.Fill(fluid, fluidFraction * car.GetFluidCapacity(fluid));
			}
		}

		//! 7) global health and damage zones
		float globalHealth;
		if (!ctx.Read(globalHealth))
			return ErrorFalse("Couldn't read global health");
		entity.SetHealth(globalHealth);
		TStringArray dmgZones = new TStringArray();
		int dmgZoneCount;
		if (!ctx.Read(dmgZoneCount))
			return ErrorFalse("Couldn't read damage zone count");
		for (i = 0; i < dmgZoneCount; i++)
		{
			string dmgZone;
			if (!ctx.Read(dmgZone))
				return ErrorFalse("Couldn't read damage zone");
			float dmgZoneHealth;
			if (!ctx.Read(dmgZoneHealth))
				return ErrorFalse("Couldn't read damage zone health");
			entity.SetHealth(dmgZone, "Health", dmgZoneHealth);
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
		return s_StorageFolderPath + id.ToString() + EXT;
	}

	static string GetFileName(string name)
	{
		return s_StorageFolderPath + name + EXT;
	}

	//! @brief saves entity and all its children (attachments/cargo) to file.
	static bool SaveToFile(EntityAI entity, string fileName, bool inventoryOnly = false, EntityAI placeholder = null)
	{
		FileSerializer file = new FileSerializer();
		
		if (!file.Open(fileName, FileMode.WRITE))
			return ErrorFalse("Couldn't open file for writing " + fileName);

		return Save(entity, file, inventoryOnly, placeholder);
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

		bool success = Restore(file, entity, entity, placeholder, player);

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
}
