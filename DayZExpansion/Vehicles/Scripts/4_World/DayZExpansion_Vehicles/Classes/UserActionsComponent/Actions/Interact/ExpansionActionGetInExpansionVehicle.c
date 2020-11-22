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
		if ( !target )
			return false;

		ExpansionVehicleBase transport;

		if ( !Class.CastTo( transport, target.GetObject() ) )
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

		int componentIndex = target.GetComponentIndex();

		int crewIdx = transport.CrewPositionIndex( componentIndex );
		if ( crewIdx < 0 )
			return false;

		Human crew = transport.CrewMember( crewIdx );
		if ( crew )
		 	return false;

		if ( !transport.CrewCanGetThrough( crewIdx ) )
		 	return false;

		array<string> selections = new array<string>();
		transport.GetActionComponentNameList( componentIndex, selections );
		 
		for ( int i = 0; i < selections.Count(); i++ )
		{
			if ( transport.CanReachSeatFromDoors(selections[i], player.GetPosition(), 1.0) )
				return true;
		}

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

		if (!action_data.m_Target)
			return;

		ExpansionVehicleBase transport;

		if ( !Class.CastTo( transport, action_data.m_Target.GetObject() ) )
			return;

		int componentIndex = action_data.m_Target.GetComponentIndex();

		int crewIdx = transport.CrewPositionIndex( componentIndex );
		if ( crewIdx < 0 )
			return;
		
		int seat = transport.GetSeatAnimationType( crewIdx );
		ExpansionHumanCommandVehicle vehCommand = action_data.m_Player.StartCommand_ExpansionVehicle( transport, crewIdx, seat );
		if ( vehCommand )
		{
			vehCommand.SetVehicleType( transport.GetAnimInstance() );
			action_data.m_Player.GetItemAccessor().HideItemInHands( true );
			
			GetDayZGame().GetBacklit().OnEnterCar();
			if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().LockInventory( LOCK_FROM_SCRIPT );
			
		}
	}
	
	override void OnEndClient( ActionData action_data )
	{
		if ( action_data.m_Player.GetInventory() ) 
			action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	
	override void OnEndServer( ActionData action_data )
	{
		if ( action_data.m_Player.GetInventory() ) 
			action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
}