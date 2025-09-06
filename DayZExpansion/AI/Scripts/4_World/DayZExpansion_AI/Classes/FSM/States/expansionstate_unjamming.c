class ExpansionState_Unjamming: eAIState
{
	Weapon_Base weapon;
	float time;
	int failed_attempts;

	override void OnEntry(string Event, ExpansionState From)
	{
		time = 0;

		if (unit.m_eAI_IsInCover && unit.eAI_GetStance() == DayZPlayerConstants.STANCEIDX_ERECT && unit.m_eAI_StancePreference != DayZPlayerConstants.STANCEIDX_PRONE)
			unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);

		unit.RaiseWeapon(false);
		
		unit.StartActionObject(eAIActionWeaponUnjam, null);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.OverrideMovementDirection(false, 0);

		if (unit.eAI_ShouldGetUp())
			unit.Expansion_GetUp();
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (!unit.GetWeaponManager() || unit.IsUnconscious()) return EXIT;
		
		if (unit.GetWeaponManager().IsRunning())
		{
			time += DeltaTime;
			if (time > 10)  //! Looks like something went terribly wrong
			{
				EXPrint(unit.ToString() + " Weapon_Unjamming - timeout");
				unit.eAI_Unbug("unjam");
				return EXIT;
			}
			
			auto target = unit.GetTarget();
			if (target && unit.GetThreatToSelf() >= 0.4)
			{
				if (!unit.m_eAI_IsInCover)
					unit.OverrideTargetPosition(target, true);
				
				if (target.GetDistanceSq(true) <= 2.25)
					unit.OverrideMovementDirection(true, -180);
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
		
		if (unit.GetWeaponManager().CanUnjam(weapon))
		{
			failed_attempts++;
			EXPrint(unit.ToString() + " Weapon_Unjamming - failed (" + failed_attempts + ")");
			weapon.ValidateAndRepair();
		}
		else
		{
			failed_attempts = 0;
		}
		
		return EXIT;
	}
}
