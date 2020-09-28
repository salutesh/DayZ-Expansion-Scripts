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
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	private CarScript car;
	
	override string GetText()
	{
		if ( car.IsPlane() )
		{
			return "#STR_EXPANSION_UA_START_PLANE";
		} else if ( car.IsHelicopter() )
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
			if ( Class.CastTo( car, vehCommand.GetTransport() ) && !car.EngineIsOn() )
			{
				if ( car.GetHealthLevel( "Engine" ) >= GameConstants.STATE_RUINED )
					return false;
				
				if ( car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
				{
					if ( ( GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 1 || GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 2 ) && car.HasKey() )
					{
						array<EntityAI> playerItems = new array<EntityAI>;
						player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, playerItems );
						for ( int i = 0; i < playerItems.Count(); ++i )
						{
							ExpansionCarKey key;
							if ( Class.CastTo( key, playerItems[i] ) )
							{
								if ( key.IsPairedTo( car ) && car.IsCarKeys( key ) )
								{
									#ifdef EXPANSION_CARKEY_LOGGING
									EXLogPrint("ActionStartEngine::ActionCondition - Player HAS paired key!");
									#endif
									return true;
								}
								#ifdef EXPANSION_CARKEY_LOGGING
								else
								{
									EXLogPrint("ActionStartEngine::ActionCondition - Player has NOT paired key!");
								}
								#endif
							}
						}
					} else
					{
						#ifdef EXPANSION_CARKEY_LOGGING
						EXLogPrint("ActionStartEngine::ActionCondition - Vehicle has no key paired to it!");
						#endif
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
				m_SparkCon = false;
				m_BeltCon = false;
				m_BatteryCon = false;

				if ( heli.IsVitalIgniterPlug() || heli.IsVitalSparkPlug() || heli.IsVitalGlowPlug() )
				{
					item2 = NULL;
					if ( heli.IsVitalIgniterPlug() ) item2 = heli.FindAttachmentBySlotName( "ExpansionIgniterPlug" );
					if ( heli.IsVitalSparkPlug() ) item2 = heli.FindAttachmentBySlotName( "SparkPlug" );
					if ( heli.IsVitalGlowPlug() ) item2 = heli.FindAttachmentBySlotName( "GlowPlug" );

					if ( item2 && !item2.IsRuined() )
						m_SparkCon = true;
				} else
				{
					m_SparkCon = true;
				}
				if ( heli.IsVitalHydraulicHoses() || heli.IsVitalRadiator() )
				{
					item2 = NULL;
					if ( heli.IsVitalHydraulicHoses() ) item2 = heli.FindAttachmentBySlotName( "ExpansionHydraulicHoses" );
					if ( heli.IsVitalRadiator() ) item2 = heli.FindAttachmentBySlotName( "CarRadiator" );

					if ( item2 && !item2.IsRuined() )
						m_BeltCon = true;
				} else
				{
					m_BeltCon = true;
				}
				if ( heli.IsVitalHelicopterBattery() || heli.IsVitalCarBattery() || heli.IsVitalTruckBattery() )
				{
					item2 = NULL;
					if ( heli.IsVitalHelicopterBattery() ) item2 = heli.FindAttachmentBySlotName( "ExpansionHelicopterBattery" );
					if ( heli.IsVitalCarBattery() ) item2 = heli.FindAttachmentBySlotName( "CarBattery" );
					if ( heli.IsVitalTruckBattery() ) item2 = heli.FindAttachmentBySlotName( "TruckBattery" );

					if ( item2 && !item2.IsRuined() )
						m_BatteryCon = true;
				} else
				{
					m_BatteryCon = true;
				}
			}
		}

		car.SetCarBatteryStateForVanilla( false );
	}
}