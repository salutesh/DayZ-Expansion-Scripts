/**
 * ExpansionEntityStoragePlaceholder.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionEntityStoragePlaceholder: InventoryItemSuper
{
	static ref array<ExpansionEntityStoragePlaceholder> s_Expansion_AllPlaceholders = new array<ExpansionEntityStoragePlaceholder>;

	ref ExpansionGlobalID m_Expansion_StoredEntityGlobalID = new ExpansionGlobalID();
	ref ExpansionNetsyncData m_Expansion_NetsyncData;

	void ExpansionEntityStoragePlaceholder()
	{
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
		s_Expansion_AllPlaceholders.Insert(this);
	}

	void ~ExpansionEntityStoragePlaceholder()
	{
		s_Expansion_AllPlaceholders.RemoveItemUnOrdered(this);
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (HasAnyCargo())
				EXPrint("WARNING: " + GetType() + " " + GetPosition() + " with cargo is about to be deleted!");

			string fileName = Expansion_GetEntityStorageFileName();
			if (FileExist(fileName))
				DeleteFile(fileName);
		}

		super.EEDelete(parent);
	}

	override string Expansion_GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(m_Expansion_StoredEntityGlobalID.IDToHex());
	}

	static ExpansionEntityStoragePlaceholder GetByStoredEntityGlobalID(TIntArray id)
	{
		EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Looking for entity storage placeholder with ID " + id[0].ToString() + " " + id[1].ToString() + " " + id[2].ToString() + " " + id[3].ToString());
		EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Number of placeholders: " + s_Expansion_AllPlaceholders.Count());

		foreach (auto placeholder: s_Expansion_AllPlaceholders)
		{
			if (placeholder.m_Expansion_StoredEntityGlobalID.IsZero())
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Skipping placeholder with zero ID " + placeholder.GetType() + " " + placeholder.GetPosition());
				continue;
			}

			if (placeholder.m_Expansion_StoredEntityGlobalID.IsEqual(id))
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Found matching placeholder " + placeholder.GetType() + " " + placeholder.GetPosition());
				return placeholder;
			}
		}

		return null;
	}

	override string GetDisplayName()
	{
		if (m_Expansion_NetsyncData)
			return Expansion_GetStoredEntityDisplayName();
		return super.GetDisplayName();
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		m_Expansion_NetsyncData.OnRPC(sender, rpc_type, ctx);
	}

	static bool Expansion_HasPlaceholder(CarScript vehicle, bool includeGarage = false)
	{
		if (vehicle.m_Expansion_GlobalID.IsZero())
			return false;

		foreach (auto placeholder: s_Expansion_AllPlaceholders)
		{
#ifdef EXPANSIONMODGARAGE
			//! Dealt with by garage module
			if (!includeGarage && placeholder.IsInherited(ExpansionGarageVehicle))
				continue;
#endif

			if (vehicle.m_Expansion_GlobalID.IsEqual(placeholder.m_Expansion_StoredEntityGlobalID.m_ID))
			{
				string type;
				placeholder.m_Expansion_NetsyncData.Get(0, type);
				EXPrint("ExpansionEntityStoragePlaceholder::Expansion_HasPlaceholder " + vehicle.GetType() + " " + vehicle.GetPosition() + " has identical global ID to placeholder " + type + " " + placeholder.GetPosition());
				if (vehicle.GetType() == type)
					return true;
			}
		}

		return false;
	}

	bool Expansion_HasStoredEntity()
	{
		if (GetGame().IsClient())
		{
			string type;
			return m_Expansion_NetsyncData.Get(0, type);
		}

		if (m_Expansion_StoredEntityGlobalID.IsZero())
			return false;

		return FileExist(Expansion_GetEntityStorageFileName());
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		return false;
	}

	override bool CanDisplayCargo()
	{
		return false;
	}

	override bool CanPutAsAttachment(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return;

		string type;
		m_Expansion_NetsyncData.Get(0, type);
		ctx.Write(type);

		m_Expansion_StoredEntityGlobalID.OnStoreSave(ctx);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return true;

		int version = ctx.GetVersion();

		int storedEntityStorageID;
		if (version < 43)
		{
			if (!ctx.Read(storedEntityStorageID))
				return false;
		}

		string type;
		if (!ctx.Read(type))
			return false;

		m_Expansion_NetsyncData.Set(0, type);

		if (!m_Expansion_StoredEntityGlobalID.OnStoreLoad(ctx))
			return false;

		if (version < 43)
		{
			string oldFileName = ExpansionEntityStorageModule.GetFileName(storedEntityStorageID);
			if (FileExist(oldFileName) && CopyFile(oldFileName, Expansion_GetEntityStorageFileName()))
				DeleteFile(oldFileName);
		}

		return true;
	}
#endif

	string Expansion_GetStoredEntityDisplayName()
	{
		string type;
		m_Expansion_NetsyncData.Get(0, type);
		return ExpansionStatic.GetItemDisplayNameWithType(type);
	}

	void Expansion_SetStoredEntityData(string type, TIntArray entityGlobalID)
	{
		m_Expansion_NetsyncData.Set(0, type);
		for (int i = 0; i < 4; i++)
			m_Expansion_StoredEntityGlobalID.m_ID[i] = entityGlobalID[i];
	}

	static bool Expansion_StoreEntityAndReplace(EntityAI entity, string placeholderType, vector position, int iFlags = ECE_OBJECT_SWAP, out ExpansionEntityStoragePlaceholder placeholder = null, bool storeCargo = false)
	{
		string type = entity.GetType();

		float lifetime = entity.GetLifetimeMax();

		TIntArray entityGlobalID = new TIntArray;
		int i;

		CarScript vehicle;
		if (Class.CastTo(vehicle, entity))
			for (i = 0; i < 4; i++)
				entityGlobalID.Insert(vehicle.m_Expansion_GlobalID.m_ID[i]);

#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicleBase exVehicle;
		if (Class.CastTo(exVehicle, entity))
			for (i = 0; i < 4; i++)
				entityGlobalID.Insert(exVehicle.m_Expansion_GlobalID.m_ID[i]);
#endif

		EntityAI placeholderEntity;
		if (!ExpansionEntityStorageModule.SaveToFileAndReplace(entity, ExpansionEntityStorageModule.GetFileName(ExpansionStatic.IntToHex(entityGlobalID)), placeholderType, position, iFlags, placeholderEntity, storeCargo))
			return false;

		if (Class.CastTo(placeholder, placeholderEntity))
			placeholder.Expansion_SetStoredEntityData(type, entityGlobalID);
		else
			EXPrint("Expansion_StoreEntityAndReplace - WARNING: Couldn't cast to ExpansionEntityStoragePlaceholder " + placeholderEntity);

		if (lifetime > placeholderEntity.GetLifetimeMax())
			placeholderEntity.SetLifetimeMax(lifetime);

		return true;
	}
}

class ExpansionDebugGoat: ExpansionEntityStoragePlaceholder
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionRestoreEntity);
	}
};
