class eAIState_Dormant: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
	#ifdef DIAG_DEVELOPER
		EXPrint(unit, "-> dormant " + typename.EnumToString(eAIWaypointBehavior, unit.GetGroup().GetWaypointBehaviour()));
	#endif
		unit.DisableSimulation(true);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
	#ifdef DIAG_DEVELOPER
		EXPrint(unit, "-> wake up " + typename.EnumToString(eAIWaypointBehavior, unit.GetGroup().GetWaypointBehaviour()));
	#endif
		unit.DisableSimulation(false);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (Guard() == eAITransition.SUCCESS)
			return CONTINUE;

		return EXIT;
	}

	int Guard()
	{
		if (unit.m_eAI_IsFightingFSM)
		{
			//Print("Fighting");
			return eAITransition.FAIL;
		}

		if (unit.m_eAI_AcuteDangerTargetCount > 0)
		{
			//Print("Acute danger");
			return eAITransition.FAIL;
		}

		if (unit.m_eAI_AcuteDangerPlayerTargetCount > 0)
		{
			//Print("Acute danger player");
			return eAITransition.FAIL;
		}

		if (g_Game.GetTickTime() - unit.m_eAI_LastHitTime <= 10)
		{
			//Print("Hit");
			return eAITransition.FAIL;
		}

		if (unit.m_eAI_PlayersWithinVisibilityDistanceLimit.Count() > 0)
			return eAITransition.FAIL;

		if (unit.Expansion_GetMovementSpeed() > 0)
		{
			//Print("Moving");
			return eAITransition.FAIL;
		}

		eAIGroup group = unit.GetGroup();

		if ((group.GetWaypoints().Count() > 1 && unit.m_eAI_CommandMove.GetWaypointDistance2DSq() == 0) || !unit.m_eAI_CommandMove.HasReachedWaypoint(false))
		{
			//Print("Moving to waypoint");
			return eAITransition.FAIL;
		}

		if (!group.GetLeader().IsAI())
			return eAITransition.FAIL;

		eAIWaypointBehavior bhv = group.GetWaypointBehaviour();
		if (bhv == eAIWaypointBehavior.ROAMING || bhv == eAIWaypointBehavior.ROAMING_LOCAL)
			return eAITransition.FAIL;

		return eAITransition.SUCCESS;
	}
}
