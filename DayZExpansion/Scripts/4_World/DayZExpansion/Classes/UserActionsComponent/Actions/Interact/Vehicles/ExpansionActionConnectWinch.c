/**
 * ExpansionActionConnectWinch.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionConnectWinch: ActionInteractBase
{
	protected ExpansionHelicopterScript m_Helicopter;
	protected CarScript m_RayCastObject;
	
	float ropeLength = 10;
	float radius = 5;
	float stiffness = 10;
	float damping = 0.5;

	void ExpansionActionConnectWinch()
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
			return "#STR_USRACT_CONNECT_WINCH " + m_RayCastObject.GetDisplayName();

		return "INVALID"; // this should not happen ever
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Disabled
		// return false;
		
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo( m_Helicopter, trans ) )
				{
					if ( m_Helicopter.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
					{
						vector contactPos;
						vector contactDir;
						int contactComponent;
						set<Object> objects = new set<Object>;

						vector start = m_Helicopter.GetPosition() + dBodyGetCenterOfMass( m_Helicopter );
						vector end = start - Vector( 0, ropeLength, 0 );

						// Initiate the raycast
						if ( DayZPhysics.RaycastRV( start, end, contactPos, contactDir, contactComponent, objects, NULL, m_Helicopter, false, false, ObjIntersectGeom, radius ) )
						{
							if ( Class.CastTo( m_RayCastObject, objects[0] ) )
							{
								return !m_RayCastObject.IsWinched();
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

		if ( !m_Helicopter )
			return;

		if ( !m_RayCastObject )
			return;

		vector heliTransform[4];
		vector objTransform[4];

		m_Helicopter.GetTransform( heliTransform );
		m_RayCastObject.GetTransform( objTransform );

		objTransform[3] = dBodyGetCenterOfMass( m_RayCastObject ) + "0 2 0";

		heliTransform[3] = dBodyGetCenterOfMass( m_Helicopter );
		// heliTransform[3][0] = 0;
		// heliTransform[3][2] = 0;

		dJoint joint = dJointCreate6DOFSpring( m_Helicopter, m_RayCastObject, heliTransform, objTransform, false, 1000 );

		vector hingelin_min = Vector( -ropeLength, -ropeLength, -ropeLength );
		vector hingelin_max = Vector( ropeLength, ropeLength, ropeLength );

		vector hingeang_min = Vector( -Math.DEG2RAD * 0, -Math.DEG2RAD * 45, -Math.DEG2RAD * 45 );
		vector hingeang_max = Vector( Math.DEG2RAD * 0, Math.DEG2RAD * 45, Math.DEG2RAD * 45 );

		dJoint6DOFSetLinearLimits( joint, hingelin_min, hingelin_max );
		dJoint6DOFSetAngularLimits( joint, hingeang_min, hingeang_max );

		dJoint6DOFSpringSetSpring( joint, 0, stiffness, damping );
		dJoint6DOFSpringSetSpring( joint, 1, stiffness, damping );
		dJoint6DOFSpringSetSpring( joint, 2, stiffness, damping );
		dJoint6DOFSpringSetSpring( joint, 3, -1, -1 );
		dJoint6DOFSpringSetSpring( joint, 4, -1, -1 );
		dJoint6DOFSpringSetSpring( joint, 5, -1, -1 );

		//m_Helicopter.SetWinch( m_RayCastObject, joint );
		//m_RayCastObject.SetWinch( m_Helicopter, joint );

		Message( action_data.m_Player, "Winched: " + m_RayCastObject );
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}