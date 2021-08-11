modded class ActionBandageTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionBandageTarget::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionBandageTarget::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionBandageTarget::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};
