/**
 * ActionOpenCarDoors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionOpenCarDoors
{
	CarScript m_CarScript;

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//! Player inside vehicle
		if ( player && player.GetCommand_Vehicle() )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ActionOpenCarDoors::ActionCondition 1.0");
			#endif

			if ( Class.CastTo(m_CarScript, player.GetCommand_Vehicle().GetTransport()) )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionOpenCarDoors::ActionCondition 1.1");
				#endif

				int crewIdx = m_CarScript.CrewMemberIndex( player );

				if ( m_CarScript.GetCarDoorsState(m_CarScript.GetDoorInvSlotNameFromSeatPos(crewIdx)) == CarDoorState.DOORS_MISSING )
					return false;

				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionOpenCarDoors::ActionCondition 1.2");
				#endif

				m_AnimSource = m_CarScript.GetAnimSourceFromSelection( m_CarScript.GetDoorSelectionNameFromSeatPos(crewIdx) );
				if ( crewIdx < 0 || !m_CarScript.CanReachDoorsFromSeat( m_AnimSource, crewIdx ) )
					return false;

				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionOpenCarDoors::ActionCondition 1.3");
				#endif

				if ( crewIdx == 0 || crewIdx == 2 )
					m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRIVER_DOOR_OPEN;
	
				if ( crewIdx == 1 || crewIdx == 3 )
					m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_CODRIVER_DOOROPEN;
				
				//! Is in reach, should open the door
				if ( m_CarScript.GetAnimationPhase( m_AnimSource ) <= 0.5 )
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("ActionOpenCarDoors::ActionCondition 1.4");
					#endif

					return !m_CarScript.IsLocked();
				}
			}
		}
		else
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ActionOpenCarDoors::ActionCondition 2.0");
			#endif

			//! Reach check from outside of m_CarScript
			if ( !IsInReach( player, target, UAMaxDistances.DEFAULT ) ) return false;

			#ifdef EXPANSIONEXPRINT
			EXPrint("ActionOpenCarDoors::ActionCondition 2.1");
			#endif

			//! Player is outside of vehicle
			//Message( GetPlayer(), "targetParent: " + target.GetParent() );
			if ( Class.CastTo( m_CarScript, target.GetParent() ) )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ActionOpenCarDoors::ActionCondition 2.2");
				#endif

				array<string> selections = new array<string>();
				
				CarDoor carDoor = CarDoor.Cast( target.GetObject() );
				
				//! Message( GetPlayer(), "target: " + target.GetObject() );
				if (carDoor)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("ActionOpenCarDoors::ActionCondition 2.3");
					#endif

					carDoor.GetActionComponentNameList(target.GetComponentIndex(), selections);
					
					for ( int i = 0; i < selections.Count(); i++ )
					{
						#ifdef EXPANSIONEXPRINT
						EXPrint("ActionOpenCarDoors::ActionCondition 2.4");
						#endif

						m_AnimSource = m_CarScript.GetAnimSourceFromSelection( selections[i]);

						//! Message( GetPlayer(), "selections: " + selections[i] );
						//! Message( GetPlayer(), "m_AnimSource: " + m_AnimSource );

						#ifdef EXPANSIONEXPRINT
						EXPrint("ActionOpenCarDoors::ActionCondition 2.5");
						#endif

						if ( m_AnimSource != "" )
						{
							//! If player is in m_CarScript and cannot reach doors
							m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
	
							//! Is in reach, should open the door
							if ( m_CarScript.GetAnimationPhase( m_AnimSource ) <= 0.5 )
							{
								#ifdef EXPANSIONEXPRINT
								EXPrint("ActionOpenCarDoors::ActionCondition 2.6");
								#endif

								return !m_CarScript.IsLocked();
							}
						}
					}
				}
			}
		}
		
		return false;
	}

	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient( action_data );

		if ( IsMissionHost() && Class.CastTo( m_CarScript, action_data.m_Target.GetParent() ) )
		{
			m_CarScript.OnCarDoorOpened( m_AnimSource );
		}
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		if ( Class.CastTo( m_CarScript, action_data.m_Target.GetParent() ) )
		{
			m_CarScript.OnCarDoorOpened( m_AnimSource );
		}
	}
}