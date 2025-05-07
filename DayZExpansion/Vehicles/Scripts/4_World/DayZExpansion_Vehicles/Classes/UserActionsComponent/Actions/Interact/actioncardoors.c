modded class ActionCarDoors
{
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, action_data.m_Player))
		{
			string selection;
			if (action_data.m_Target.GetObject() && action_data.m_Target.GetComponentIndex() > -1)
				selection = action_data.m_Target.GetObject().GetActionComponentName(action_data.m_Target.GetComponentIndex());

			if (m_IsOpening)
				vehicle.OnDoorOpened(selection);
			else
				vehicle.OnDoorClosed(selection);
		}
	}
}
