/**
 * ExpansionMarketPlayerItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketPlayerItem
{
	EntityAI Item;
	string ClassName;
	string DisplayName;
	int Count;
	ref map<string, int> ContainerItems;
	int ContainerItemsCount;
	bool IsWeapon = false;
	bool IsMagazine = false;
	bool IsAttached = false;

	void ExpansionMarketPlayerItem(EntityAI item)
	{
		Item = item;
		ClassName = item.GetType();
		DisplayName = item.GetDisplayName();

		if (item.IsWeapon())
			IsWeapon = true;

		if (item.GetInventory().IsAttachment())
		{
			if (!item.GetHierarchyParent().IsInherited(SurvivorBase))
				IsAttached = true;
		}

		if (item.IsInherited(MagazineStorage))
			IsMagazine = true;

		ContainerItems = new map<string, int>;
		UpdateContainerItems(true);
	}

	void UpdateContainerItems(bool ignoreItemKind = false)
	{
		if (ignoreItemKind || Item.IsKindOf("Container_Base"))
		{
			array<EntityAI> items = new array<EntityAI>;
			items.Reserve(Item.GetInventory().CountInventory());

			Item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

			ContainerItems.Clear();
			ContainerItemsCount = 0;

			for ( int i = 0; i < items.Count(); i++ )
			{
				if (items[i] == Item)
					continue;

				//! Skip attachments without cargo on vehicles
				if (items[i].GetInventory().IsAttachment() && !MiscGameplayFunctions.Expansion_HasAnyCargo(items[i]))
				{
					if (items[i].GetHierarchyParent().IsInherited(CarScript))
						continue;
					#ifdef EXPANSIONMODVEHICLE
					else if (items[i].GetHierarchyParent().IsInherited(ExpansionVehicleBase))
						continue;
					#endif
				}

				string name = items[i].GetType();
				int count;

				if (ContainerItems.Find(name, count))
				{
					ContainerItems.Set(name, count + 1);
				}
				else
				{
					ContainerItems.Insert(name, 1);
				}

				ContainerItemsCount++;
			}

			if (!ContainerItemsCount)
			{
				//! Inventory not yet initialized on client, use netsynched cargo count
				CarScript car;
				if (Class.CastTo(car, Item))
				{
					ContainerItemsCount = car.m_Expansion_CargoCount;
				}
				#ifdef EXPANSIONMODVEHICLE
				else
				{
					ExpansionVehicleBase vehicle;
					if (Class.CastTo(vehicle, Item))
					{
						ContainerItemsCount = vehicle.m_Expansion_CargoCount;
					}
				}
				#endif
			}
		}
	}

	TStringArray GetWeaponAttachmentSlotNames(EntityAI weapon)
	{
		TStringArray slots = new TStringArray;

		if (weapon && weapon.IsWeapon())
		{
			weapon.ConfigGetTextArray("attachments", slots);
		}

		return slots;
		slots.Clear();
	}

	bool IsWeapon()
	{
		return IsWeapon;
	}

	bool IsMagazine()
	{
		return IsMagazine;
	}

	bool IsAttached()
	{
		return IsAttached;
	}

	EntityAI GetItem()
	{
		return Item;
	}
};