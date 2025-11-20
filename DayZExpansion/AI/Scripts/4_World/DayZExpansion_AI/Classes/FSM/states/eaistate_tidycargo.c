class eAISearchInventoyCommandActionCallback: HumanCommandActionCallback
{
}

class eAIState_TidyCargo: eAIState
{
	float m_Time;
	float m_MinTime;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Time = 0;
		m_MinTime = 0;
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		auto callback = unit.GetCommandModifier_Action();
		if (callback)
			callback.Cancel();
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		m_Time += DeltaTime;

		if (unit.m_eAI_CargosToTidy.Count() > 0)
		{
			Clothing clothing = unit.m_eAI_CargosToTidy[0];

			if (clothing)
			{
				if (!ExpansionInventoryTools.IsTidyInProgress(clothing))
				{
					if (ExpansionInventoryTools.Tidy(clothing))
					{
						if (m_MinTime == 0)
							unit.AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_SEARCHINVENTORY, eAISearchInventoyCommandActionCallback);

						if (m_MinTime < 3.0)
							m_MinTime += Math.RandomFloat(1, 1.5);

					#ifdef DIAG_DEVELOPER
						string msg = unit.ToString() + " tidied " + clothing;
						EXPrint(msg);
						ExpansionStatic.MessageNearPlayers(unit.GetPosition(), 100, msg);
					#endif
					}
				}

				clothing.m_eAI_CargoChangedCount = 0;
			}

			unit.m_eAI_CargosToTidy.Remove(0);
		}

		if (m_Time < m_MinTime && unit.GetThreatToSelf() <= 0.2)
			return CONTINUE;

		return EXIT;
	}

	int Guard()
	{
		if (unit.m_eAI_CargosToTidy.Count() == 0) return eAITransition.FAIL;

		if (unit.m_eAI_IsInventoryVisible) return eAITransition.FAIL;
		
		if (unit.IsFighting()) return eAITransition.FAIL;
		if (unit.IsRaised()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		if (unit.IsClimbing()) return eAITransition.FAIL;
		if (unit.IsFalling()) return eAITransition.FAIL;
		
		if (!unit.GetCommand_Move()) return eAITransition.FAIL;

		if (unit.m_eAI_DangerousAreaCount > 0 && unit.m_eAI_IsInDangerByArea)
			return eAITransition.FAIL;

		if (unit.GetThreatToSelf() > 0.2) return eAITransition.FAIL;

		return eAITransition.SUCCESS;
	}
}
