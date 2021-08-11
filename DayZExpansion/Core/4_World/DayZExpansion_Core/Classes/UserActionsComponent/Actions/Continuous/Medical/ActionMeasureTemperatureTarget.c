modded class ActionMeasureTemperatureTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionMeasureTemperatureTarget::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionMeasureTemperatureTarget::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionMeasureTemperatureTarget::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};