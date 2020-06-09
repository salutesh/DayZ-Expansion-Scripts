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
		if ( !target )
			return false;
		
		Transport transport;

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
			{
				CarScript car = CarScript.Cast(transport);
				
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
		
		if (!action_data.m_Target)
			return;
		
		Transport transport;

		if ( !Class.CastTo( transport, action_data.m_Target.GetObject() ) )
			return;
		
		int componentIndex = action_data.m_Target.GetComponentIndex();

		int crewIdx = transport.CrewPositionIndex( componentIndex );
		if ( crewIdx < 0 )
			return;
		
		action_data.m_Player.AttachmentDebugPrint( "ExpansionActionGetInTransport::Start" );

		IEntity parent = action_data.m_Player.GetParent();

		if ( parent )
		{
			action_data.m_Player.UnlinkFromLocalSpace();

			parent.Update();
		}
		
		action_data.m_Player.AttachmentDebugPrint( "ExpansionActionGetInTransport::StartCommand" );

		int seat = transport.GetSeatAnimationType( crewIdx );
		HumanCommandVehicle vehCommand = action_data.m_Player.StartCommand_Vehicle( transport, crewIdx, seat );
		if ( vehCommand )
		{
			vehCommand.SetVehicleType( transport.GetAnimInstance() );
			action_data.m_Player.GetItemAccessor().HideItemInHands( true );
			
			GetDayZGame().GetBacklit().OnEnterCar();	
			if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);

			//! if ( action_data.m_Player.GetInventory() ) 
			//	action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}

		CarScript car = CarScript.Cast( transport );

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