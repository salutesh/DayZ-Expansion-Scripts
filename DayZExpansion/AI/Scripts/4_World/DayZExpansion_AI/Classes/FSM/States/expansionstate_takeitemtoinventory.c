class ExpansionState_TakeItemToInventory: eAIState
{
	ItemBase item;
	float time;
	bool loweringWeapon;

	override void OnEntry(string Event, ExpansionState From)
	{
		#ifdef DIAG
		EXTrace.Print(EXTrace.AI, unit, "TakeItemToInventory " + item.ToString());
		#endif
		time = 0;
		loweringWeapon = false;
		if (unit.GetEmoteManager().IsEmotePlaying())
			unit.GetEmoteManager().ServerRequestEmoteCancel();
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.GetEmoteManager().IsEmotePlaying() || unit.GetActionManager().GetRunningAction() || unit.GetWeaponManager().IsRunning() || !unit.GetCommand_MoveAI())
		{
			time += DeltaTime;
			if (time > 10)  //! Looks like something went terribly wrong
			{
				EXPrint(unit.ToString() + " TakeItemToInventory - timeout");
				unit.eAI_Unbug("take to inventory");
				time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		
		//! Taking items while raised breaks hands! Wait until lowered
		if (unit.IsRaised())
		{
			loweringWeapon = true;
			unit.RaiseWeapon(false);
			time = 0;
		}
		
		if (loweringWeapon)
		{
			if (time < 0.5)
			{
				time += DeltaTime;
				return CONTINUE;
			}
			
			time = 0;
			loweringWeapon = false;
		}
		
		if (item && !item.Expansion_GetRootPlayerAliveExcluding(null))
		{
			if (unit.eAI_GetThreatOverride(item))
				return EXIT;
			
			if (!unit.eAI_TakeItemToInventory(item))
			{
				unit.eAI_Unbug("take to inventory");
				return EXIT;
			}
		}
		
		if (time < 0.5)
		{
			time += DeltaTime;
			return CONTINUE;
		}
		
		if (unit.GetActionManager().GetRunningAction())
			return CONTINUE;
		
		return EXIT;
	}
}
