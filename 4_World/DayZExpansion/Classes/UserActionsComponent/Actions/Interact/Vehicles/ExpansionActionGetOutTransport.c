/**
 * ExpansionActionGetOutTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionGetOutTransport
 * @brief		
 **/
class ExpansionActionGetOutTransport: ActionInteractBase
{
	private Transport m_transport;
	private int m_crewIdx;

	void ExpansionActionGetOutTransport()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = "GetInDriver";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#leave_vehicle";
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
 		m_transport = null;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			m_transport = vehCommand.GetTransport();
			if ( m_transport )
			{
				m_crewIdx = m_transport.CrewMemberIndex( player );
				if ( m_crewIdx >= 0 && m_transport.CrewCanGetThrough( m_crewIdx ) )
					return true;
			}
		}

		return false;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );

		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			//! if ( action_data.m_Player.GetInventory() ) 
			//	action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);

			CarScript car;
			if ( Class.CastTo( car, vehCommand.GetTransport() ) )
			{
				if ( !car.IsCar() || car.IsBoat() )
				{
					vehCommand.GetOutVehicle();
				} else if ( car.LeavingSeatDoesAttachment( vehCommand.GetVehicleSeat() ) )
				{
					vehCommand.GetOutVehicle();
				} else
				{
					float speed = car.GetSpeedometer();
					if ( speed <= 8 )
					{
						vehCommand.GetOutVehicle();
					} else
					{
						vehCommand.JumpOutVehicle();
					}
				}

				if ( IsMissionClient() )
				{
					GetUApi().GetInputByName( "UACarLeft" ).ForceDisable( false );
					GetUApi().GetInputByName( "UACarRight" ).ForceDisable( false );
					GetUApi().GetInputByName( "UACarForward" ).ForceDisable( false );
					GetUApi().GetInputByName( "UACarBack" ).ForceDisable( false );
					
					GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( false );
					GetUApi().GetInputByName( "UACarShiftGearDown" ).ForceDisable( false );
				}
				
				GetDayZGame().GetBacklit().OnLeaveCar();									
			}

			if ( action_data.m_Player.GetInventory() ) 
				action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);

			action_data.m_Player.SetInventorySoftLock(false);
		}
	}
	
	override void End( ActionData action_data )
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( Unhide, 500, false, ( action_data.m_Player ) );

		super.End( action_data );
	}
	
	void Unhide(PlayerBase player)
	{
		player.GetItemAccessor().HideItemInHands(false);

		//! if ( player.GetInventory() ) 
		//	player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);

		//! player.SetInventorySoftLock(false);
	}

	override void OnUpdate(ActionData action_data)
	{
		if ( action_data.m_State == UA_START )
		{
			if ( !action_data.m_Player.GetCommand_Vehicle().IsGettingOut() )
			{
				End( action_data );
			}
		}
	}
	
	override bool CanBeUsedInRestrain()
	{
		return true;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override void OnEndServer( ActionData action_data )
	{
		action_data.m_Player.SetInVehicle( false );

		//! if ( action_data.m_Player.GetInventory() ) 
		//	action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);

		//! action_data.m_Player.SetInventorySoftLock(false);
	}

	override void OnEndClient( ActionData action_data )
	{
		super.OnEndClient( action_data );

		//! if ( action_data.m_Player.GetInventory() ) 
		//	action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);

		//! action_data.m_Player.SetInventorySoftLock(false);
	}
	
	/*
	override bool IsInstant()
	{
		return true;
	}
	*/
}