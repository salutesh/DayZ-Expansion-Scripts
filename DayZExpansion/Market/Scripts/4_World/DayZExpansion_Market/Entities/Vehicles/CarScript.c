#ifdef EXPANSIONMODVEHICLE
modded class CarScript
{
	protected int m_Expansion_ParkingFine;

	void CarScript()
	{
		RegisterNetSyncVariableInt("m_Expansion_ParkingFine");
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		Expansion_EvaluateSafeZoneParkingFine();
	}

	void Expansion_EvaluateSafeZoneParkingFine()
	{
		//! If vehicle is not in a safezone, bail
		if (!m_Expansion_IsInSafeZone)
			return;

		//! If parking time is zero, bail
		if (!m_Expansion_SZParkingTime)
			return;

		//! If a parking fine is already set, bail
		if (m_Expansion_ParkingFine > 0)
			return;

		//! If parking time exceeds max allowed vehicle parking time in SZ or vehicle is already forced locked (meaning it was loaded from DB
		//! or entity storage and previously had a fine, since the fine itself is not persistent but the locked state is and forced locked is
		//! only used for fines currently), set parking fine
		auto settings = GetExpansionSettings().GetMarket();
		int baseFine = settings.SZVehicleParkingTicketFine;
		if (baseFine > 0 && (m_Expansion_SZParkingTime > settings.MaxSZVehicleParkingTime || GetLockedState() == ExpansionVehicleLockState.FORCEDLOCKED))
		{
			Expansion_CloseAllDoors();

			int fine = baseFine;

			array<EntityAI> items = {};
			GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

			//! Each item in cargo/atts (if att is a container or not directly attached to vehicle) increases the base fine by 1% times the size of the item
			ItemBase ib;
			foreach (EntityAI item: items)
			{
				if (item != this && (item.IsInherited(Container_Base) || item.GetHierarchyParent() != this || item.GetInventory().IsInCargo()) && Class.CastTo(ib, item))
					fine += ib.Expansion_GetItemSize() * baseFine * 0.01;
			}

			SetLockedState(ExpansionVehicleLockState.READY_TO_FORCELOCK);
			Expansion_SetParkingFine(fine);

			if (m_Expansion_LastDriverUID)
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(m_Expansion_LastDriverUID);
				if (player)
				{
					string text = string.Format("%1 at %2 was given a parking fine of %3 for exceeding the allowed safezone parking time of %4.", GetDisplayName(), ExpansionStatic.VectorToString(GetPosition(), ExpansionVectorToString.Labels), fine, ExpansionStatic.GetTimeString(settings.MaxSZVehicleParkingTime, true));
					if (m_Expansion_SZCleanup)
					{
						float lifetime = GetExpansionSettings().GetSafeZone().VehicleLifetimeInSafeZone;
						text += string.Format(" The vehicle will be deleted in %1 if not moved out of the safezone.", ExpansionStatic.GetTimeString(lifetime - m_Expansion_SZParkingTime, true));
					}
					ExpansionNotification("STR_EXPANSION_MARKET_TITLE", text).Error(player.GetIdentity());
				}
			}
		}
	}

	void Expansion_SetParkingFine(int fine)
	{
		m_Expansion_ParkingFine = fine;
		SetSynchDirty();
	}

	int Expansion_GetParkingFine()
	{
		return m_Expansion_ParkingFine;
	}
}
#endif
