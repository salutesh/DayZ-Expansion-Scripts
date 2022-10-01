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
	static const int VERSION = 2;
	static const string EXT = ".bin";

	static string s_StorageFolderPath;
	static int s_NextID;

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

		int instance_id = GetGame().ServerConfigGetInt( "instanceId" );
		s_StorageFolderPath = "$mission:storage_" + instance_id + "\\expansion\\entitystorage\\";
		if (FileExist(s_StorageFolderPath))
		{
			TStringArray fileNames = ExpansionStatic.FindFilesInLocation(s_StorageFolderPath, EXT);
			s_NextID = fileNames.Count();
		}
		else
		{
			ExpansionStatic.MakeDirectoryRecursive(s_StorageFolderPath);
		}
	}
#endif

	//! @brief save entity and all its children (attachments/cargo) to ctx. Will not delete the entity!
	static bool Save(EntityAI entity, ParamsWriteContext ctx)
	{
		int i;

		EntityAI parent = entity.GetHierarchyParent();
		if (!parent)
			ctx.Write(VERSION);

		//! @note order of operations matters! DO NOT CHANGE!

		//! 1a) entity type
		ctx.Write(entity.GetType());

		//! 1b) location
		InventoryLocation il = new InventoryLocation();
		entity.GetInventory().GetCurrentInventoryLocation(il);
		if (!il.IsValid())
		{
			Error(entity.ToString() + ": Invalid location");
			return false;
		}
		InventoryLocationType ilt = il.GetType();
		EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Save " + entity.GetType() + " inventory location type " + typename.EnumToString(InventoryLocationType, ilt));
		ctx.Write(ilt);
		switch (ilt)
		{
			case InventoryLocationType.GROUND:
				vector mat[4];
				entity.GetTransform(mat);
				for (i = 0; i < 4; i++)
				{
					ctx.Write(mat[i]);
				}
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
				return false;
				break;
		}

		//! 2) attachments + cargo
		int attCount = entity.GetInventory().AttachmentCount();
		int cargoItemCount;
		if (entity.GetInventory().GetCargo())
			cargoItemCount = entity.GetInventory().GetCargo().GetItemCount();
		ctx.Write(attCount + cargoItemCount);
		for (i = 0; i < attCount + cargoItemCount; i++)
		{
			EntityAI item;
			if (i < attCount)
				item = entity.GetInventory().GetAttachmentFromIndex(i);
			else
				item = entity.GetInventory().GetCargo().GetItem(i - attCount);
			Save(item, ctx);
		}

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

		//! 6) global health and damage zones
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
	static bool Restore(ParamsReadContext ctx, EntityAI parent = null)
	{
		int i;

		int entityStorageVersion;
		if (!parent)
		{
			if (!ctx.Read(entityStorageVersion))
				return ErrorFalse("Couldn't read entity storage version");

			//! TODO: For now, just fail on outdated version. Once this goes to production, should be dealt with appropriately.
			if (entityStorageVersion != VERSION)
				return ErrorFalse("Outdated entity storage version " + entityStorageVersion);
		}

		//! 1a) entity type
		string type;
		if (!ctx.Read(type))
			return ErrorFalse("Couldn't read type");

		//! 1b) location (creates entity)
		InventoryLocationType ilt;
		if (!ctx.Read(ilt))
			return ErrorFalse("Couldn't read inventory location type");
		EXTrace.Print(EXTrace.GENERAL_ITEMS, parent, "ExpansionEntityStorage::Restore " + type + " inventory location type " + typename.EnumToString(InventoryLocationType, ilt));
		InventoryLocation il = new InventoryLocation();
		EntityAI entity;
		switch (ilt)
		{
			case InventoryLocationType.GROUND:
				vector mat[4];
				for (i = 0; i < 4; i++)
				{
					vector tmp;
					if (!ctx.Read(tmp))
						return ErrorFalse("Couldn't read mat[" + i + "]");
					mat[i] = tmp;
				}
				il.SetGround(null, mat);
				entity = GameInventory.LocationCreateEntity(il, type, ECE_PLACE_ON_SURFACE, RF_DEFAULT);
				break;
			case InventoryLocationType.ATTACHMENT:
			case InventoryLocationType.HANDS:
				int slotId;
				if (!ctx.Read(slotId))
					return ErrorFalse("Couldn't read slot ID");
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

		if (!entity)
			return ErrorFalse("No entity created: " + type);

		//! @note order of operations matters! DO NOT CHANGE!

		//! 2) attachments + cargo
		int inventoryCount;
		if (!ctx.Read(inventoryCount))
			return ErrorFalse("Couldn't read inventory count");
		for (i = 0; i < inventoryCount; i++)
		{
			if (!Restore(ctx, entity))
				return false;
		}

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

		//! 6) global health and damage zones
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

		entity.AfterStoreLoad();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(entity.EEOnAfterLoad);  //! Make sure EEOnAfterLoad gets called AFTER whole hierarchy has loaded

		return true;
	}

	static string GetFileName(int id)
	{
		return s_StorageFolderPath + id.ToString() + EXT;
	}

	//! @brief saves entity and all its children (attachments/cargo) to file.
	//! If no ID is given, will choose one and set it.
	static bool SaveToFile(EntityAI entity, inout int id = -1)
	{
		string fileName;
		if (id != -1)
			fileName = GetFileName(id);

		while (id == -1 || FileExist(fileName))
		{
			id = s_NextID++;
			fileName = GetFileName(id);
		}

		FileSerializer file = new FileSerializer();
		
		if (!file.Open(fileName, FileMode.WRITE))
			return ErrorFalse("Couldn't open file for writing " + id);

		return Save(entity, file);
	}

	//! @brief restores entity and all its children from file (creates entities). Deletes file on success.
	static bool RestoreFromFile(int id)
	{
		string fileName = GetFileName(id);

		FileSerializer file = new FileSerializer();
		
		if (!file.Open(fileName, FileMode.READ))
			return ErrorFalse("Couldn't open file for reading " + id);

		bool success = Restore(file);

		file.Close();

		if (!success)
			return false;

		DeleteFile(fileName);

		return true;
	}

	//! @brief saves entity and all its children (attachments/cargo) and replaces original entity with placeholder. Deletes original entity on success!
	static Object SaveToFileAndReplace(EntityAI entity, string placeholderType, inout int id = -1)
	{
		if (!SaveToFile(entity, id))
			return null;

		Object placeholder = GetGame().CreateObject(placeholderType, entity.GetPosition());
		if (!placeholder)
			return null;

		GetGame().ObjectDelete(entity);

		return placeholder;
	}
}

class ExpansionDebugGoat: BuildingSuper
{
	int m_Expansion_EntityStorageID = -1;

	void ExpansionDebugGoat()
	{
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
		m_Expansion_NetsyncData.Set(0, "");
	}

	override void SetActions()
	{
		super.SetActions();

#ifdef DIAG
		AddAction(ExpansionActionDebugRestoreEntity);
#endif
	}

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return;

		ctx.Write(m_Expansion_EntityStorageID);

		string type;
		m_Expansion_NetsyncData.Get(1, type);
		ctx.Write(type);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return true;

		if (!ctx.Read(m_Expansion_EntityStorageID))
			return false;

		string type;
		if (!ctx.Read(type))
			return false;

		m_Expansion_NetsyncData.Set(1, type);

		return true;
	}
#endif

	string Expansion_GetStoredEntityDisplayName()
	{
		string type;
		m_Expansion_NetsyncData.Get(1, type);
		return ExpansionStatic.GetItemDisplayNameWithType(type);
	}
}
