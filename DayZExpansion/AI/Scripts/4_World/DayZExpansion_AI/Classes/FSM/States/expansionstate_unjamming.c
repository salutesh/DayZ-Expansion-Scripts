class ExpansionState_Unjamming: eAIState
{
	Weapon_Base weapon;
	float time;
	int failed_attempts;

	override void OnEntry(string Event, ExpansionState From)
	{
		time = 0;
		unit.RaiseWeapon(false);
		
		unit.StartActionObject(eAIActionWeaponUnjam, null);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.OverrideMovementDirection(false, 0);
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
			if (target && target.GetDistanceSq(true) <= 2.25)
			{
				unit.OverrideMovementDirection(true, -180);
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
