modded class ActionDeployObject
{
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		if (action_data.m_MainItem.IsInherited(ExplosivesBase) || action_data.m_MainItem.IsInherited(TrapBase) || action_data.m_MainItem.IsKindOf("ExpansionExplosive"))
			action_data.m_Player.m_eAI_LastAggressionTime = GetGame().GetTickTime();  //! Aggro guards in area (if any)
	}
}
