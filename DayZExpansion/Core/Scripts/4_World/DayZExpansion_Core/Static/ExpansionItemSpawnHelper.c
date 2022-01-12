/**
 * ExpansionItemSpawnHelper.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	static Object SpawnOnParent(string className, PlayerBase player, inout EntityAI parent, out int quantity, TStringArray attachments = NULL, int skinIndex = -1, bool magFullAmmo = true, inout bool attachmentNotAttached = false)
	{
		ISHDebugPrint("SpawnOnParent - Start");

		//! Try to spawn in inventory first
		Object obj = SpawnInInventorySecure(className, player, parent, skinIndex);

		ItemBase item;
		if (!Class.CastTo(item, obj))
		{
			//! NULL or not an item

			quantity--;

			if (obj)
			{
				//! Not an item, but was spawned in inventory. Get rid of it.
				//! TODO: This is just here for historical reasons, do we really need it?

				GetGame().ObjectDelete(obj);

				ISHDebugPrint("SpawnOnParent - End and return NULL!");

				return NULL;
			}

			//! Spawn on ground
			obj = GetGame().CreateObject(className, parent.GetPosition(), false, GetGame().IsKindOf(className, "DZ_LightAI"));
			if (!Class.CastTo(item, obj))
			{
				if (obj)
				{
					ISHDebugPrint("SpawnOnParent - End and return obj: " + obj.ToString());

					return obj;
				}

				ISHDebugPrint("SpawnOnParent - End and return NULL!");

				return NULL;
			}

			if (skinIndex > -1 && item.ExpansionHasSkin(skinIndex))
				item.ExpansionSetSkin(skinIndex);
		}
		else if (quantity == -1)
		{
			//! Ignore quantity
			ISHDebugPrint("SpawnOnParent - Ignoring quantity");
		}
		else if (item.IsAmmoPile())
		{
			//! Ammo/Bullets
			Ammunition_Base ammo = Ammunition_Base.Cast(item);
			int maxAmmo = ammo.GetAmmoMax();

			if (quantity < maxAmmo)
			{
				ammo.ServerSetAmmoCount(quantity);
				quantity = 0;
			}
			else if (!maxAmmo)
			{
				Error("Error: " + className + " max ammo count is zero! Faulty config.cpp");
				quantity = 0;
			}
			else
			{
				ammo.ServerSetAmmoCount(maxAmmo);
				quantity -= maxAmmo;
			}

			ISHDebugPrint("SpawnOnParent - AMMO - End and return obj: " + obj.ToString());
		}
		else if (item.IsMagazine())
		{
			//! Magazines
			Magazine mag = Magazine.Cast(item);

			quantity--;

			if (!magFullAmmo)
				mag.ServerSetAmmoCount(0);

			ISHDebugPrint("SpawnOnParent - MAGAZINE - End and return obj: " + obj.ToString());
		}
		else if (item.IsInherited(Edible_Base))
		{
			//! Consumables like food, sodacans, bottles, pills...

			quantity--;

			if (item.HasQuantity())
				item.SetQuantity(item.GetQuantityMax());
		}
		else if (item.ConfigGetBool("canBeSplit"))
		{
			//! Stackable items
			int max = item.GetQuantityMax();

			if (quantity < max)
			{
				item.SetQuantity(quantity);
				quantity = 0;
			}
			else if (!max)
			{
				Error("Error: " + className + " max quantity/stack is zero! Faulty config.cpp");
				quantity = 0;
			}
			else
			{
				item.SetQuantity(max);
				quantity -= max;
			}

			ISHDebugPrint("SpawnOnParent - STACKABLE");
		}
		else
		{
			//! Everything else

			quantity--;
		}
		
		if (item.CanBeDisinfected())
			item.SetCleanness(1);

		if (attachments)
			SpawnAttachments( attachments, player, item, skinIndex, magFullAmmo, attachmentNotAttached );

		ISHDebugPrint("SpawnOnParent - End and return obj: " + obj.ToString());

		return obj;
	}
	
	//! Spawn in parent inventory, create player-owned temporary storage container if inventory full
	static Object SpawnInInventorySecure(string className, PlayerBase player, inout EntityAI parent, int skinIndex = -1)
	{
		bool canSpawnInInventory = GetGame().IsKindOf(className, "Inventory_Base") || GetGame().IsKindOf(className, "Magazine_Base") || GetGame().IsKindOf(className, "Rifle_Base") || GetGame().IsKindOf(className, "Pistol_Base") || GetGame().IsKindOf(className, "Archery_Base") || GetGame().IsKindOf(className, "Launcher_Base");

		if (!canSpawnInInventory)
			return NULL;

		Object obj;
		ExpansionTemporaryOwnedContainer newStorage;

		while (true)
		{
			ISHDebugPrint("ExpansionItemSpawnHelper::SpawnInInventorySecure - parent: " + parent.ClassName());

			ExpansionTemporaryOwnedContainer storage = ExpansionTemporaryOwnedContainer.Cast(parent);
			if (storage)
				storage.ExpansionSetCanReceiveItems(true);

			obj = SpawnInInventory(className, parent, skinIndex);

			if (storage)
				storage.ExpansionSetCanReceiveItems(false);

			if (obj || newStorage)
				break;

			ISHDebugPrint("ExpansionItemSpawnHelper::SpawnInInventorySecure - creating temporary storage container at " + parent.GetPosition());

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
	static void SpawnAttachments(TStringArray attachments, PlayerBase player, EntityAI parent, int skinIndex = -1, bool magFullAmmo = true, inout bool attachmentNotAttached = false)
	{
		int quantity = -1;
		foreach (string attachmentName: attachments)
		{
			EntityAI attachmentParent = parent;
			SpawnOnParent(attachmentName, player, attachmentParent, quantity, NULL, skinIndex, magFullAmmo);
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
	static Object SpawnVehicle( string className, PlayerBase player, inout EntityAI parent, vector position, vector orientation, out int quantity, TStringArray attachments = NULL, int skinIndex = -1, inout bool attachmentNotAttached = false )
	{		
		ISHDebugPrint("SpawnVehicle - Start");

		int flags = ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH;

		if (!GetGame().IsKindOf(className, "ExpansionBoatScript") && !GetGame().IsKindOf(className, "ExpansionVehicleBoatBase"))
			flags |= ECE_TRACE;

		Object obj = GetGame().CreateObjectEx(className, position, flags);

		quantity--;

		CarScript vehicle;
		#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicleBase exVeh;
		ExpansionCarKey key;
		#endif
		if (Class.CastTo(vehicle, obj))
		{
			vehicle.Fill(CarFluid.FUEL, vehicle.GetFluidCapacity(CarFluid.FUEL));
			vehicle.Fill(CarFluid.OIL, vehicle.GetFluidCapacity(CarFluid.OIL));
			vehicle.Fill(CarFluid.BRAKE, vehicle.GetFluidCapacity(CarFluid.BRAKE));
			vehicle.Fill(CarFluid.COOLANT, vehicle.GetFluidCapacity(CarFluid.COOLANT));

			if (skinIndex > -1)
				vehicle.ExpansionSetSkin(skinIndex);

			#ifdef EXPANSIONMODVEHICLE
			if (vehicle.CanBeLocked())
			{
				Class.CastTo(key, SpawnInInventorySecure("ExpansionCarKey", player, parent));

				if (key)
				{
					vehicle.PairKeyTo(key);
					vehicle.LockCar(key);
				}
			}
			#endif
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

			if (exVeh.CanBeLocked())
			{
				Class.CastTo(key, SpawnInInventorySecure("ExpansionCarKey", player, parent));

				if (key)
				{
					exVeh.PairKeyTo(key);
					exVeh.LockCar(key);
				}
			}
		}
		#endif
		else
		{
			if (obj)
			{
				GetGame().ObjectDelete(obj);
			}

			ISHDebugPrint("SpawnVehicle - End return NULL!");		
			
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

		ISHDebugPrint("SpawnVehicle - End return obj:" + obj.ToString());	

		return obj;
	}

	protected static void ISHDebugPrint(string text)
	{
	#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionItemSpawnHelper::" + text);
	#endif
	}
}
