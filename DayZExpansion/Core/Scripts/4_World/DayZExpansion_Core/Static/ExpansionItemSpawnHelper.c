/**
 * ExpansionItemSpawnHelper.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionItemSpawnHelper
{
	// ------------------------------------------------------------
	// Expansion Object SpawnOnParent
	// ------------------------------------------------------------
	static Object SpawnOnParent(string className, PlayerBase player, inout EntityAI parent, out int remainingAmount, int quantityPercent = -1, TStringArray attachments = NULL, int skinIndex = -1, bool magFillAmmo = true, inout bool attachmentNotAttached = false)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, "ExpansionItemSpawnHelper", "SpawnOnParent");
#endif

		if (quantityPercent < 0)
			quantityPercent = 100;

		//! Try to spawn in inventory first
		Object obj = SpawnInInventorySecure(className, player, parent, skinIndex);

		ItemBase item;
		if (!Class.CastTo(item, obj))
		{
			//! NULL or not an item

			remainingAmount--;

			if (obj)
			{
				//! Not an item, but was spawned in inventory. Get rid of it.
				//! TODO: This is just here for historical reasons, do we really need it?

				GetGame().ObjectDelete(obj);

				return NULL;
			}

			//! Spawn on ground
			obj = GetGame().CreateObject(className, parent.GetPosition(), false, GetGame().IsKindOf(className, "DZ_LightAI"));
			if (!Class.CastTo(item, obj))
			{
				if (obj)
				{
					return obj;
				}

				return NULL;
			}

			if (skinIndex > -1 && item.ExpansionHasSkin(skinIndex))
				item.ExpansionSetSkin(skinIndex);
		}
		else if (remainingAmount == -1)
		{
			//! Ignore remaining amount
		}
		else if (item.IsAmmoPile())
		{
			//! Ammo/Bullets
			Ammunition_Base ammo = Ammunition_Base.Cast(item);
			int maxAmmo = ammo.GetAmmoMax();

			if (remainingAmount < maxAmmo)
			{
				ammo.ServerSetAmmoCount(remainingAmount);
				remainingAmount = 0;
			}
			else if (!maxAmmo)
			{
				Error("Error: " + className + " max ammo count is zero! Faulty config.cpp");
				remainingAmount = 0;
			}
			else
			{
				ammo.ServerSetAmmoCount(maxAmmo);
				remainingAmount -= maxAmmo;
			}
		}
		else if (item.IsMagazine())
		{
			//! Magazines
			Magazine mag = Magazine.Cast(item);

			remainingAmount--;

			if (magFillAmmo)
				mag.ServerSetAmmoCount(Math.Round(mag.GetAmmoMax() * quantityPercent / 100));
			else
				mag.ServerSetAmmoCount(0);
		}
		else if (item.IsInherited(Edible_Base))
		{
			//! Consumables like food, sodacans, bottles, pills, gas canisters...

			remainingAmount--;

			if (item.HasQuantity())
				item.SetQuantity(item.GetQuantityMax() * quantityPercent / 100);
		}
		else if (item.ConfigGetBool("canBeSplit"))
		{
			//! Stackable items
			int max = item.GetQuantityMax();

			if (remainingAmount < max)
			{
				item.SetQuantity(remainingAmount);
				remainingAmount = 0;
			}
			else if (!max)
			{
				Error("Error: " + className + " max quantity/stack is zero! Faulty config.cpp");
				remainingAmount = 0;
			}
			else
			{
				item.SetQuantity(max);
				remainingAmount -= max;
			}
		}
		else
		{
			//! Everything else

			remainingAmount--;
		}
		
		if (item.CanBeDisinfected())
			item.SetCleanness(1);

		if (attachments)
			SpawnAttachments( attachments, player, item, quantityPercent, skinIndex, magFillAmmo, attachmentNotAttached );

		return obj;
	}
	
	//! Spawn in parent inventory, create player-owned temporary storage container if inventory full
	static Object SpawnInInventorySecure(string className, PlayerBase player, inout EntityAI parent, int skinIndex = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, "ExpansionItemSpawnHelper", "SpawnInInventorySecure");
#endif

		bool canSpawnInInventory = GetGame().IsKindOf(className, "Inventory_Base") || GetGame().IsKindOf(className, "Magazine_Base") || GetGame().IsKindOf(className, "Rifle_Base") || GetGame().IsKindOf(className, "Pistol_Base") || GetGame().IsKindOf(className, "Archery_Base") || GetGame().IsKindOf(className, "Launcher_Base");

		if (!canSpawnInInventory)
			return NULL;

		Object obj;
		ExpansionTemporaryOwnedContainer newStorage;

		while (true)
		{
			ExpansionTemporaryOwnedContainer storage = ExpansionTemporaryOwnedContainer.Cast(parent);
			if (storage)
				storage.ExpansionSetCanReceiveItems(true);

			obj = SpawnInInventory(className, parent, skinIndex);

			if (storage)
				storage.ExpansionSetCanReceiveItems(false);

			if (obj || newStorage)
				break;

			//! If it's an inventory item and couldn't be spawned in parent inventory (likely because it's full),
			//! create new temporary storage container and set as parent
			newStorage = ExpansionTemporaryOwnedContainer.Cast(GetGame().CreateObject("ExpansionTemporaryOwnedContainer", parent.GetPosition()));

			if (!newStorage)
			{
				Error("Failed to create temporary storage container!");

				return NULL;
			}

			newStorage.ExpansionSetContainerOwner(player);

			parent = newStorage;
		}

		return obj;
	}

	// ------------------------------------------------------------
	// Expansion SpawnAttachments
	// ------------------------------------------------------------
	//! Use this form when spawning locally (e.g. only client-side)
	static void SpawnAttachments(TStringArray attachments, EntityAI parent, int skinIndex = -1)
	{
		foreach (string attachmentName: attachments)
		{
			SpawnInInventory(attachmentName, parent, skinIndex);
		}
	}

	//! Use this form when spawning normally (makes sure items get spawned when they don't fit in inventory)
	static void SpawnAttachments(TStringArray attachments, PlayerBase player, EntityAI parent, int quantityPercent = -1, int skinIndex = -1, bool magFillAmmo = true, inout bool attachmentNotAttached = false)
	{
		int remainingAmount = -1;
		foreach (string attachmentName: attachments)
		{
			EntityAI attachmentParent = parent;
			SpawnOnParent(attachmentName, player, attachmentParent, remainingAmount, quantityPercent, NULL, skinIndex, magFillAmmo);
			if (attachmentParent != parent)
				attachmentNotAttached = true;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion EntityAI SpawnInInventory
	// ------------------------------------------------------------
	static EntityAI SpawnInInventory(string name, EntityAI parent, int skinIndex = -1, bool attachOnly = false)
	{
		EntityAI entity;

		if (parent.GetInventory())
		{
			ItemBase parentItem = ItemBase.Cast(parent);
			
			if (parentItem)
				entity = parentItem.ExpansionCreateInInventory(name, attachOnly);
			else if (attachOnly)
				entity = parent.GetInventory().CreateAttachment(name);
			else
				entity = parent.GetInventory().CreateInInventory(name);

			if (entity)
			{
				ItemBase item = ItemBase.Cast(entity);

				if (item && skinIndex > -1 && item.ExpansionHasSkin(skinIndex))
					item.ExpansionSetSkin(skinIndex);
			}
		}

		return entity;
	}

	static EntityAI SpawnAttachment(string name, EntityAI parent, int skinIndex = -1)
	{
		return SpawnInInventory(name, parent, skinIndex, true);
	}

	// ------------------------------------------------------------
	// Expansion Object SpawnVehicle
	// ------------------------------------------------------------
	static Object SpawnVehicle( string className, PlayerBase player, inout EntityAI parent, vector position, vector orientation, out int remainingAmount, TStringArray attachments = NULL, int skinIndex = -1,  string carKeyName = "", inout bool attachmentNotAttached = false )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, "ExpansionItemSpawnHelper", "SpawnVehicle");
#endif

		int flags = ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH;

		if (!GetGame().IsKindOf(className, "ExpansionBoatScript") && !GetGame().IsKindOf(className, "ExpansionVehicleBoatBase"))
			flags |= ECE_TRACE;

		Object obj = GetGame().CreateObjectEx(className, position, flags);

		remainingAmount--;

		CarScript vehicle;
		ItemBase keyb;
		#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicleBase exVeh;
		ExpansionCarKey expkey;
		#endif
		#ifdef MuchCarKey
		MCK_CarKey_Base MCKkey;
		#endif
		if (Class.CastTo(vehicle, obj))
		{
			vehicle.Fill(CarFluid.FUEL, vehicle.GetFluidCapacity(CarFluid.FUEL));
			vehicle.Fill(CarFluid.OIL, vehicle.GetFluidCapacity(CarFluid.OIL));
			vehicle.Fill(CarFluid.BRAKE, vehicle.GetFluidCapacity(CarFluid.BRAKE));
			vehicle.Fill(CarFluid.COOLANT, vehicle.GetFluidCapacity(CarFluid.COOLANT));

			if (skinIndex > -1)
				vehicle.ExpansionSetSkin(skinIndex);

			if (carKeyName != "")
			{
				Class.CastTo(keyb, SpawnInInventorySecure(carKeyName, player, parent));

				#ifdef EXPANSIONMODVEHICLE
				if (Class.CastTo(expkey, keyb) && vehicle.CanBeLocked())
				{
					vehicle.PairKeyTo(expkey);
					vehicle.LockCar(expkey);
				}
				#endif
				#ifdef MuchCarKey
				if (Class.CastTo(MCKkey, keyb))
				{
					int mck_id = MCKkey.GenerateNewID();
					MCKkey.SetNewMCKId(mck_id);
					vehicle.m_CarKeyId = mck_id;
					vehicle.m_HasCKAssigned = true;
					vehicle.m_IsCKLocked = true;
					vehicle.SynchronizeValues(); 
					vehicle.ResetLifetime();
				}
				#endif
			}
		}
		#ifdef EXPANSIONMODVEHICLE
		else if (Class.CastTo(exVeh, obj))
		{
			exVeh.Fill(CarFluid.FUEL, exVeh.GetFluidCapacity(CarFluid.FUEL));
			exVeh.Fill(CarFluid.OIL, exVeh.GetFluidCapacity(CarFluid.OIL));
			exVeh.Fill(CarFluid.BRAKE, exVeh.GetFluidCapacity(CarFluid.BRAKE));
			exVeh.Fill(CarFluid.COOLANT, exVeh.GetFluidCapacity(CarFluid.COOLANT));

			if (skinIndex > -1)
				exVeh.ExpansionSetSkin(skinIndex);

			if (carKeyName != "")
			{
				Class.CastTo(keyb, SpawnInInventorySecure(carKeyName, player, parent));

				if (Class.CastTo(expkey, keyb) && exVeh.CanBeLocked())
				{
					exVeh.PairKeyTo(expkey);
					exVeh.LockCar(expkey);
				}
				#ifdef MuchCarKey
				if (Class.CastTo(MCKkey, keyb))
				{
					int mck_id = MCKkey.GenerateNewID();
					MCKkey.SetNewMCKId(mck_id);
					exVeh.m_CarKeyId = mck_id;
					exVeh.m_HasCKAssigned = true;
					exVeh.m_IsCKLocked = true;
					exVeh.SynchronizeValues(); 
					exVeh.ResetLifetime();
				}
				#endif
			}
		}
		#endif
		else
		{
			if (obj)
			{
				GetGame().ObjectDelete(obj);
			}
			
			return NULL;
		}

		#ifdef EXPANSIONMODVEHICLE
		//! Prevent boat getting flung in the air or stuck on ground due to random physics interaction
		if (obj.IsInherited(ExpansionBoatScript))
		{
			SetVelocity( obj, Vector( 0, 0, 0 ) );
			dBodySetAngularVelocity( obj, Vector( 0, 0, 0 ) );
			dBodyActive( obj, ActiveState.INACTIVE );  //! Boat physics will get reactivated by its DeferredInit
		}
		#endif

		obj.SetOrientation(orientation);

		if (attachments)
			SpawnAttachments(attachments, player, EntityAI.Cast(obj), skinIndex, false, attachmentNotAttached);

		return obj;
	}

	protected static void ISHDebugPrint(string text)
	{
	}

	//! Better version of InventoryLocation.DumpToString (includes slot name instead of ID)
	static string DumpLocationToString(InventoryLocation loc)
	{
		string res = "{ type=" + typename.EnumToString(InventoryLocationType, loc.GetType());

		res += " item=" + Object.GetDebugName(loc.GetItem());

		if (loc.GetParent())
			res += " parent=" + Object.GetDebugName(loc.GetParent());

		switch (loc.GetType())
		{
			case InventoryLocationType.GROUND:
				vector pos = loc.GetPos();
				float dir[4];
				loc.GetDir( dir );
				res += " pos=(" + pos[0] + ", " + pos[1] + ", " + pos[2] + ")";
				res += " dir=(" + dir[0] + ", " + dir[1] + ", " + dir[2] + ", " + dir[3] + ")";
				break;
			case InventoryLocationType.ATTACHMENT:
				res += " slot=" + InventorySlots.GetSlotName(loc.GetSlot());
				break;
			case InventoryLocationType.CARGO:
			case InventoryLocationType.PROXYCARGO:
				res += " idx=" + loc.GetIdx() + " row=" + loc.GetRow() + " col=" + loc.GetCol() + " f=" + loc.GetFlip();
				break;
		}

		res += " }";

		return res;
	}

	//! https://pastebin.com/FFuaPFiT, except without bugs :P
	static EntityAI Clone(EntityAI src, bool recursively = true, InventoryLocation location = null)
	{
		int idx;

		EntityAI dst;
		switch (location.GetType())
		{
			case InventoryLocationType.GROUND:
				dst = GameInventory.LocationCreateEntity(location, src.GetType(), ECE_PLACE_ON_SURFACE, RF_DEFAULT);
				break;
			case InventoryLocationType.ATTACHMENT:
			case InventoryLocationType.HANDS:
				dst = GameInventory.LocationCreateEntity(location, src.GetType(), ECE_IN_INVENTORY, RF_DEFAULT);
				break;
			case InventoryLocationType.CARGO:
			case InventoryLocationType.PROXYCARGO:
				dst = location.GetParent().GetInventory().CreateEntityInCargoEx(src.GetType(), location.GetIdx(), location.GetRow(), location.GetCol(), location.GetFlip());  //! Only way to get flip correct
				break;
			default:
				EXPrint("ExpansionItemSpawnHelper::Clone - unknown location type " + location.GetType());
				return null;
		}

		if (!dst)
		{
			EXPrint("ExpansionItemSpawnHelper::Clone - failed to create " + src.GetType() + " at location " + DumpLocationToString(location));
			return null;
		}

		EXPrint("ExpansionItemSpawnHelper::Clone - created " + Object.GetDebugName(dst) + " at location " + DumpLocationToString(location));

		ScriptReadWriteContext ctx = new ScriptReadWriteContext;
		src.OnStoreSave(ctx.GetWriteContext());
		dst.OnStoreLoad(ctx.GetReadContext(), GetGame().SaveVersion());

		TStringArray dmgZones();
		src.GetDamageZones(dmgZones);

		dst.SetHealth(src.GetHealth());
		foreach (string dmgZone: dmgZones)
		{
			dst.SetHealth(dmgZone, "Health", src.GetHealth(dmgZone, "Health"));
		}

		dst.AfterStoreLoad();

		if (recursively)
		{
			EntityAI cSrc;
			InventoryLocation cLocation();
			for (idx = 0; idx < src.GetInventory().AttachmentCount(); idx++)
			{
				cSrc = src.GetInventory().GetAttachmentFromIndex(idx);
				cSrc.GetInventory().GetCurrentInventoryLocation(cLocation);
                cLocation.SetParent(dst);
				Clone(cSrc, recursively, cLocation);
			}

			if (src.GetInventory().GetCargo())
			{
				for (idx = 0; idx < src.GetInventory().GetCargo().GetItemCount(); idx++)
				{
					cSrc = src.GetInventory().GetCargo().GetItem(idx);
					cSrc.GetInventory().GetCurrentInventoryLocation(cLocation);
					cLocation.SetParent(dst);
					Clone(cSrc, recursively, cLocation);
				}
			}
		}

		dst.EEOnAfterLoad();

		dst.SetSynchDirty();

		return dst;
	}
}
