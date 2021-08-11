modded class ActionInjectEpinephrineTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionInjectEpinephrineTarget::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionInjectEpinephrineTarget::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionInjectEpinephrineTarget::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};