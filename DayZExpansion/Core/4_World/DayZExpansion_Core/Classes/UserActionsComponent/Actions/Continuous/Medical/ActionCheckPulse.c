#ifdef EXPANSIONMODMARKET
modded class ActionCheckPulse
{
	#ifdef ENFUSION_AI_PROJECT
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionCheckPulse::ActionCondition Start");
		#endif

		if (player && target.GetObject().IsInherited(ExpansionTraderAIBase))
			return false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ActionCheckPulse::ActionCondition End");
		#endif

		return super.ActionCondition(player, target, item);
	}
	#endif
};
#endif