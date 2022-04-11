/**
 * ExpansionActionRocket.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRocket: ActionInteractBase
{
	void ExpansionActionRocket()
	{
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
		CarScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{
			if ( car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				return true;
			}

			return false;
		}

		ExpansionHumanCommandVehicle exVehCommand = player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase vehicle;
		if ( vehCommand && Class.CastTo( vehicle, exVehCommand.GetTransport() ) )
		{
			if ( vehicle.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				return true;
			}

			return false;
		}

		return false;
	}

	protected void SpawnRocket( EntityAI vehicle )
	{
		ExpansionRocket rocket;
		if ( Class.CastTo( rocket, GetGame().CreateObject( "ExpansionRocket", vehicle.ModelToWorld( vehicle.GetMemoryPointPos("rocket_pos") ) ) ) )
		{
			//vector dir = vehicle.GetMemoryPointPos("rocket_dir");
			vector dir = vehicle.GetDirection();
			//vector dir = vector.Direction( vehicle.GetMemoryPointPos("rocket_pos"), vehicle.GetMemoryPointPos("rocket_dir") );

			rocket.SetDirection( dir );
			rocket.SetPosition( vehicle.ModelToWorld( vehicle.GetMemoryPointPos( "rocket_pos" ) ) );

			rocket.CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			rocket.EnableDynamicCCD( true );

			float speed = GetVelocity( vehicle ).Length() * 3.6 / 2.0;

			SetVelocity( rocket, dir * speed /** ( 1.0 / 40.0 /*)*/ );
			dBodyApplyForce( rocket, dir * speed * dBodyGetMass( rocket ) );
		}
	}

	override void OnExecuteServer( ActionData action_data )
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		CarScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{
			SpawnRocket( car );
			return;
		}

		ExpansionHumanCommandVehicle exVehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase vehicle;
		if ( vehCommand && Class.CastTo( vehicle, exVehCommand.GetTransport() ) )
		{
			SpawnRocket( vehicle );
			return;
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}