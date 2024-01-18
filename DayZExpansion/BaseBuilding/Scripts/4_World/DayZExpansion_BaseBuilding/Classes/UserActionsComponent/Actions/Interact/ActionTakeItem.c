/**
 * ActionTakeItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTakeItem
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

		return Expansion_TakeItemActionCondition(player, target, item, m_ShowAdminPrefix);
	}

	static bool Expansion_TakeItemActionCondition(PlayerBase player, ActionTarget target, ItemBase item, out bool showAdminPrefix)
	{
		//! Disallow taking codelock if it is attached and has code
		//! to prevent taking it accidentally
		//! (have to go into inventory and drag from slot to remove)
		ExpansionCodeLock codelock = ExpansionCodeLock.Cast( target.GetObject() );
		if ( target.GetParent() && codelock && codelock.HasCode() )
			return false;

		bool isInsideOwnTerritory;
		if (!player.Expansion_CanDismantleFlag(isInsideOwnTerritory))
			return false;

	#ifndef SERVER
		if (isInsideOwnTerritory && GetPermissionsManager().IsAdminToolsToggledOn())
			showAdminPrefix = true;
		else
			showAdminPrefix = false;
	#endif

		return true;
	}
}
