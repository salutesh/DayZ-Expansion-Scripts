/**
 * ExpansionActionRestoreContents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRestoreContents: ActionInteractBase
{
	void ExpansionActionRestoreContents()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
		m_Text = "#STR_EXPANSION_ACTION_RESTORE_CONTENTS";
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

		//! Disallow restore contents action on openable containers if closed (useful for debugging if automatic restore on open failed)
		if ((container.ExpansionIsOpenable() || container.IsNonExpansionOpenable()) && !container.IsOpen())
			return false;

		//! If a codelock is attached, it handles storing/restoring contents on lock/unlock
		ItemBase codelock = container.Expansion_GetCodeLock();
		if (codelock && codelock.ExpansionIsLocked())
			return false;

		if (!container.Expansion_HasEntityStorage() || !container.Expansion_IsEmptyIgnoringLockedSlots())
			return false;

		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ItemBase container;
		if (!Class.CastTo(container, action_data.m_Target.GetParentOrObject()))
			return;

		string errorMessage;
		if (!container.Expansion_RestoreContentsEx(action_data.m_Player, errorMessage))
			ExpansionNotification("Virtual Storage", string.Format("Could not restore contents from virtual storage: %1", errorMessage)).Error(action_data.m_Player.GetIdentity());
	}
}
