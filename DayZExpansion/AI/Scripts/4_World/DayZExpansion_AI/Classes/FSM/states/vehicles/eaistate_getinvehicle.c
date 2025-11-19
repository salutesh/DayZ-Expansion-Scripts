class eAIState_GetInVehicle : eAIState_GoToVehicle
{
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		m_Time = 0;

		unit.LookAtDirection("0 0 1");  //! Reset look direction to prevent head snapping to previous direction when sitting
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		auto vehCmd = unit.GetCommand_Vehicle();

		if (!vehCmd && m_Transport && !unit.IsInTransport() && !unit.eAI_GetTransport())
		{
			//! Wait until unit is idle to prevent animation glitches
			if (!unit.Expansion_IsAnimationIdle())
			{
				return CONTINUE;
			}

			unit.Notify_Transport(m_Transport, m_Seat);
		}

		m_Time += DeltaTime;

		if (!vehCmd || vehCmd.IsGettingIn() || m_Time < 1.5)
		{
			return CONTINUE;
		}

		return EXIT;
	}
};
