class eAIState_Weapon_Reloading_Reloading: eAIState
{
	Weapon_Base m_Weapon;
	Magazine m_Magazine;
	float m_Time;
	int m_FailedAttempts;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Time = 0;

		if (unit.m_eAI_IsInCover && unit.eAI_GetStance() == DayZPlayerConstants.STANCEIDX_ERECT && unit.m_eAI_StancePreference != DayZPlayerConstants.STANCEIDX_PRONE)
			unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);

		unit.ReloadWeaponAI(m_Weapon, m_Magazine);
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
		
		if (!m_Weapon) return EXIT;
		
		if (unit.GetWeaponManager().IsRunning() || unit.GetActionManager().GetRunningAction())
		{
			m_Time += DeltaTime;
			if (m_Time > 12)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Weapon + ", " + m_Magazine + ") - timeout");
				return EXIT;
			}
			
			vector position;
			
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
					//! If we have a bolt action rifle with ammo in internal or attached mag, circle around player while cycling action
					else if (distSq < 100.0 && m_Weapon.IsInherited(BoltActionRifle_Base) && m_Weapon.Expansion_HasAmmo())
					{
						unit.eAI_ForceSideStep(Math.RandomFloat(1, 1.5), null, 0.0, false);
						unit.OverrideMovementSpeed(false, 0);
					}
				}
			}
			else
			{
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
		
		if (m_Weapon.IsDamageDestroyed())
		{
			unit.eAI_DropItem(m_Weapon);
		}
		else if (m_Weapon.IsChamberEmpty(m_Weapon.GetCurrentMuzzle()))
		{
			m_FailedAttempts++;
			EXPrint(unit.ToString() + " Weapon_Reloading - Reloading - failed (" + m_FailedAttempts + ")");
			if (m_FailedAttempts >= 3)
			{
				//! Fuck the gun
			#ifdef DIAG_DEVELOPER
				string msg = unit.ToString() + " Reloading failed after " + m_FailedAttempts + " attempts - dropping " + m_Weapon;
				EXPrint(msg);
				ExpansionStatic.MessageNearPlayers(unit.GetPosition(), 100, msg);
			#endif
				unit.eAI_ThreatOverride(m_Weapon, true);
				unit.eAI_DropItem(m_Weapon);
				m_FailedAttempts = 0;
			}
			else
			{
				m_Weapon.ValidateAndRepair();
			}
		}
		else
		{
			m_FailedAttempts = 0;
		}
		
		return EXIT;
	}

	int GuardEx(Weapon_Base weapon, inout Magazine magazine)
	{
		if (!weapon || weapon.IsDamageDestroyed())
			return eAITransition.FAIL;
		
		if (!magazine && !weapon.Expansion_HasAmmo(magazine))
		{
			if (!unit.eAI_HasAmmoForFirearm(weapon, magazine))
				return eAITransition.FAIL;
		}
		
		#ifdef EXTRACE_DIAG
		if (!magazine)
			EXTrace.Start0(EXTrace.AI, this, "Reloading " + weapon + " from internal mag");
		else
			EXTrace.Start0(EXTrace.AI, this, "Reloading " + weapon + " from mag " + magazine);
		#endif
		
		m_Weapon = weapon;
		m_Magazine = magazine;
		
		return eAITransition.SUCCESS;
	}
};
