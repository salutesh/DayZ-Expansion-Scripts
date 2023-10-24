/**
 * ExpansionActionDismissAI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
modded class ExpansionActionDismissAI
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		eAIBase ai;
		if (!Class.CastTo(ai, target.GetObject()) || ai.Expansion_IsQuestVIP())
			return false;

		return true;
	}
};
#endif
