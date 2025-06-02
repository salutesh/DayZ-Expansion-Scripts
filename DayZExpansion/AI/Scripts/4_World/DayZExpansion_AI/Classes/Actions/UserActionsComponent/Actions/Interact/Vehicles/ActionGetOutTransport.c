modded class ActionGetOutTransport
{
	override void ProcessGetOutTransportActionData(Transport veh, GetOutTransportActionData got_action_data)
	{
		super.ProcessGetOutTransportActionData(veh, got_action_data);

		//! @note 2 km/h is the speed damage threshold in EntityAI::RegisterTransportHit
		if (got_action_data.m_Speed < 2)
			return;

		//! As long as driver is in seat, we don't need to target vehicle since driver will be targeted anyway if a threat
		if (veh.CrewMemberIndex(got_action_data.m_Player) != DayZPlayerConstants.VEHICLESEAT_DRIVER)
			return;

		//! If player is targeted while jumping out vehicle, target vehicle
		auto playerTargetInfo = got_action_data.m_Player.GetTargetInformation();
		if (!playerTargetInfo.IsTracked())
			return;

		CarScript vehicle;
		if (!Class.CastTo(vehicle, veh))
			return;
	
		auto vehicleTargetInfo = vehicle.GetTargetInformation();
		foreach (eAIBase ai, eAITarget target: playerTargetInfo.m_Targets)
		{
			if (!ai)
				continue;

			if (!target.m_IsTracked)
				continue;
			
			EXPrint(ai.ToString() + " (group " + ai.GetGroup().GetID() + ") targeting " + vehicle);
			vehicleTargetInfo.AddAI(ai, 120000);
		}
	}
}
