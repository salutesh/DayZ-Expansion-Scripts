modded class ActionUnpin
{
	override void OnExecute(ActionData action_data)
	{
		super.OnExecute(action_data);

		if (!action_data.m_MainItem.IsInherited(SmokeGrenadeBase))
			action_data.m_Player.m_eAI_LastAggressionTime = GetGame().GetTickTime();  //! Aggro guards in area (if any)
	}
}
