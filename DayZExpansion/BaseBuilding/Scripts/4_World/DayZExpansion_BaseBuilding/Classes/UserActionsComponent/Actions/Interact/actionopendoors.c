modded class ActionOpenDoors
{
	override string GetText()
	{
		string text = super.GetText();

		if (text == "#STR_EXPANSION_OPEN_LOCKED_DOOR" && GetPermissionsManager().IsAdminToolsToggledOn())
			text = "[ADMIN] " + text;

		return text;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item)) 
			return false;

		if (m_Text == "#STR_EXPANSION_OPEN_LOCKED_DOOR")
			m_Text = "#open";

		BuildingBase building;
		if (Class.CastTo(building, target.GetObject()))
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if (doorIndex != -1)
			{
				ItemBase codelock = building.Expansion_GetCodeLock(doorIndex);
				if (codelock)
				{
					//! If an Expansion codelock is attached, open action is handled by ExpansionActionOpen
					if (codelock.IsInherited(ExpansionCodeLock))
						return false;

					if (codelock.ExpansionIsLocked() && m_Text == "#open")
						m_Text = "#STR_EXPANSION_OPEN_LOCKED_DOOR";
				}
			}
		}

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		BuildingBase building;
		if (Class.CastTo(building, action_data.m_Target.GetObject()))
		{
			int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
			if (doorIndex != -1 && building.IsDoorLocked(doorIndex))
			{
				PlayerBase player = action_data.m_Player;
				DoorManipulationParams doorParams = new DoorManipulationParams();
				doorParams.m_DoorIndex = doorIndex;
				doorParams.m_CheckIfLocked = CheckIfDoorIsLocked();
				doorParams.m_Caller = player;
				if (building.CanDoorBeOpened(doorParams))
				{
					ItemBase codelock = building.Expansion_GetCodeLock(doorIndex);
					if (codelock && codelock.ExpansionIsLocked())
					{
						if (codelock.IsKnownUser(player) || codelock.Expansion_CanUnlockFromInside(player, ""))
						{
							codelock.ExpansionUnlock();
							building.OpenDoor(doorIndex);
						}
					}
				}
			}
		}

		super.OnStartServer(action_data);
	}
}
