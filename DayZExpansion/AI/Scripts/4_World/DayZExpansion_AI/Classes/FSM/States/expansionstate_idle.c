class ExpansionState_Idle: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		auto hands = unit.GetItemInHands();
		if (hands && hands.HasEnergyManager() && hands.GetCompEM().IsWorking() && hands.GetCompEM().CanSwitchOff())
		{
			hands.GetCompEM().SwitchOff();
		}
		
		auto cmd = unit.GetCommand_MoveAI();
		if (!unit.m_eAI_CurrentCoverObject || !cmd || unit.m_eAI_CommandMove.GetWaypoint() != unit.m_eAI_CurrentCoverPosition)
		{
			unit.OverrideTargetPosition(unit.GetPosition() + unit.GetDirection() * unit.Expansion_GetMovementSpeed() * 0.333333);
			unit.OverrideMovementDirection(false, 0);
			unit.OverrideMovementSpeed(true, 0);
			
			if (unit.eAI_ShouldGetUp())
				unit.Expansion_GetUp();
		}
		else
		{
			unit.m_eAI_TargetPositionIsFinal = true;
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		return EXIT;
	}
}
