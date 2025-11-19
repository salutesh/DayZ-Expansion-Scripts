class eAIState_Weapon_Reloading_Start: eAIState
{
	void OnEntryEx(string Event, ExpansionState From, out int lastAttemptTime)
	{
		lastAttemptTime = g_Game.GetTime();

		unit.OverrideMovementSpeed(false, 0);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		return EXIT;
	}
};
