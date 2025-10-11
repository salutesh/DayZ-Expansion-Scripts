class eAIState_Weapon_Reloading_Fail: eAIState
{
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		unit.RaiseWeapon(false);
		
		m_Time = 0;
	}

	int OnUpdateEx(float DeltaTime, int SimulationPrecision, Weapon_Base weapon)
	{
		m_Time += DeltaTime;
		
		// waiting for the weapon to be lowered
		if (weapon && m_Time < 0.5)
			return CONTINUE;
		
		return EXIT;
	}
};
