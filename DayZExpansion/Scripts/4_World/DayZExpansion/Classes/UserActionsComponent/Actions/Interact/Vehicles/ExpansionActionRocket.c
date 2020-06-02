/**
 * ExpansionActionRocket.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRocket: ActionInteractBase
{
	private ref Timer m_RocketTimer;

	private int m_Rockets;

	private CarScript m_Vehicle;

	void ExpansionActionRocket()
	{
		m_RocketTimer 		= new Timer();

		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon	 	= CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget 	= new CCTNone;
	}	

	override string GetText()
	{
		return "#rocket";
	}

	override typename GetInputType()
	{
		return ExpansionInputRocket;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			if ( Class.CastTo( m_Vehicle, vehCommand.GetTransport() ) )
			{
				if ( m_Vehicle.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
				{
					return true;
				}
			}
		}

		return false;
	}

	protected void SpawnRocket()
	{
		m_Rockets += 1;

		ExpansionRocket rocket;
		if ( Class.CastTo( rocket, GetGame().CreateObject( "ExpansionRocket", m_Vehicle.ModelToWorld(m_Vehicle.GetMemoryPointPos("rocket_pos")) ) ) )
		{
			//vector dir = m_Vehicle.GetMemoryPointPos("rocket_dir");
			vector dir = m_Vehicle.GetDirection();
			//vector dir = vector.Direction( m_Vehicle.GetMemoryPointPos("rocket_pos"), m_Vehicle.GetMemoryPointPos("rocket_dir") );

			rocket.SetDirection( dir );
			rocket.SetPosition( m_Vehicle.ModelToWorld(m_Vehicle.GetMemoryPointPos("rocket_pos")) );

			rocket.CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			rocket.EnableDynamicCCD( true );

			float speed = m_Vehicle.GetSpeedometer() / 2;

			SetVelocity( rocket, dir * speed /** ( 1.0 / 40.0 /*)*/ );
			dBodyApplyForce( rocket, dir * speed * dBodyGetMass( rocket ) );
		}

		if ( m_Rockets >= 10 )
		{
			m_RocketTimer.Stop();
		}
	}

	override void OnExecuteServer( ActionData action_data )
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				CarScript car;
				if ( Class.CastTo(car, trans) )
				{
					m_Rockets = 0;

					m_RocketTimer.Run( 0.25, this, "SpawnRocket", NULL, true ); 
				}
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}