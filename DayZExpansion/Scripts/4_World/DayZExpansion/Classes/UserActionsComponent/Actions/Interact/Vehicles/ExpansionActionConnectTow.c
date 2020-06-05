/**
 * ExpansionActionConnectTow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionConnectTow: ActionInteractBase
{
	protected CarScript m_DrivingVehicle;
	protected CarScript m_RayCastObject;

	void ExpansionActionConnectTow()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if ( m_RayCastObject )
			return "#connect_tow " + m_RayCastObject.GetDisplayName();

		return "INVALID"; // this should not happen ever
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		#ifndef EXPANSION_VEHICLE_TOWING
		return false;
		#endif
		
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo( m_DrivingVehicle, trans ) )
				{
					#ifdef EXPANSION_VEHICLE_TOWING
					if ( m_DrivingVehicle.IsTowed() )
						return false;
					#endif
						
					if ( m_DrivingVehicle.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
					{
                        vector contactPos;
                        vector contactDir;
                        int contactComponent;
                        set<Object> objects = new set<Object>;

                        vector start = m_DrivingVehicle.GetPosition();
                        vector end = start - Vector( 0, 0, -5 );

                        // Initiate the raycast
                        if ( DayZPhysics.RaycastRV( start, end, contactPos, contactDir, contactComponent, objects, NULL, m_DrivingVehicle, false, false, ObjIntersectNone, 3 ) )
                        {
                            if ( Class.CastTo( m_RayCastObject, objects[0] ) )
                            {
								#ifdef EXPANSION_VEHICLE_TOWING
                                return !m_RayCastObject.IsTowed();
								#endif
                            }
                        }
                    }
                }
            }
        }
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
        super.OnStartServer( action_data );

        if ( !m_DrivingVehicle )
            return;

        if ( !m_RayCastObject )
            return;

		#ifdef EXPANSION_VEHICLE_TOWING
        m_DrivingVehicle.CreateTow( m_RayCastObject );
		#endif
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}