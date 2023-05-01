modded class ActionGetOutTransport
{
	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		auto got_action_data = GetOutTransportActionData.Cast(action_data);
		if (!got_action_data.m_WasJumpingOut)
			return;

		//! If player is targeted while jumping out vehicle, target vehicle
		auto playerTargetInfo = action_data.m_Player.GetTargetInformation();
		if (!playerTargetInfo.IsTargetted())
			return;

		auto vehCmd = action_data.m_Player.GetCommand_Vehicle();
		CarScript vehicle;
		if (!Class.CastTo(vehicle, vehCmd.GetTransport()))
			return;
	
		auto vehicleTargetInfo = vehicle.GetTargetInformation();
		foreach (int groupId, eAITarget target: playerTargetInfo.m_Groups)
		{
			foreach (eAIBase ai: target.ai_list)
			{
				if ( !ai )
					continue;
				
				EXPrint(ai.ToString() + " (group " + groupId + ") targeting " + vehicle);
				vehicleTargetInfo.AddAI(ai, 120000);
			}
		}
	}
}
