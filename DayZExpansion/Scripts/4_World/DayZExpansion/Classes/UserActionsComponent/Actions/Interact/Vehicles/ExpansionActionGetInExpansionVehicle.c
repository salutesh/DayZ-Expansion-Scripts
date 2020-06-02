/**
 * ExpansionActionGetInExpansionVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionGetInExpansionVehicle
 * @brief		
 **/
class ExpansionActionGetInExpansionVehicle: ActionInteractBase
{
	private ExpansionVehicleScript m_Transport;
	private int m_CrewIdx;

	void ExpansionActionGetInExpansionVehicle()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = "GetInDriver";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#get_in_vehicle";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Transport = NULL;
		m_CrewIdx = -1;

		if ( !target )
			return false;

		if ( !Class.CastTo( m_Transport, target.GetObject() ) )
			return false;

		if ( player.GetCommand_Vehicle() )
			return false;

		if ( player.GetCommand_Ladder() )
			return false;

		if ( player.GetCommand_Swim() )
			return false;

		if ( player.GetCommand_Climb() )
			return false;

		if ( player.GetCommand_ExpansionVehicle() )
			return false;

		m_CrewIdx = 0;

		int componentIndex = target.GetComponentIndex();

		m_CrewIdx = m_Transport.CrewPositionIndexSelection( componentIndex );

		// That's why you cant get in mh6
		if ( m_CrewIdx < 0 )
			return false;

		// Human crew = m_Transport.CrewMember( m_CrewIdx );
		// if ( crew )
		// 	return false;
		// 
		if ( !m_Transport.CrewCanGetThrough( m_CrewIdx ) )
		 	return false;
// 
		// array<string> selections = new array<string>();
// 
		// m_Transport.GetActionComponentNameList( componentIndex, selections );
		// 
		// for ( int i = 0; i < selections.Count(); i++ )
		// {
		// 	// if ( m_Transport.CanReachSeatFromDoors(selections[i], player.GetPosition(), 1.0) )
		// 		return true;
		// }

		return true;
	}

	override bool CanBeUsedInRestrain()
	{
		return true;
	}
	
	override void OnUpdate( ActionData action_data )
	{
		if ( action_data.m_State == UA_START )
		{
			End( action_data );
		}
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		int seat = m_Transport.GetSeatAnimationType( m_CrewIdx );
		action_data.m_Player.StartCommand_ExpansionVehicle( m_Transport, m_CrewIdx, seat );
	}
}