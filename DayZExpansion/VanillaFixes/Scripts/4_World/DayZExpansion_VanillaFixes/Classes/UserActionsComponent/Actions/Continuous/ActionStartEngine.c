#ifdef SERVER
//! "Failed engine start" sounds not working https://feedback.bistudio.com/T177537
modded class ActionStartEngine
{
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				CarScript car;
				if (Class.CastTo(car, trans))
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(car.Expansion_SendOnBeforeEngineStart, 500);  // slight delay to sync sound with hand animation
				}
			}
		}
	}
}
#endif
