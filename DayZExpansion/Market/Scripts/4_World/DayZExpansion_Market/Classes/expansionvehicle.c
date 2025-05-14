modded class ExpansionVehicle
{
	protected int m_ParkingFine;

	void EvaluateSafeZoneParkingFine()
	{
		//! If vehicle is not in a safezone, bail
		if (!m_IsInSafeZone)
			return;

		//! If parking time is zero, bail
		if (!m_SZParkingTime)
			return;

		//! If a parking fine is already set, bail
		if (m_ParkingFine > 0)
			return;

		//! If parking time exceeds max allowed vehicle parking time in SZ or vehicle is already forced locked (meaning it was loaded from DB
		//! or entity storage and previously had a fine, since the fine itself is not persistent but the locked state is and forced locked is
		//! only used for fines currently), set parking fine
		auto settings = GetExpansionSettings().GetMarket();
		int baseFine = settings.SZVehicleParkingTicketFine;
		if (baseFine > 0 && (m_SZParkingTime > settings.MaxSZVehicleParkingTime || GetLockState() == ExpansionVehicleLockState.FORCEDLOCKED))
		{
			CloseAllDoors();

			int fine = baseFine;

			EntityAI vehicle = GetEntity();

			array<EntityAI> items = {};
			vehicle.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

			//! Each item in cargo/atts (if att is a container or not directly attached to vehicle) increases the base fine by 1% times the size of the item
			ItemBase ib;
			foreach (EntityAI item: items)
			{
				if (item != vehicle && (item.IsInherited(Container_Base) || item.GetHierarchyParent() != vehicle || item.GetInventory().IsInCargo()) && Class.CastTo(ib, item))
					fine += ib.Expansion_GetItemSize() * baseFine * 0.01;
			}

			SetLockState(ExpansionVehicleLockState.READY_TO_FORCELOCK);
			SetParkingFine(fine);

			if (m_LastDriverUID)
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(m_LastDriverUID);
				if (player)
				{
					CF_Localiser localiser;
					string displayName = GetDisplayName();
					string posText = ExpansionStatic.VectorToString(GetPosition(), ExpansionVectorToString.Labels);
					string fineText = fine.ToString();
					string parkingTimeText = ExpansionStatic.GetTimeString(settings.MaxSZVehicleParkingTime, true);
					if (m_SZCleanup)
					{
						float lifetime = GetExpansionSettings().GetSafeZone().VehicleLifetimeInSafeZone;
						string lifetimeText = ExpansionStatic.GetTimeString(Math.Max(lifetime - m_SZParkingTime, 0), true);
						localiser = new CF_Localiser("STR_EXPANSION_MARKET_PARKING_FINE_VEHICLE_SAFEZONE_CLEANUP", displayName, posText, fineText, parkingTimeText, lifetimeText);
					}
					else
					{
						localiser = new CF_Localiser("STR_EXPANSION_MARKET_PARKING_FINE", displayName, posText, fineText, parkingTimeText);
					}
					ExpansionNotification("STR_EXPANSION_MARKET_TITLE", localiser).Error(player.GetIdentity());
				}
			}
		}
	}

	void SetParkingFine(int fine)
	{
		m_ParkingFine = fine;
		GetEntity().SetSynchDirty();
	}

	int GetParkingFine()
	{
		return m_ParkingFine;
	}
}
