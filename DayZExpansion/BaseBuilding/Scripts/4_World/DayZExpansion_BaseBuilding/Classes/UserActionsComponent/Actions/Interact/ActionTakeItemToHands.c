/**
 * ActionTakeItemToHands.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTakeItemToHands
{
	bool m_ShowAdminPrefix;

	override string GetText()
	{
		string text = super.GetText();

		if (m_ShowAdminPrefix)
			text = "[ADMIN] " + text;

		return text;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;

		return ActionTakeItem.Expansion_TakeItemActionCondition(player, target, item, m_ShowAdminPrefix);
	}
}
