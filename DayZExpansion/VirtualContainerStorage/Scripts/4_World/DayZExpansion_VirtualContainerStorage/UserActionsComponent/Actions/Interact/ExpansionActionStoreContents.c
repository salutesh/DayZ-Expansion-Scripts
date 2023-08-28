/**
 * ExpansionActionStoreContents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionStoreContents: ActionInteractBase
{
	void ExpansionActionStoreContents()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
		m_Text = "#STR_EXPANSION_ACTION_STORE_CONTENTS";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase container;
		if (!Class.CastTo(container, target.GetParentOrObject()))
			return false;

		//! Disallow store contents action on openable containers if open (will store automatically when closed if enabled)
		if ((container.ExpansionIsOpenable() || container.IsNonExpansionOpenable()) && container.IsOpen())
			return false;

		if (!container.Expansion_CanUseVirtualStorage())
			return false;

		if (container.Expansion_HasEntityStorage() || container.Expansion_IsEmptyIgnoringLockedSlots())
			return false;

		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ItemBase container;
		if (!Class.CastTo(container, action_data.m_Target.GetParentOrObject()))
			return;

		if (!container.Expansion_StoreContents())
			ExpansionNotification("STR_EXPANSION_ERROR_TITLE", "Could not save contents to virtual storage").Error(action_data.m_Player.GetIdentity());
	}
}
