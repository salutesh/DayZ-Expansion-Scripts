modded class ActionCloseDoors
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item)) 
			return false;

		if (m_Text == "#STR_EXPANSION_BB_CODE_CLOSE_LOCK")
			m_Text = "#close";

		BuildingBase building;
		if (Class.CastTo(building, target.GetObject()))
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if (doorIndex != -1)
			{
				ItemBase codelock = building.Expansion_GetCodeLock(doorIndex);
				if (codelock)
				{
					//! If an Expansion codelock is attached, close action is handled by ExpansionActionClose
					if (codelock.IsInherited(ExpansionCodeLock))
						return false;

					if (!codelock.IsDamageDestroyed() && codelock.HasCode() && !codelock.ExpansionIsLocked() && m_Text == "#close")
						m_Text = "#STR_EXPANSION_BB_CODE_CLOSE_LOCK";
				}
			}
		}

		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		BuildingBase building;
		if (Class.CastTo(building, action_data.m_Target.GetObject()))
		{
			int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
			if (doorIndex != -1)
			{
				ItemBase codelock = building.Expansion_GetCodeLock(doorIndex);
				if (codelock && !codelock.IsDamageDestroyed() && codelock.HasCode() && !codelock.ExpansionIsLocked())
					codelock.ExpansionLock();
			}
		}
	}
}
