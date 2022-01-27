/**
 * ExpansionActionOpen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpen: ActionInteractBase
{
	ItemBase m_Target;

	void ExpansionActionOpen()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}

	override string GetText()
	{
		if ( m_Target && m_Target.IsLocked() )
		{
			if ( m_Target.IsInherited( ExpansionSafeBase ) )
				return "#STR_EXPANSION_OPEN_LOCKED_SAFE";
			ExpansionWallBase wall;
			if ( (Class.CastTo( wall, m_Target ) && wall.HasGate()) || m_Target.GetType() == "Fence" )
				return "#STR_EXPANSION_OPEN_LOCKED_GATE";
			return "#STR_EXPANSION_OPEN_LOCKED_DOOR";
		}

		return "#open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !Class.CastTo( m_Target, target.GetObject() ) )
			if ( !Class.CastTo( m_Target, target.GetParent() ) )
				return false;

		string selection = m_Target.GetActionComponentName( target.GetComponentIndex() );

		return m_Target.ExpansionCanOpen( player, selection );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		if ( !m_Target || ( m_Target.IsLocked() && !m_Target.IsKnownUser( action_data.m_Player ) ) )
			return;

		string selection = m_Target.GetActionComponentName( action_data.m_Target.GetComponentIndex() );

		if ( m_Target.IsLocked() )
			m_Target.UnlockAndOpen( selection );
		else
			m_Target.Open( selection );
	}
}
