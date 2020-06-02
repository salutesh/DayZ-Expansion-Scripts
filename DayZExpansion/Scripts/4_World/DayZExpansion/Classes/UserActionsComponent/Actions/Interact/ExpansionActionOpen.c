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
	protected ItemBase m_ExBB;

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
		if (!player)
			return false;
		
		m_ExBB = ItemBase.Cast( target.GetObject() );

		if ( !m_ExBB && !Class.CastTo( m_ExBB, target.GetParent() ) )
			return false;

		if ( m_ExBB )
		{
			string selection = m_ExBB.GetActionComponentName( target.GetComponentIndex() );
				
			if ( m_ExBB && m_ExBB.CanOpen( player, selection ) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{		
		string selection = m_ExBB.GetActionComponentName( action_data.m_Target.GetComponentIndex() );
		m_ExBB.Open( selection );
	}
}