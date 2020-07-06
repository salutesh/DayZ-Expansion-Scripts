/**
 * ActionStartEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStartEngine
{
	private CarScript m_ExCar;
	
	override string GetText()
	{
		if ( m_ExCar.IsPlane() )
		{
			return "#STR_EXPANSION_UA_START_PLANE";
		} else if ( m_ExCar.IsHelicopter() )
		{
			return "#STR_EXPANSION_UA_START_HELICOPTER";
		}

		return "#STR_EXPANSION_UA_START_CAR";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
			return false;
		
		IEntity vehicle = player.GetParent();
		if( !vehicle )
			return false;

		ExpansionBoatScript boat;
		if ( Class.CastTo( boat, vehicle ) )
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;

			if ( IsMissionClient() )
			{
				if ( boat.IsUsingBoatController() )
					return false;
			}

			if ( !boat.IsCar() )
				return false;

			if ( boat.MotorIsOn() )
				return true;
		} else if ( vehicle.IsInherited( ExpansionHelicopterScript ) || vehicle.IsInherited( ExpansionPlaneScript ) ) 
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		} else 
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		}
		
		if ( vehicle.IsInherited(ExpansionBikeScript) )
			return false;
		
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			if ( Class.CastTo( m_ExCar, vehCommand.GetTransport() ) && !m_ExCar.EngineIsOn() )
			{
				if ( m_ExCar.GetHealthLevel( "Engine" ) >= GameConstants.STATE_RUINED )
					return false;
				
				if ( m_ExCar.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
				{
					if ( GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 1 && m_ExCar.HasKey() )
					{
						array<EntityAI> playerItems = new array<EntityAI>;
						player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, playerItems );
						for ( int i = 0; i < playerItems.Count(); ++i )
						{
							ExpansionCarKey key;
							if ( Class.CastTo( key, playerItems[i] ) )
							{
								if ( key.IsPairedTo( m_ExCar ) )
									return true;
							}
						}
					} else
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		CarScript car;
		if ( !Class.CastTo( car, action_data.m_Player.GetParent() ) )
			return;

		car.SetCarBatteryStateForVanilla( true );

		if ( !car.EngineIsOn() )
		{
			ExpansionHelicopterScript heli;
			if ( Class.CastTo( heli, car ) )
			{
				EntityAI item2;

				if ( m_SparkCon )
				{
					item2 = NULL;
					if ( heli.IsVitalIgniterPlug() )
						item2 = heli.FindAttachmentBySlotName( "ExpansionIgniterPlug" );

					m_SparkCon = item2 && !item2.IsRuined();
				}

				if ( m_SparkCon )
				{
					item2 = NULL;
					if ( heli.IsVitalHydraulicHoses() )
						item2 = heli.FindAttachmentBySlotName( "ExpansionHydraulicHoses" );

					m_SparkCon = item2 && !item2.IsRuined();
				}

				if ( m_SparkCon )
				{
					item2 = NULL;
					if ( heli.IsVitalHelicopterBattery() )
						item2 = heli.FindAttachmentBySlotName( "ExpansionHelicopterBattery" );

					m_SparkCon = item2 && !item2.IsRuined();
				}
			}
		}

		car.SetCarBatteryStateForVanilla( false );
	}
}