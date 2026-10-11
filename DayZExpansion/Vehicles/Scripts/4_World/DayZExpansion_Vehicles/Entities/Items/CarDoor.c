modded class CarDoor
{
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
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, GetHierarchyParent()))
			return vehicle.NameOverride(output);

		return false;
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
