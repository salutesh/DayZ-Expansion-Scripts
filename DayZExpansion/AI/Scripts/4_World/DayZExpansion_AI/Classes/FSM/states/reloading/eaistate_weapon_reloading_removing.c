class eAIState_Weapon_Reloading_Removing: eAIState
{
	int OnUpdateEx(float DeltaTime, int SimulationPrecision, Weapon_Base weapon)
	{
		if (!weapon || unit.GetItemInHands() != weapon)
			return EXIT;  // remove complete
		
		if (unit.GetActionManager().GetRunningAction())
			return CONTINUE;
		
		if (weapon.IsDamageDestroyed() || !unit.eAI_TakeItemToInventoryDropShoulderImpl(weapon))
			unit.eAI_DropItem(weapon);
		
		return CONTINUE;
	}
};
