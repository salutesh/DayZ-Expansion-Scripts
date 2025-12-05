/**
 * MiscGameplayFunctions.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MiscGameplayFunctions
{
	//! @brief generic function to check if entity is "loose", i.e. has no cargo, can be released/removed if attached,
	//! has no crew if it's a vehicle, and is not a master key.
	//! @note if checkIfRuined is set to true, will return false if entity is ruined.
	static bool Expansion_IsLooseEntity(EntityAI item, bool checkIfRuined = false)
	{
		if (checkIfRuined && item.IsRuined())
			return false;

		#ifdef EXPANSIONMODVEHICLE
		//! If this is a master key of a vehicle, don't allow to be included
		ExpansionCarKey key;
		if (Class.CastTo(key, item) && key.IsMaster())
			return false;
		#endif

		#ifdef EXPANSIONMODQUESTS
		ItemBase itemBase;
		if (Class.CastTo(itemBase, item))
		{
			if (itemBase.Expansion_IsQuestItem())
				return false;
		}
		#endif

		if (item.GetInventory())
		{
			//! Check if the item has a container and any items in it
			if (Expansion_HasAnyCargo(item))
				return false;

			//! Check if item is attachment that can be released
			if (item.GetInventory().IsAttachment())
			{
				if (item.IsMagazine())
					return false;
				EntityAI parent = item.GetHierarchyParent();
				if (!item.CanDetachAttachment(parent))
					return false;
				if (!parent.CanReleaseAttachment(item))
					return false;
				if (!parent.GetInventory().CanRemoveAttachment(item))
					return false;
				return true;
			}
		}
		
		ExpansionVehicle vehicle = ExpansionVehicle.Get(item);
		if (vehicle)
		{
			if (vehicle.GetCrew().Count() > 0)
				return false;
		}

		return true;
	}

	//! Used by p2p market and personal storage, couldn't think of a better name/place for it
	static bool Expansion_ItemCheck(EntityAI item, TStringArray excludedClassNames = null)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.StartStack(EXTrace.GENERAL_ITEMS, MiscGameplayFunctions);
	#endif

		if (item.IsRuined())
			return false;

		if (!item.CanPutInCargo(null))
			return false;

		if (excludedClassNames && ExpansionStatic.IsAnyOf(item, excludedClassNames))
			return false;

		//! Don`t add rotten food items
		Edible_Base foodItem;
		if (Class.CastTo(foodItem, item) && foodItem.HasFoodStage())
		{
			FoodStage foodStage = foodItem.GetFoodStage();
			FoodStageType foodStageType = foodStage.GetFoodStageType();
			if (foodStageType == FoodStageType.ROTTEN || foodStageType == FoodStageType.BURNED)
				return false;
		}

		ExplosivesBase explosive;
		if (Class.CastTo(explosive, item) && explosive.Expansion_IsLive())
			return false;

		#ifdef WRDG_DOGTAGS
		//! Don`t add players own dogtag
		if (item.IsInherited(Dogtag_Base))
		{
			if (!Expansion_IsLooseEntity(item, true))
				return false;
		}
		#endif

		#ifdef EXPANSIONMODQUESTS
		//! Don`t add quest items
		ItemBase itemIB;
		if (Class.CastTo(itemIB, item))
		{
			if (itemIB.Expansion_IsQuestItem() || itemIB.Expansion_IsQuestGiver())
				return false;
		}
		#endif

		return true;
	}

	//! @brief returns the direct children (attachments/cargo)
	static array<EntityAI> Expansion_GetItems(EntityAI item)
	{
		array<EntityAI> items = {};
		GameInventory inventory = item.GetInventory();

		if (inventory)
		{
			int i;

			for (i = 0; i < inventory.AttachmentCount(); ++i)
			{
				items.Insert(inventory.GetAttachmentFromIndex(i));
			}

			CargoBase cargo = inventory.GetCargo();
			if (cargo)
			{
				for (i = 0; i < cargo.GetItemCount(); ++i)
				{
					items.Insert(cargo.GetItem(i));
				}
			}
		}

		return items;
	}

	//! @brief check if entity or any attachment has cargo, use this instead of entity.HasAnyCargo()
	//! because the latter will return false on client if inventory has not been initialized yet
	//! @note uninitialized inventory currently only dealt with if entity is a vehicle
	static bool Expansion_HasAnyCargo(EntityAI item)
	{
		if (item.HasAnyCargo())
			return true;

		if (!item.GetHierarchyParent())
		{
			auto vehicle = ExpansionVehicle.Get(item);
			if (vehicle)
			{
				if (vehicle.GetCargoCount() > 0)
					return true;
			}
		}

		//! Check if any of the item's attachments has any cargo
		for (int i = 0; i < item.GetInventory().AttachmentCount(); i++)
		{
			EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(i);
			if (attachment && attachment.HasAnyCargo())
				return true;
		}

		return false;
	}

	/**
	 * @brief count items in cargo of entity and in cargo of attachments
	 * 
	 * @note does NOT include count of items in cargo of items that are themselves in cargo
	 */
	static int Expansion_CountCargo(EntityAI entity)
	{
		int cargoCount;

		ExpansionVehicle vehicle;
		if (!entity.GetHierarchyParent() && ExpansionVehicle.Get(vehicle, entity))
		{
			cargoCount = vehicle.GetCargoCount();
		}
		else
		{
			CargoBase cargo = entity.GetInventory().GetCargo();
			if (cargo)
				cargoCount = cargo.GetItemCount();
		}

		for (int i = 0; i < entity.GetInventory().AttachmentCount(); i++)
		{
			EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
			cargoCount += Expansion_CountCargo(attachment);
		}

		return cargoCount;
	}

	/**
	 * @brief recursively get items in cargo of entity, in cargo of items in cargo, and in cargo of attachments
	 * 
	 * @param includeAttachments   if true, includes attachments themselves as well, not just their cargo
	 */
	static array<EntityAI> Expansion_GetCargoItems(EntityAI entity, bool includeAttachments = false)
	{
		array<EntityAI> cargoItems = {};

		int i;

		CargoBase cargo = entity.GetInventory().GetCargo();

		if (cargo)
		{
			int cargoCount = cargo.GetItemCount();
			for (i = 0; i < cargoCount; i++)
			{
				EntityAI cargoItem = cargo.GetItem(i);
				cargoItems.Insert(cargoItem);
				cargoItems.InsertAll(Expansion_GetCargoItems(cargoItem));
			}
		}

		for (i = 0; i < entity.GetInventory().AttachmentCount(); i++)
		{
			EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
			if (includeAttachments)
				cargoItems.Insert(attachment);
			cargoItems.InsertAll(Expansion_GetCargoItems(attachment));
		}

		return cargoItems;
	}

	static bool Expansion_MoveCargo(EntityAI src, EntityAI dst, InventoryMode mode = InventoryMode.SERVER)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, MiscGameplayFunctions);
	#endif

		if (!src.GetInventory() || !dst.GetInventory())
			return false;

		CargoBase cargo = src.GetInventory().GetCargo();
		if (!cargo)
			return false;

		int cargoCount = cargo.GetItemCount();
		if (!cargoCount)
			return false;

		CargoBase dstCargo = dst.GetInventory().GetCargo();
		if (!dstCargo)
			return false;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Moving " + cargoCount + " cargo items to " + dst);
	#endif

		auto srcLoc = new InventoryLocation;
		auto dstLoc = new InventoryLocation;

		int moved;
		for (int i = cargoCount - 1; i >= 0; i--)
		{
			EntityAI item = cargo.GetItem(i);
			item.GetInventory().GetCurrentInventoryLocation(srcLoc);
			dstLoc.SetCargo(dst, item, srcLoc.GetIdx(), srcLoc.GetRow(), srcLoc.GetCol(), srcLoc.GetFlip());
			if (!dstLoc.IsValid() || !item.GetInventory().TakeToDst(mode, srcLoc, dstLoc))
				break;
			moved++;
		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Moved " + item + " to " + dst);
		#endif
		}

		#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Moved " + moved + "/" + cargoCount + " cargo items to " + dst);
		#endif

		return true;
	}

	/**
	 * @brief Transfer <amount> catridges from this mag to destination mag
	 * 
	 * @param dst mag to transfer cartridges to
	 * @param amount Number of cartridges to transfer
	 * @param resetDstAmmoCount Reset destination ammo count to zero before transfer
	 * @param totalDamage Will be set to total cartridge damage of all successfully transferred cartridges
	 * 
	 * @return Number of successfully transferred cartridges
	 */
	static int Expansion_TransferCartridges(Magazine src, Magazine dst, int amount, bool resetDstAmmoCount = true, out float totalDamage = 0.0)
	{
		if (resetDstAmmoCount)
			dst.ServerSetAmmoCount(0);

		amount = Math.Min(amount, Math.Min(src.GetAmmoCount(), dst.GetAmmoMax() - dst.GetAmmoCount()));

		int numberOfTransferredCartridges;
		for (int i = 0; i < amount; ++i)
		{
			float damage;
			string cartrige_name;
			if (src.ServerAcquireCartridge(damage, cartrige_name) && dst.ServerStoreCartridge(damage, cartrige_name))
			{
				numberOfTransferredCartridges++;
				totalDamage += damage;
			}
		}

		dst.SetSynchDirty();
		src.SetSynchDirty();

		return numberOfTransferredCartridges;
	}
	
	static array<EntityAI> Expansion_GetAttachments(EntityAI entity)
	{
		array<EntityAI> attachments = {};
		for (int i = 0; i < entity.GetInventory().AttachmentCount(); ++i)
		{
			EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
			if (attachment)
				attachments.Insert(attachment);
		}

		return attachments;
	}
}
