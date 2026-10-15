
class ExpansionDoor
{
	CarDoor m_Door;
	int m_InventorySlotID;

	string m_InventorySlot;
	string m_Selection;
	string m_Animation;

	bool m_IsDoor;
	bool m_IsValid;
	bool m_IsHood;
	bool m_IsTrunk;

	void ExpansionDoor(ExpansionVehicle vehicle, string slot, string rootPath)
	{
		m_InventorySlot = slot;
		m_InventorySlotID = InventorySlots.GetSlotIdFromString(m_InventorySlot);
		InventorySlots.GetSelectionForSlotId(m_InventorySlotID, m_Selection);

		slot.ToLower();

		if (rootPath == string.Empty)
		{
			m_IsDoor = true;
			
			if (!m_Selection)
			{
				int crewSize = vehicle.CrewSize();
				for (int index = 0; index < crewSize; index++)
				{
					string slotName = vehicle.GetDoorInvSlotNameFromSeatPos(index);
					slotName.ToLower();
					if (slotName == slot)
					{
						m_Selection = vehicle.GetDoorSelectionNameFromSeatPos(index);
						break;
					}
				}
			}

			if (m_Selection)
				m_Animation = vehicle.GetAnimSourceFromSelection(m_Selection);
		}
		else
		{
			string path;

			path = rootPath + " selection";
			if (g_Game.ConfigIsExisting(path))
				m_Selection = g_Game.ConfigGetTextOut(path);

			path = rootPath + " animation";
			if (g_Game.ConfigIsExisting(path))
				m_Animation = g_Game.ConfigGetTextOut(path);

			path = rootPath + " isDoor";
			if (g_Game.ConfigIsExisting(path))
				m_IsDoor = g_Game.ConfigGetInt(path);
		}

		if (m_Selection && m_Animation)
			m_IsValid = true;

		string selection = m_Selection;
		selection.ToLower();

		if (slot.Contains("hood") || selection.Contains("hood"))
			m_IsHood = true;
		else if (slot.Contains("trunk") || selection.Contains("trunk"))
			m_IsTrunk = true;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.VEHICLES, this, string.Format("%1 %2 %3 isDoor %4 isHood %5 isTrunk %6", vehicle, m_InventorySlot, m_InventorySlotID, m_IsDoor, m_IsHood, m_IsTrunk));
	#endif
	}
};
