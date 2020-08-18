modded class ActionForceBiteTetracyclineAntibiotics
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBiteTetracyclineAntibiotics::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionForceBiteTetracyclineAntibiotics::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBiteTetracyclineAntibiotics::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};