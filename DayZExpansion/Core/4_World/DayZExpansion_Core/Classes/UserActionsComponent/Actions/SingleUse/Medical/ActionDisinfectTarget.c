modded class ActionDisinfectTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionDisinfectTarget::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionDisinfectTarget::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionDisinfectTarget::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};