class ExpansionState_TakeItemToHands: eAIState
{
	ItemBase item;
	float time;
	bool loweringWeapon;
	int requestSwapToMeleeForZombieTime;
	bool allowDrop;

	override void OnEntry(string Event, ExpansionState From)
	{
		#ifdef DIAG
		EXTrace.Print(EXTrace.AI, unit, "TakeItemToHands " + item.ToString());
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
				EXPrint(unit.ToString() + " TakeItemToHands - timeout");
				unit.eAI_Unbug("take to hands");
				time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		
		//! Taking items to hands while raised breaks hands! Wait until lowered
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
		
		ItemBase hands = unit.GetItemInHands();
		if (item && hands != item && !item.Expansion_GetRootPlayerAliveExcluding(unit))
		{
			if (unit.eAI_GetThreatOverride(item))
			{
				//! If we can't take the item, exit
				if (!unit.eAI_TakeItemToInventoryDropShoulderImpl(item))
					return EXIT;
			}
			else if (hands)
			{
				//! Try to put current hand item in inventory (shoulder slot or cargo), drop if not possible
				if (!unit.eAI_TakeItemToInventoryDropShoulderImpl(hands, allowDrop))
				{
					if (allowDrop)
						unit.eAI_DropItem(hands, false, true, false);
					else
					{
						unit.eAI_ThreatOverride(item, true);
						return EXIT;
					}
				}
				else if (unit.m_eAI_LastDroppedItem == item)
					unit.eAI_ThreatOverride(item, false);
				else
					unit.m_eAI_LastDroppedItem = null;
			}
			else if (!unit.eAI_TakeItemToHands(item))
			{
				unit.eAI_Unbug("take to hands");
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
