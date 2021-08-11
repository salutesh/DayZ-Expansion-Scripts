modded class ActionForceBiteCharcoalTablets
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBiteCharcoalTablets::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionForceBiteCharcoalTablets::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBiteCharcoalTablets::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};