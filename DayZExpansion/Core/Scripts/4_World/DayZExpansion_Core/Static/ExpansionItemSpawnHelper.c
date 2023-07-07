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
	static ref map<string, vector> s_VehicleSizes = new map<string, vector>;

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
		else if (!item.IsInherited(Edible_Base) && item.Expansion_IsStackable())
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

			if (item.HasQuantity())
			{
				float itemQuantity = item.GetQuantityMax() * quantityPercent / 100;
				if (item.Expansion_IsStackable())
					itemQuantity = Math.Round(itemQuantity);
				item.SetQuantity(itemQuantity);
			}
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

		bool canSpawnInInventory = ExpansionStatic.IsInventoryItem(className);

		if (!canSpawnInInventory)
		{
			Error("Not an inventory item: " + className);
			return NULL;
		}

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

	static bool IsSpawnPositionFree(vector position, vector orientation, string className, out Object blockingObject = NULL)
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.GENERAL_ITEMS, ExpansionItemSpawnHelper);
#endif

		//className.ToLower();

		vector size;
		if (!s_VehicleSizes.Find(className, size))
		{
			vector minMax[2];
			if (ExpansionStatic.GetCollisionBox(className, minMax))
			{
				size = Vector(minMax[1][0] - minMax[0][0], minMax[1][1] - minMax[0][1], minMax[1][2] - minMax[0][2]);

				#ifdef DIAG
				Object debugBox;

				//! Bottom left
				debugBox = GetGame().CreateObjectEx("ExpansionDebugBox_Blue", position, ECE_NOLIFETIME);
				debugBox.SetOrientation(orientation);
				debugBox.SetPosition(debugBox.ModelToWorld(Vector(-size[0] / 2, 0, -size[2] / 2)));

				//! Bottom right
				debugBox = GetGame().CreateObjectEx("ExpansionDebugBox_Orange", position, ECE_NOLIFETIME);
				debugBox.SetOrientation(orientation);
				debugBox.SetPosition(debugBox.ModelToWorld(Vector(size[0] / 2, 0, -size[2] / 2)));

				//! Top right
				debugBox = GetGame().CreateObjectEx("ExpansionDebugBox_Red", position, ECE_NOLIFETIME);
				debugBox.SetOrientation(orientation);
				debugBox.SetPosition(debugBox.ModelToWorld(Vector(size[0] / 2, size[1], size[2] / 2)));

				//! Top left
				debugBox = GetGame().CreateObjectEx("ExpansionDebugBox_Purple", position, ECE_NOLIFETIME);
				debugBox.SetOrientation(orientation);
				debugBox.SetPosition(debugBox.ModelToWorld(Vector(-size[0] / 2, size[1], size[2] / 2)));
				#endif
			}
			else
			{
				//! Fallback just in case
				size = "5 5 10";
			}
			s_VehicleSizes.Insert(className, size);
		}

		//! Reduce check dimensions by 5% so we don't get blocked by objects the vehicle may stand on
		vector checkDimensions = Vector(size[0] * 0.95, size[1] * 0.95, size[2] * 0.95);
		vector checkPosition = Vector(position[0], position[1] + size[1] * 0.05, position[2]);

		blockingObject = GetObjectBlockingPosition(checkPosition, orientation, checkDimensions);

		CF_Log.Debug("IsVehicleSpawnPositionFree " + className + " size " + size + " pos " + position + " ori " + orientation + " blocking " + blockingObject);
		
		return blockingObject == NULL;
	}
	
	static Object GetObjectBlockingPosition(vector position, vector orientation, vector size, array<Object> excluded_objects = null)
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.GENERAL_ITEMS, ExpansionItemSpawnHelper);
#endif

		array<Object> objects = new array<Object>;
		
		if (!GetGame().IsBoxColliding( position + Vector(0, size[1] / 2, 0), orientation, size, excluded_objects, objects))
			return NULL;

		foreach (Object obj: objects)
		{
			bool match = false;
			//! https://feedback.bistudio.com/T173348
			if (obj.IsInherited(Man) || (obj.IsInherited(ItemBase) && obj.ConfigGetString("physLayer") == "item_large") || obj.IsInherited(CarScript))
				match = true;
			#ifdef EXPANSIONMODVEHICLE
			else if (obj.IsInherited(ExpansionVehicleBase))
				match = true;
			#endif

			if (match)
				return obj;
		}

		return NULL;
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

		bool placeOnSurface;
		//! https://feedback.bistudio.com/T173348
		if (!GetGame().IsKindOf(className, "ExpansionBoatScript") && !GetGame().IsKindOf(className, "ExpansionVehicleBoatBase"))
			placeOnSurface = true;
		if (placeOnSurface)
			flags |= ECE_TRACE;

		Object obj = GetGame().CreateObjectEx(className, position, flags);

		remainingAmount--;

		CarScript vehicle;
		ItemBase keyb;
		#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicleBase exVeh;
		ExpansionCarKey expkey;
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
			}

			#ifdef EXPANSIONMODVEHICLE
			float dist = vehicle.GetModelZeroPointDistanceFromGround();
			if (placeOnSurface && position[1] + dist > vehicle.GetPosition()[1])
				vehicle.SetPosition(position + Vector(0, dist, 0));
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

			if (carKeyName != "")
			{
				Class.CastTo(keyb, SpawnInInventorySecure(carKeyName, player, parent));

				if (Class.CastTo(expkey, keyb) && exVeh.CanBeLocked())
				{
					exVeh.PairKeyTo(expkey);
					exVeh.LockCar(expkey);
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
		int i;

		//! 1) create entity
		EntityAI dst;
		switch (location.GetType())
		{
			case InventoryLocationType.GROUND:
				dst = GameInventory.LocationCreateEntity(location, src.GetType(), ECE_PLACE_ON_SURFACE, RF_DEFAULT);
				break;
			case InventoryLocationType.ATTACHMENT:
				dst = GameInventory.LocationCreateEntity(location, src.GetType(), ECE_IN_INVENTORY, RF_DEFAULT);
				int slotId = location.GetSlot();
				location.GetParent().GetInventory().SetSlotLock(slotId, src.GetHierarchyParent().GetInventory().GetSlotLock(slotId));
				break;
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

		//! @note order of operations matters! DO NOT CHANGE!

		//! 2) attachments + cargo
		if (recursively)
		{
			EntityAI cSrc;
			InventoryLocation cLocation();
			InventoryLocation dLocation();
			for (i = 0; i < src.GetInventory().AttachmentCount(); i++)
			{
				cSrc = src.GetInventory().GetAttachmentFromIndex(i);
				cSrc.GetInventory().GetCurrentInventoryLocation(cLocation);
                dLocation.SetAttachment(dst, null, cLocation.GetSlot());
				Clone(cSrc, recursively, dLocation);
			}

			if (src.GetInventory().GetCargo())
			{
				for (i = 0; i < src.GetInventory().GetCargo().GetItemCount(); i++)
				{
					cSrc = src.GetInventory().GetCargo().GetItem(i);
					cSrc.GetInventory().GetCurrentInventoryLocation(cLocation);
					dLocation.SetCargo(dst, null, cLocation.GetIdx(), cLocation.GetRow(), cLocation.GetCol(), cLocation.GetFlip());
					Clone(cSrc, recursively, dLocation);
				}
			}
		}

		//! 3) special treatment for weapons
		Weapon_Base srcWeapon;
		Weapon_Base dstWeapon;
		if (Class.CastTo(srcWeapon, src) && Class.CastTo(dstWeapon, dst))
		{
			float ammoDamage;
			string ammoTypeName;

			for (int mi = 0; mi < srcWeapon.GetMuzzleCount(); ++mi)
			{
				if (!srcWeapon.IsChamberEmpty(mi))
				{
					if (srcWeapon.GetCartridgeInfo(mi, ammoDamage, ammoTypeName))
					{
						dstWeapon.PushCartridgeToChamber(mi, ammoDamage, ammoTypeName);
					}
				}
				
				for (int ci = 0; ci < srcWeapon.GetInternalMagazineCartridgeCount(mi); ++ci)
				{
					if (srcWeapon.GetInternalMagazineCartridgeInfo(mi, ci, ammoDamage, ammoTypeName))
					{
						dstWeapon.PushCartridgeToInternalMagazine(mi, ammoDamage, ammoTypeName);
					}
				}
			}
		}

		//! 4) storesave/storeload
		ScriptReadWriteContext ctx = new ScriptReadWriteContext;
		src.OnStoreSave(ctx.GetWriteContext());
		dst.OnStoreLoad(ctx.GetReadContext(), GetGame().SaveVersion());

		//! 4b) Volatile variables
		ItemBase srcItem;
		ItemBase dstItem;
		if (Class.CastTo(srcItem, src) && Class.CastTo(dstItem, dst))
		{
			if (!srcItem.Expansion_IsLootable())
				dstItem.Expansion_SetLootable(false);
			dstItem.m_Expansion_PreviousOwner = srcItem.m_Expansion_PreviousOwner;
		}

		//! 5) special treatment for mags
		Magazine srcMag;
		Magazine dstMag;
		if (Class.CastTo(srcMag, src) && Class.CastTo(dstMag, dst))
		{
			float damage;  //! @note damage is the damage of the cartridge itself (0..1), NOT the damage it inflicts!
			string cartTypeName;
			dstMag.ServerSetAmmoCount(0);
			for (i = 0; i < srcMag.GetAmmoCount(); i++)
			{
				srcMag.GetCartridgeAtIndex(i, damage, cartTypeName);
				dstMag.ServerStoreCartridge(damage, cartTypeName);
			}
		}

		//! 6) Special treatment for Car (no special treatment needed for ExpansionVehicleBase since it inherits from ItemBase)
		Car srcCar;
		Car dstCar;
		if (Class.CastTo(srcCar, src) && Class.CastTo(dstCar, dst))
		{
			CarFluid fluid;
			for (i = 0; i < EnumTools.GetEnumSize(CarFluid); i++)
			{
				fluid = EnumTools.GetEnumValue(CarFluid, i);
				dstCar.Fill(fluid, srcCar.GetFluidFraction(fluid) * dstCar.GetFluidCapacity(fluid));
			}
		}

		//! 7) global health and damage zones
		TStringArray dmgZones();
		src.GetDamageZones(dmgZones);
		dst.SetHealth(src.GetHealth());
		foreach (string dmgZone: dmgZones)
		{
			dst.SetHealth(dmgZone, "Health", src.GetHealth(dmgZone, "Health"));
		}

		dst.AfterStoreLoad();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(dst.EEOnAfterLoad);  //! Make sure EEOnAfterLoad gets called AFTER whole hierarchy has loaded

		return dst;
	}
}
