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
	static Object SpawnOnParent(string className, PlayerBase player, inout EntityAI parent, out int quantity, TStringArray attachments = NULL, int skinIndex = -1, bool magFullAmmo = true)
	{
		ISHDebugPrint("SpawnOnParent - Start");

		//! Try to spawn in inventory first
		Object obj = SpawnInInventorySecure(className, player, parent);

		ItemBase item;
		if (!Class.CastTo(item, obj))
		{
			//! NULL or not an item

			if (obj)
			{
				//! Not an item, but was spawned in inventory. Get rid of it.
				//! TODO: This is just here for historical reasons, do we really need it?

				GetGame().ObjectDelete(obj);

				quantity--;

				ISHDebugPrint("SpawnOnParent - End and return NULL!");

				return NULL;
			}

			//! Spawn on ground
			obj = GetGame().CreateObject(className, parent.GetPosition(), false, GetGame().IsKindOf(className, "DZ_LightAI"));
			if (!Class.CastTo(item, obj))
			{
				quantity--;

				if (obj)
				{
					ISHDebugPrint("SpawnOnParent - End and return obj: " + obj.ToString());

					return obj;
				}

				ISHDebugPrint("SpawnOnParent - End and return NULL!");

				return NULL;
			}
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

		if (attachments)
			SpawnAttachments( attachments, item );

		if (skinIndex > -1)
			item.ExpansionSetSkin(skinIndex);

		ISHDebugPrint("SpawnOnParent - End and return obj: " + obj.ToString());

		return obj;
	}
	
	//! Spawn in parent inventory, create player-owned temporary storage container if inventory full
	static Object SpawnInInventorySecure(string className, PlayerBase player, inout EntityAI parent)
	{
		bool canSpawnInInventory = GetGame().IsKindOf(className, "Inventory_Base") || GetGame().IsKindOf(className, "Magazine_Base") || GetGame().IsKindOf(className, "Rifle_Base") || GetGame().IsKindOf(className, "Pistol_Base") || GetGame().IsKindOf(className, "Archery_Base") || GetGame().IsKindOf(className, "Launcher_Base");

		if (!canSpawnInInventory)
			return NULL;

		Object obj;
		ExpansionTemporaryOwnedContainer newStorage;

		while (true)
		{
			Print("ExpansionItemSpawnHelper::SpawnInInventorySecure - parent: " + parent.ClassName());

			ExpansionTemporaryOwnedContainer storage = ExpansionTemporaryOwnedContainer.Cast(parent);
			if (storage)
				storage.ExpansionSetCanReceiveItems(true);

			obj = parent.GetInventory().CreateInInventory(className);

			if (storage)
				storage.ExpansionSetCanReceiveItems(false);

			if (obj || newStorage)
				break;

			Print("ExpansionItemSpawnHelper::SpawnInInventorySecure - creating temporary storage container at " + parent.GetPosition());

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
	static void SpawnAttachments(TStringArray attachments, EntityAI parent, int skinIndex = -1)
	{
		if (parent.GetInventory())
		{
			ItemBase parentItem = ItemBase.Cast(parent);
			foreach (string attachmentName: attachments)
			{
				ItemBase attachment;

				if (parentItem)
					attachment = ItemBase.Cast(parentItem.ExpansionCreateInInventory(attachmentName));
				else
					attachment = ItemBase.Cast(parent.GetInventory().CreateAttachment(attachmentName));

				if (attachment && skinIndex > -1 && attachment.ExpansionHasSkin(skinIndex))
					attachment.ExpansionSetSkin(skinIndex);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SpawnAttachment
	// ------------------------------------------------------------
	static void SpawnAttachment(string name, EntityAI parent, out ItemBase attachment)
	{
		if (parent.GetInventory())
		{
			ItemBase parentItem = ItemBase.Cast(parent);
			
			if (parentItem)
				attachment = ItemBase.Cast(parentItem.ExpansionCreateInInventory(name));
			else
				attachment = ItemBase.Cast(parent.GetInventory().CreateAttachment(name));			
		}
	}

	// ------------------------------------------------------------
	// Expansion Object SpawnVehicle
	// ------------------------------------------------------------
	static Object SpawnVehicle( string className, PlayerBase player, inout EntityAI parent, vector position, vector orientation, out int quantity, TStringArray attachments = NULL, int skinIndex = -1 )
	{		
		ISHDebugPrint("SpawnVehicle - Start");

		Object obj = GetGame().CreateObject(className, position);

		//! TODO: Add support for ExpansionVehicleBase
		CarScript vehicle;
		if (!Class.CastTo(vehicle, obj))
		{
			quantity--;

			if (obj)
			{
				GetGame().ObjectDelete(obj);
			}

			ISHDebugPrint("SpawnVehicle - End return NULL!");		
			
			return NULL;
		}

		vehicle.SetOrientation(orientation);

		if (attachments)
			SpawnAttachments(attachments, vehicle, skinIndex);

		vehicle.Fill(CarFluid.FUEL, vehicle.GetFluidCapacity(CarFluid.FUEL));
		vehicle.Fill(CarFluid.OIL, vehicle.GetFluidCapacity(CarFluid.OIL));
		vehicle.Fill(CarFluid.BRAKE, vehicle.GetFluidCapacity(CarFluid.BRAKE));
		vehicle.Fill(CarFluid.COOLANT, vehicle.GetFluidCapacity(CarFluid.COOLANT));

		if (skinIndex > -1)
			vehicle.ExpansionSetSkin(skinIndex);

		#ifdef EXPANSIONMODVEHICLE
		if (vehicle.CanBeLocked())
		{
			ExpansionCarKey key;
			Class.CastTo(key, SpawnInInventorySecure("ExpansionCarKey", player, parent));

			if (key)
			{
				vehicle.PairKeyTo(key);
				vehicle.LockCar(key);
			}
		}
		#endif

		quantity--;

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
