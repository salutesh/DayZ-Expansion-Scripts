modded class ActionForceBitePurificationTablets
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBitePurificationTablets::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionForceBitePurificationTablets::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBitePurificationTablets::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};