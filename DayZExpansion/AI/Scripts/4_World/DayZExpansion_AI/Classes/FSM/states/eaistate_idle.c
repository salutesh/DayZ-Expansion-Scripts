class eAIState_Idle: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		EntityAI hands = unit.GetItemInHands();
		if (hands && hands.HasEnergyManager() && hands.GetCompEM().IsWorking() && hands.GetCompEM().CanSwitchOff())
		{
			if (hands.GetType() == unit.m_eAI_TypeSwitchedOnDuringCombat)
			{
				hands.GetCompEM().SwitchOff();
				unit.m_eAI_TypeSwitchedOnDuringCombat = "";
			}
		}
		
		auto cmd = unit.GetCommand_Move();
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
