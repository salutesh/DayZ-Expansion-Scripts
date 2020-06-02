/**
 * ExpansionActionGetInTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionGetInTransport
 * @brief		
 **/
class ExpansionActionGetInTransport: ActionBase
{
	private Transport m_Transport;

	private int m_CrewIdx;

	void ExpansionActionGetInTransport()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
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

		int componentIndex = target.GetComponentIndex();

		m_CrewIdx = m_Transport.CrewPositionIndex( componentIndex );
		if ( m_CrewIdx < 0 )
			return false;

		Human crew = m_Transport.CrewMember( m_CrewIdx );
		if ( crew )
			return false;
		
		if ( !m_Transport.CrewCanGetThrough( m_CrewIdx ) )
			return false;

		array<string> selections = new array<string>();

		m_Transport.GetActionComponentNameList( componentIndex, selections );
		
		for ( int i = 0; i < selections.Count(); i++ )
		{
			if ( m_Transport.CanReachSeatFromDoors(selections[i], player.GetPosition(), 1.0) )
			{
				CarScript car = CarScript.Cast(m_Transport);
				
				if ( car.DoorCount() <= 0 )
				{
					return !car.IsLocked();
				} else
				{
					return true;
				}
			}
		}

		return false;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.AttachmentDebugPrint( "ExpansionActionGetInTransport::Start" );

		IEntity parent = action_data.m_Player.GetParent();

		if ( parent )
		{
			action_data.m_Player.UnlinkFromLocalSpace();

			parent.Update();
		}
		
		action_data.m_Player.AttachmentDebugPrint( "ExpansionActionGetInTransport::StartCommand" );

		int seat = m_Transport.GetSeatAnimationType( m_CrewIdx );
		HumanCommandVehicle vehCommand = action_data.m_Player.StartCommand_Vehicle( m_Transport, m_CrewIdx, seat );
		if ( vehCommand )
		{
			vehCommand.SetVehicleType( m_Transport.GetAnimInstance() );
			action_data.m_Player.GetItemAccessor().HideItemInHands( true );
			
			GetDayZGame().GetBacklit().OnEnterCar();	
			if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);

			//! if ( action_data.m_Player.GetInventory() ) 
			//	action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}

		CarScript car = CarScript.Cast( m_Transport );

		if ( IsMissionClient() )
		{
			bool isCar = false;
			bool isBoat = false;
			bool isPlane = false;

			if ( car )
			{
				isCar = car.IsCar();
				isBoat = car.IsBoat();
				isPlane = car.IsPlane();
			}
			
			GetUApi().GetInputByName( "UACarLeft" ).ForceDisable( !isCar );
			GetUApi().GetInputByName( "UACarRight" ).ForceDisable( !isCar );

			if ( isPlane )
			{
				GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( isPlane );
			} else
			{
				GetUApi().GetInputByName( "UACarForward" ).ForceDisable( !isCar );
			}
			
			GetUApi().GetInputByName( "UACarBack" ).ForceDisable( !isCar );

			if ( isBoat )
			{
				GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( false );
				GetUApi().GetInputByName( "UACarShiftGearDown" ).ForceDisable( false );
			} else
			{
				GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( !isCar );
				GetUApi().GetInputByName( "UACarShiftGearDown" ).ForceDisable( !isCar );
			}
		}
	}

	override bool CanBeUsedInRestrain()
	{
		return true;
	}
	
	override void OnUpdate( ActionData action_data )
	{
		if ( action_data.m_State == UA_START )
		{
			if ( !action_data.m_Player.GetCommand_Vehicle() || !action_data.m_Player.GetCommand_Vehicle().IsGettingIn() )
			{
				End( action_data );
			}
		}
	}
	
	override int GetActionCategory()
	{
		return AC_INTERACT;
	}
	
	/*
	override void OnEndClient( ActionData action_data )
	{
		//! if ( action_data.m_Player.GetInventory() ) 
		//	action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	*/
	
	override void OnEndServer( ActionData action_data )
	{
		action_data.m_Player.SetInVehicle ( true );

		//! if ( action_data.m_Player.GetInventory() ) 
		//	action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
}