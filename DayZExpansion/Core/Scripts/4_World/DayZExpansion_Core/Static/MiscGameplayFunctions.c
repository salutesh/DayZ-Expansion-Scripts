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
				if (!item.GetHierarchyParent().CanReleaseAttachment(item))
					return false;
				if (!item.GetHierarchyParent().GetInventory().CanRemoveAttachment(item))
					return false;
				return true;
			}
		}
		
		CarScript car;
		if (Class.CastTo(car, item))
		{
			if (car.Expansion_GetVehicleCrew().Count() > 0)
				return false;
		}
		#ifdef EXPANSIONMODVEHICLE
		else
		{
			ExpansionVehicleBase vehicle;
			if (Class.CastTo(vehicle, item))
			{
				if (vehicle.Expansion_GetVehicleCrew().Count() > 0)
					return false;
			}
		}
		#endif

		return true;
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
			CarScript car;
			if (Class.CastTo(car, item))
			{
				if (car.m_Expansion_CargoCount > 0)
					return true;
			}
			#ifdef EXPANSIONMODVEHICLE
			else
			{
				ExpansionVehicleBase vehicle;
				if (Class.CastTo(vehicle, item))
				{
					if (vehicle.m_Expansion_CargoCount > 0)
						return true;
				}
			}
			#endif
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
}
