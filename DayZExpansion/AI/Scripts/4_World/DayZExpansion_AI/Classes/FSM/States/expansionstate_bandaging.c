class ExpansionState_Bandaging_Self: eAIState
{
	int last_bandage_attempt_time;
	ItemBase bandage;
	float timeout;
	float time;
	int bleedingSourceCount;

	override void OnEntry(string Event, ExpansionState From)
	{
		last_bandage_attempt_time = GetGame().GetTime();
		bleedingSourceCount = unit.GetBleedingSourceCount();
		time = 0;
		float effectivity = bandage.GetBandagingEffectivity();
		if (effectivity > 0)
			timeout = UATimeSpent.BANDAGE / effectivity + 8.0;
		else
			timeout = 16.0;
		unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		//! If we had dropped another item to take bandage to hands, restore item threat
		if (unit.m_eAI_LastDroppedItem && !unit.m_eAI_LastDroppedItem.Expansion_CanBeUsedToBandage())
			unit.eAI_ThreatOverride(unit.m_eAI_LastDroppedItem, false);
		
		if (bandage)
		{
			if (bandage.IsDamageDestroyed() || !unit.eAI_TakeItemToInventory(bandage))
			{
				unit.eAI_DropItem(bandage);
			}
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (unit.GetActionManager().GetRunningAction())
		{
			if (unit.GetBleedingSourceCount() != bleedingSourceCount)
			{
				bleedingSourceCount = unit.GetBleedingSourceCount();
				time = 0;
			}

			time += DeltaTime;
			if (time > timeout)  //! Looks like something went terribly wrong
			{
				EXPrint(unit.ToString() + " Bandaging_Self - timeout");
				unit.eAI_Unbug("bandage");
				time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		else if (unit.eAI_ShouldBandage())
		{
			if (bandage)
			{
				if (unit.IsBleeding() && !bandage.IsDamageDestroyed())
				{
					last_bandage_attempt_time = GetGame().GetTime();
					time = 0;
					
					unit.StartActionObject(ActionBandageSelf, null);
					
					return CONTINUE;
				}
			}
		}
		
		if (unit.Expansion_GetUp(true))
			return CONTINUE;
		
		return EXIT;
	}
}
