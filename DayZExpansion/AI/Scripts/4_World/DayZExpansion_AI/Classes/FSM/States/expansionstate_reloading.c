class ExpansionState_Reloading: eAIState
{
	Weapon_Base weapon;
	Magazine magazine;
	float time;
	int failed_attempts;

	override void OnEntry(string Event, ExpansionState From)
	{
		time = 0;

		if (unit.m_eAI_IsInCover && unit.eAI_GetStance() == DayZPlayerConstants.STANCEIDX_ERECT && unit.m_eAI_StancePreference != DayZPlayerConstants.STANCEIDX_PRONE)
			unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);

		unit.ReloadWeaponAI(weapon, magazine);
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
		
		if (!weapon) return EXIT;
		
		if (unit.GetWeaponManager().IsRunning() || unit.GetActionManager().GetRunningAction())
		{
			time += DeltaTime;
			if (time > 12)  //! Looks like something went terribly wrong
			{
				EXPrint(unit.ToString() + " Weapon_Reloading - Reloading - timeout");
				unit.eAI_Unbug("reload");
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
					else if (distSq < 100.0 && weapon.IsInherited(BoltActionRifle_Base) && weapon.Expansion_HasAmmo())
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
		
		if (weapon.IsDamageDestroyed())
		{
			unit.eAI_DropItem(weapon);
		}
		else if (weapon.IsChamberEmpty(weapon.GetCurrentMuzzle()))
		{
			failed_attempts++;
			EXPrint(unit.ToString() + " Weapon_Reloading - Reloading - failed (" + failed_attempts + ")");
			if (failed_attempts >= 3)
			{
				//! Fuck the gun
			#ifdef DIAG_DEVELOPER
				string msg = unit.ToString() + " Reloading failed after " + failed_attempts + " attempts - dropping " + weapon;
				EXPrint(msg);
				ExpansionStatic.MessageNearPlayers(unit.GetPosition(), 100, msg);
			#endif
				unit.eAI_ThreatOverride(weapon, true);
				unit.eAI_DropItem(weapon);
				failed_attempts = 0;
			}
			else
			{
				weapon.ValidateAndRepair();
			}
		}
		else
		{
			failed_attempts = 0;
		}
		
		return EXIT;
	}
};
