modded class CarDoor
{
	override bool CanDetachAttachment(EntityAI parent)
	{
		// parent casts to CarScript, this causes an error filling up crash log.
		ExpansionVehicleBase veh;
		if (Class.CastTo(veh, parent))
		{
			InventoryLocation loc = new InventoryLocation();
			if (!GetInventory().GetCurrentInventoryLocation(loc))
				return false;

			string slotName = InventorySlots.GetSlotName(loc.GetSlot());
			if (slotName && veh.GetCarDoorsState(slotName) == CarDoorState.DOORS_OPEN)
				return true;

			return false;
		}

		return super.CanDetachAttachment(parent);
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "EEHealthLevelChanged").Add(oldLevel).Add(newLevel).Add(zone);
#endif

		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (!m_Initialized)
			return;

		if (newLevel != GameConstants.STATE_RUINED)
			return;

		if (zone == "Window")
			return;

		if (GetExpansionSettings().GetVehicle().VehicleDropsRuinedDoors)
			Expansion_PhysicsDrop();
	}

	override bool NameOverride(out string output)
	{
		bool ret;

		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, GetHierarchyParent()))
		{
			if (vehicle.IsLocked())
			{
				output = string.Format("%1 (%2)", ConfigGetString("displayName"), "#locked");
				ret = true;
			}

			string ownerName = vehicle.GetOwnerName();
			if (ownerName)
			{
				if (!output)
					output = ConfigGetString("displayName");
				output = string.Format("%1's %2", ownerName, output);
				ret = true;
			}
		}

		return ret;
	}

	override bool Expansion_CarContactActivates()
	{
		return true;
	}

	string ExpansionMainType()
	{
		return "";
	}
};
