/**
 * ExpansionActionClose.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionClose: ActionInteractBase
{
	ItemBase m_Target;

	void ExpansionActionClose()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}

	override string GetText()
	{
		string text;

		//! If CodelockActionsAnywhere is ON, then "Close and lock" will be provided by ExpansionActionEnterCodeLock
		//! If it is OFF, then it will be provided by this action
		if ( m_Target && m_Target.HasCode() && !GetExpansionSettings().GetBaseBuilding().CodelockActionsAnywhere )
			text = "#STR_EXPANSION_BB_CODE_CLOSE_LOCK";
		else
			text = "#close";

		if (GetPermissionsManager().IsAdminToolsToggledOn())
			text = "[ADMIN] " + text;

		return text;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		if ( !Class.CastTo( m_Target, target.GetObject() ) )
			if ( !Class.CastTo( m_Target, target.GetParent() ) )
				return false;

		string selection = m_Target.GetActionComponentName( target.GetComponentIndex() );

		return m_Target.ExpansionCanClose( player, selection );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ItemBase tgtItem;
		if ( !Class.CastTo( tgtItem, action_data.m_Target.GetObject() ) )
			if ( !Class.CastTo( tgtItem, action_data.m_Target.GetParent() ) )
				return;

		string selection = tgtItem.GetActionComponentName( action_data.m_Target.GetComponentIndex() );

		//! If CodelockActionsAnywhere is ON, then "Close and lock" will be provided by ExpansionActionEnterCodeLock
		//! If it is OFF, then it will be provided by this action
		if ( tgtItem && tgtItem.HasCode() && !GetExpansionSettings().GetBaseBuilding().CodelockActionsAnywhere )
			tgtItem.CloseAndLock( selection );
		else
			tgtItem.Expansion_Close( selection );
	}
}