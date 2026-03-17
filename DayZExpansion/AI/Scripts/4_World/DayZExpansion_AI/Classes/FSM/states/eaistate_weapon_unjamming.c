class eAIState_Weapon_Unjamming: eAIState
{
	Weapon_Base m_Weapon;
	float m_Time;
	int m_FailedAttempts;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Time = 0;

		if (unit.m_eAI_IsInCover && unit.eAI_GetStance() == DayZPlayerConstants.STANCEIDX_ERECT && unit.m_eAI_StancePreference != DayZPlayerConstants.STANCEIDX_PRONE)
			unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);

		unit.RaiseWeapon(false);
		
		unit.StartActionObject(eAIActionWeaponUnjam, null);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		if (unit.eAI_IsSideStepping())
			unit.eAI_CancelSidestep();

		if (unit.eAI_ShouldGetUp())
			unit.Expansion_GetUp();
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (!unit.GetWeaponManager() || unit.IsUnconscious()) return EXIT;
		
		if (unit.GetWeaponManager().IsRunning())
		{
			m_Time += DeltaTime;
			if (m_Time > 10)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Weapon + ") - timeout");
				return EXIT;
			}
			
			auto target = unit.GetTarget();
			if (target && unit.GetThreatToSelf() >= 0.4)
			{
				if (!unit.m_eAI_IsInCover)
					unit.OverrideTargetPosition(target, true);
				
				if (!unit.eAI_IsSideStepping() && target.m_LOS)
				{
					float distSq = target.GetDistanceSq(true);
					if (distSq <= 9.0)
					{
						float movementDirection;
						if (Math.RandomIntInclusive(0, 1))
							movementDirection = Math.RandomFloat(135, 180);
						else
							movementDirection = Math.RandomFloat(-135, -180);
						unit.eAI_ForceSideStep(Math.RandomFloat(1, 1.5), null, movementDirection);
						unit.OverrideMovementSpeed(false, 0);
					}
				}
			}
			else
			{
				vector position;

				auto group = unit.GetGroup();
				if (group)
				{
					if (group.GetFormationLeader() != unit)
						position = group.GetFormationPosition(unit);
					else
						position = group.GetCurrentWaypoint();
				}
			
				if (position != vector.Zero)
					unit.OverrideTargetPosition(position);
			}
		
			return CONTINUE;
		}
		
		if (unit.GetWeaponManager().CanUnjam(m_Weapon))
		{
			m_FailedAttempts++;
			EXPrint(unit.ToString() + " Weapon_Unjamming - failed (" + m_FailedAttempts + ")");
			m_Weapon.ValidateAndRepair();
		}
		else
		{
			m_FailedAttempts = 0;
		}
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsClimbing() || unit.IsFalling() || unit.IsFighting() || !unit.GetCommand_Move()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (!Class.CastTo(m_Weapon, unit.GetItemInHands())) return eAITransition.FAIL;
		
		if (!unit.GetWeaponManager().CanUnjam(m_Weapon)) return eAITransition.FAIL;
		
		// don't move to the state if the action manager is operating
		if (!unit.GetActionManager() || unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
}
