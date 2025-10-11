class eAIState_OpenVehicleDoor : eAIState_GoToVehicle
{
	string m_Source;
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		bool hasDoor;

		ExpansionFSMHelper.DoorAnimationSource(m_Entity, m_Seat, hasDoor, m_Source);
		if (hasDoor)
		{
			m_Entity.SetAnimationPhase(m_Source, 1.0);
		}

		m_Time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		m_Time += DeltaTime;

		if (m_Time > 1.0 || !m_Source || m_Entity.GetAnimationPhase(m_Source) > 0.95)
		{
			return EXIT;
		}

		return CONTINUE;
	}
};
