/**
 * ExpansionActionOpen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpen: ActionInteractBase
{
	void ExpansionActionOpen()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
			return false;

		ItemBase tgt;
		if ( !Class.CastTo( tgt, target.GetObject() ) )
			if ( !Class.CastTo( tgt, target.GetParent() ) )
				return false;

		string selection = tgt.GetActionComponentName( target.GetComponentIndex() );
		
		return tgt.CanOpen( player, selection );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ItemBase tgt;
		if ( !Class.CastTo( tgt, action_data.m_Target.GetObject() ) )
			if ( !Class.CastTo( tgt, action_data.m_Target.GetParent() ) )
				return;

		string selection = tgt.GetActionComponentName( action_data.m_Target.GetComponentIndex() );
		
		tgt.Open( selection );
	}
}