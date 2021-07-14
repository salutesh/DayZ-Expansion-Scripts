modded class ActionForceBiteVitaminBottle
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBiteVitaminBottle::ActionCondition Start");
		#endif
		if (player)
		{
			if (player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionForceBiteVitaminBottle::ActionCondition End");
				#endif

				return false;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionForceBiteVitaminBottle::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
};