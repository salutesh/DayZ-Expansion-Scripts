class ExpansionActionOpenVehicleDoor: ActionInteractBase
{
	string m_AnimSource = "";
	
	void ExpansionActionOpenVehicleDoor()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		//m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
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
		ExpansionVehicleBase vehicle;

		//! player inside vehicle
		if ( player && player.GetCommand_ExpansionVehicle() )
		{
			if ( Class.CastTo( vehicle, player.GetCommand_ExpansionVehicle().GetTransport() ) )
			{
				int crewIdx = vehicle.CrewMemberIndex( player );

				if( vehicle.GetCarDoorsState(vehicle.GetDoorInvSlotNameFromSeatPos(crewIdx)) == CarDoorState.DOORS_MISSING )
					return false;

				if( vehicle.IsLocked() )
					return false;

				m_AnimSource = vehicle.GetAnimSourceFromSelection( vehicle.GetDoorSelectionNameFromSeatPos(crewIdx) );
				if ( crewIdx < 0 || !vehicle.CanReachDoorsFromSeat( m_AnimSource, crewIdx ) )
					return false;
	
				if ( crewIdx == 0 || crewIdx == 2 )
					m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRIVER_DOOR_OPEN;
	
				if ( crewIdx == 1 || crewIdx == 3 )
					m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_CODRIVER_DOOROPEN;
				
				//! is in reach, should open the door
				if ( vehicle.GetAnimationPhase( m_AnimSource ) <= 0.5 )
					return true;
			}
		} else
		{
			//! reach check from outside of vehicle
			if ( !IsInReach(player, target, UAMaxDistances.DEFAULT) )
				return false;

			//! player is outside of vehicle
			if ( Class.CastTo(vehicle, target.GetParent()) )
			{
				array<string> selections = new array<string>();

				if( vehicle.IsLocked() )
					return false;
				
				CarDoor door;
				if ( Class.CastTo( door, target.GetObject() ) )
				{
					door.GetActionComponentNameList( target.GetComponentIndex(), selections );
					
					for ( int i = 0; i < selections.Count(); i++ )
					{
						m_AnimSource = vehicle.GetAnimSourceFromSelection( selections[i] );
						if ( m_AnimSource != "" )
						{
							//! if player is in vehicle and cannot reach doors
							m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
	
							//! is in reach, should open the door
							if ( vehicle.GetAnimationPhase( m_AnimSource ) <= 0.5 )
								return true;
						}
					}
				}
			}
		}
		
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ExpansionVehicleBase vehicle;
		//! player inside vehicle
		if ( action_data.m_Player && action_data.m_Player.GetCommand_ExpansionVehicle() )
		{
			vehicle = ExpansionVehicleBase.Cast( action_data.m_Player.GetCommand_ExpansionVehicle().GetTransport() );
		} else
		{
			vehicle = ExpansionVehicleBase.Cast( action_data.m_Target.GetParent() );
		}

		if ( vehicle )
		{
			vehicle.SetAnimationPhase( m_AnimSource, 1.0);
		}
	}

	override void OnStartClient( ActionData action_data )
	{
		ExpansionVehicleBase vehicle;
		//! player inside vehicle
		if ( action_data.m_Player && action_data.m_Player.GetCommand_ExpansionVehicle() )
		{
			vehicle = ExpansionVehicleBase.Cast(action_data.m_Player.GetCommand_ExpansionVehicle().GetTransport());
		} else
		{
			vehicle = ExpansionVehicleBase.Cast(action_data.m_Target.GetParent());
		}
	
		if ( vehicle )
		{
			vehicle.SetAnimationPhase( m_AnimSource, 1.0  );
			
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
				SEffectManager.PlaySound( "offroad_door_open_SoundSet", vehicle.GetPosition() );
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}