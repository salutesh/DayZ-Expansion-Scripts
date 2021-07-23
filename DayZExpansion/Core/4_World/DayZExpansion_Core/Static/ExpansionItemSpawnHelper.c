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
	//! Oh boy... this needs some good cleaning at some point
	static Object SpawnOnParent(string className, PlayerBase player, inout EntityAI parent, out int quantity, TStringArray attachments = NULL, int skinIndex = -1, bool magFullAmmo = true)
	{
		ISHDebugPrint("SpawnOnParent - Start");

		//! Try to spawn in inventory first
		Object obj = SpawnInInventorySecure(className, player, parent);

		ItemBase item;
		if (!Class.CastTo(item, obj))
		{
			if (obj)
			{
				GetGame().ObjectDelete(obj);

				quantity--;

				ISHDebugPrint("SpawnOnParent - End and return NULL!");

				return NULL;
			}

			//! Spawn on ground
			obj = GetGame().CreateObject(className, parent.GetPosition(), false, GetGame().IsKindOf(className, "DZ_LightAI"));
			if (!Class.CastTo(item, obj))
			{
				if (obj)
				{
					quantity--;

					ISHDebugPrint("SpawnOnParent - End and return obj: " + obj.ToString());

					return obj;
				}

				return NULL;
			}
		}

		if (item.ExpansionIsLiquidItem() || item.IsFood() || item.HasEnergyManager() || item.HasFoodStage())
		{
			quantity--;

			if (item.HasEnergyManager() && attachments)
				SpawnAttachments( attachments, item );

			ISHDebugPrint("SpawnOnParent - End and return obj: " + obj.ToString());			

			return obj;
		}

		//! Consumable items like pils
		Edible_Base edible;
		int maxQuantity;
		Class.CastTo(edible, item);
		if (edible)
		{
			maxQuantity = item.GetQuantityMax();
			Print("SpawnOnParent - Edible - Max quantity: " + maxQuantity);
			if (maxQuantity == 0)
			{
				quantity--;
				ISHDebugPrint("SpawnOnParent - EDIBLE - 1 - End and return obj: " + obj.ToString());
				return obj;			
			}
			else if (maxQuantity > 0)
			{
				quantity--;
				edible.SetQuantity(maxQuantity);
				ISHDebugPrint("SpawnOnParent - EDIBLE - 2 - End and return obj: " + obj.ToString());

				return obj;
			}
		}

		//! Expansion Spraycans
		ExpansionSpraycanBase spray;
		Class.CastTo(spray, item);
		if (spray)
		{
			maxQuantity = item.GetQuantityMax();
			Print("SpawnOnParent - Edible - Max quantity: " + maxQuantity);
			if (maxQuantity == 0)
			{
				quantity--;
				ISHDebugPrint("SpawnOnParent - SPRAYCAN - 1 - End and return obj: " + obj.ToString());
				return obj;			
			}
			else if (maxQuantity > 0)
			{
				quantity--;
				spray.SetQuantity(maxQuantity);
				ISHDebugPrint("SpawnOnParent - SPRAYCAN - 2 - End and return obj: " + obj.ToString());

				return obj;
			}
		}

		//! Ammo/Bullets
		Ammunition_Base ammo;
		int maxAmmo;
		if (item.IsAmmoPile())
		{
			Class.CastTo(ammo, item);
			maxAmmo = ammo.GetAmmoMax();

			if (quantity < maxAmmo)
			{
				ammo.ServerSetAmmoCount(quantity);
				quantity -= quantity;

				ISHDebugPrint("SpawnOnParent - AMMO - 1 - End and return obj: " + obj.ToString());

				return obj;
			}

			ammo.ServerSetAmmoCount(maxAmmo);
			quantity -= maxAmmo;

			ISHDebugPrint("SpawnOnParent - AMMO - 2 - End and return obj: " + obj.ToString());

			return obj;
		}

		//! Magazines
		int max;
		Magazine mag;
		if (Class.CastTo( mag, item ))
		{
			if (item.IsMagazine())
			{
				quantity = quantity - 1;
				if (!magFullAmmo)
					mag.ServerSetAmmoCount(0);

				ISHDebugPrint("SpawnOnParent - MAGAZINE - 1 - End and return obj: " + obj.ToString());			

				return obj;
			}
		}

		//! Stackable with quantity - 1
		int currentVarStackMax;
		float varStackMax = item.ConfigGetFloat("varStackMax");
		ISHDebugPrint("SpawnOnParent - VAR STACK - Start");
		ISHDebugPrint("SpawnOnParent - VAR STACK - varStackMax: " + varStackMax);
		if (varStackMax > 0)
		{
			currentVarStackMax = item.GetQuantityMax();
			if (varStackMax > 0 || currentVarStackMax > 0)
			{
				if (varStackMax > 0)
					currentVarStackMax = varStackMax;

				if (quantity < currentVarStackMax)
				{
					if (!item.ConfigGetBool("destroyOnEmpty"))
						item.SetQuantity(0);
					else
						item.SetQuantity(quantity);
					quantity = 0;

					ISHDebugPrint("SpawnOnParent - VAR STACK - 1 - End and return obj: " + obj.ToString());

					return obj;
				}
				else
				{
					quantity -= currentVarStackMax;
					item.SetQuantity(currentVarStackMax);				

					ISHDebugPrint("SpawnOnParent - VAR STACK - 2 - End and return obj: " + obj.ToString());

					return obj;
				}
			}
		}
		
		//! Stackable with quantity - 2
		int currentvarQuantityMax;
		int varQuantityMax = item.ConfigGetInt("varQuantityMax");
		ISHDebugPrint("SpawnOnParent - VAR QUANTITY - Start");
		ISHDebugPrint("SpawnOnParent - VAR QUANTITY - varQuantityMax: " + varQuantityMax);
		if (varQuantityMax > 0)
		{
			currentvarQuantityMax = item.GetQuantityMax();
			ISHDebugPrint("SpawnOnParent - VAR QUANTITY - currentvarQuantityMax: " + currentvarQuantityMax);
			if (varQuantityMax > 0 || max > 0)
			{
				if (varQuantityMax > 0)
					currentvarQuantityMax = varQuantityMax;
	
				if (quantity < currentvarQuantityMax)
				{
					ISHDebugPrint("SpawnOnParent - VAR QUANTITY - quantity: " + quantity);
					item.SetQuantity(quantity);
					quantity = 0;

					ISHDebugPrint("SpawnOnParent - VAR QUANTITY - 1 - End and return obj: " + obj.ToString());

					return obj;
				}
				else
				{
					item.SetQuantity(currentvarQuantityMax);
					quantity -= currentvarQuantityMax;				

					ISHDebugPrint("SpawnOnParent - VAR QUANTITY - 2 - End and return obj: " + obj.ToString());

					return obj;
				}
			}
		}

		if (attachments)
			SpawnAttachments( attachments, item );

		if (skinIndex > -1)
			item.ExpansionSetSkin(skinIndex);

		quantity--;

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
	static void SpawnAttachments(ref TStringArray attachments, EntityAI parent, int skinIndex = -1)
	{
		if (parent.GetInventory())
		{
			ItemBase parentItem = ItemBase.Cast(parent);
			foreach (string attachmentName: attachments)
			{
				ItemBase attachment;

				if (parentItem)
					attachment = parentItem.ExpansionCreateInInventory(attachmentName);
				else
					attachment = parent.GetInventory().CreateAttachment(attachmentName);

				if (attachment && skinIndex > -1 && attachment.ExpansionHasSkin(skinIndex))
					attachment.ExpansionSetSkin(skinIndex);
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion Object SpawnVehicle
	// ------------------------------------------------------------
	static Object SpawnVehicle( string className, PlayerBase player, inout EntityAI parent, vector position, vector direction, out int quantity, ref TStringArray attachments = NULL, int skinIndex = -1 )
	{		
		ISHDebugPrint("SpawnVehicle - Start");

		Object obj = GetGame().CreateObjectEx(className, position, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_PLACE_ON_SURFACE);

		//! TODO: Add support for ExpansionVehicleBase
		CarScript vehicle;
		if (!Class.CastTo(vehicle, obj))
		{
			if (obj)
			{
				GetGame().ObjectDelete(obj);
			}

			ISHDebugPrint("SpawnVehicle - End return false!");		
			
			return NULL;
		}

		vehicle.SetDirection(direction);
		vehicle.SetPosition(position);

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
