/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_VEHICLE_NEW_HUD
modded class IngameHud
{
	//! Boat
	protected Widget			m_BoatPanel;

	protected ImageWidget		m_BoatRPMPointer;
	protected ImageWidget		m_BoatRPMDial;
	protected ImageWidget		m_BoatRPMRedline;

	protected ImageWidget		m_BoatSpeedPointer;
	protected TextWidget		m_BoatSpeedValue;

	protected ImageWidget		m_BoatTemperaturePointer;
	protected ImageWidget		m_BoatFuelPointer;

	protected ImageWidget		m_BoatShield;

	protected ImageWidget		m_BoatBatteryLight;
	protected ImageWidget		m_BoatEngineLight;
	protected ImageWidget		m_BoatOilLight;
	
	protected Widget			m_BoatGearPanel;
	protected TextWidget		m_BoatGearPrev;
	protected TextWidget		m_BoatGearCurrent;
	protected TextWidget		m_BoatGearNext;

	//! Helicopter	
	protected Widget			m_HelicopterPanel;

	protected ImageWidget		m_HelicopterSpeedPointer;
	protected ImageWidget		m_HelicopterAltitudePointer;

	protected TextWidget		m_HelicopterSpeedValue;
	protected TextWidget		m_HelicopterAltitudeValue;
	protected TextWidget		m_HelicopterVerticalSpeedValue;

	protected ImageWidget		m_HelicopterTemperaturePointer;
	protected ImageWidget		m_HelicopterFuelPointer;

	protected ImageWidget		m_HelicopterShield;

	protected ImageWidget		m_HelicopterBatteryLight;
	protected ImageWidget		m_HelicopterEngineLight;
	protected ImageWidget		m_HelicopterOilLight;
	
	protected Widget			m_HelicopterHoverAltitudePanel;
	protected TextWidget		m_HelicopterHoverAltitudeValue;
	protected TextWidget		m_HelicopterHoverAltitudeLabel;

	//! Plane	
	protected Widget			m_PlanePanel;

	protected ImageWidget		m_PlaneSpeedPointer;
	protected ImageWidget		m_PlaneAltitudePointer;

	protected TextWidget		m_PlaneSpeedValue;
	protected TextWidget		m_PlaneAltitudeValue;

	protected ImageWidget		m_PlaneTemperaturePointer;
	protected ImageWidget		m_PlaneFuelPointer;

	protected ImageWidget		m_PlaneShield;

	protected ImageWidget		m_PlaneBatteryLight;
	protected ImageWidget		m_PlaneEngineLight;
	protected ImageWidget		m_PlaneOilLight;

	protected ProgressBarWidget m_PlaneThrottle;
	protected ProgressBarWidget m_PlaneFlaps;

	protected EntityAI			m_ExpansionVehicle;
	
	override void Init( Widget hud_panel_widget )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Init").Add(hud_panel_widget);
#endif

		super.Init( hud_panel_widget );
		
		//! Vehicle Panels
		if ( m_LeftHudPanelWidget )
		{
			//! Boat
			m_BoatPanel								= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/Vehicles/GUI/layouts/hud/vehicles/boat.layout", m_LeftHudPanelWidget );	
			if ( m_BoatPanel )
			{
				m_BoatPanel.Show( false );
				
				m_BoatRPMPointer					= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatRPMPointer") );
				m_BoatRPMDial						= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatRPMDial") );
				m_BoatRPMRedline					= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatRPMDialRedline") );
				
				m_BoatSpeedPointer					= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatSpeedPointer") );
				m_BoatSpeedValue					= TextWidget.Cast( m_BoatPanel.FindAnyWidget("BoatSpeedValue") );

				m_BoatTemperaturePointer			= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatTemperaturePointer") );
				m_BoatFuelPointer					= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatFuelPointer") );
			
				m_BoatGearPanel						= Widget.Cast( m_BoatPanel.FindAnyWidget("GearsIndicator") );
				m_BoatGearPrev						= TextWidget.Cast( m_BoatPanel.FindAnyWidget("Prev") );
				m_BoatGearCurrent					= TextWidget.Cast( m_BoatPanel.FindAnyWidget("Current") );
				m_BoatGearNext						= TextWidget.Cast( m_BoatPanel.FindAnyWidget("Next") );

				m_BoatShield					= ImageWidget.Cast( m_BoatPanel.FindAnyWidget("BoatShieldLight") );
				m_BoatShield.LoadImageFile(1, "{206DBEAC9C7AD942}DayZExpansion/Core/GUI/icons/hud/connection_lost.edds");
			}
			
			//! Helicopter
			m_HelicopterPanel						= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/Vehicles/GUI/layouts/hud/vehicles/helicopter.layout", m_LeftHudPanelWidget );
			if ( m_HelicopterPanel )
			{
				m_HelicopterPanel.Show( false );
			
				m_HelicopterTemperaturePointer		= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliTemperaturePointer") );
				m_HelicopterFuelPointer				= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliFuelPointer") );

				m_HelicopterSpeedPointer			= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliSpeedPointer") );
				m_HelicopterAltitudePointer			= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliALTPointer") );

				m_HelicopterSpeedValue				= TextWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliSpeedCurrent") );
				m_HelicopterAltitudeValue			= TextWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliALTCurrent") );
				m_HelicopterVerticalSpeedValue		= TextWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliVertSpeedCurrent") );

				m_HelicopterBatteryLight			= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliBatteryLight") );
				m_HelicopterEngineLight				= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliEngineLight") );
				m_HelicopterOilLight				= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliOilLight") );
			
				m_HelicopterHoverAltitudePanel		= Widget.Cast( m_HelicopterPanel.FindAnyWidget("HeliHoverALTIndicator") );
				m_HelicopterHoverAltitudeValue		= TextWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliHoverALTLabelCurrent") );
				m_HelicopterHoverAltitudeLabel		= TextWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliHoverALTLabel") );

				m_HelicopterShield					= ImageWidget.Cast( m_HelicopterPanel.FindAnyWidget("HeliShieldLight") );
				m_HelicopterShield.LoadImageFile(1, "{206DBEAC9C7AD942}DayZExpansion/Core/GUI/icons/hud/connection_lost.edds");
			}
			
			//! Plane
			m_PlanePanel							= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/Vehicles/GUI/layouts/hud/vehicles/plane.layout", m_LeftHudPanelWidget );
			if ( m_PlanePanel )
			{
				m_PlanePanel.Show( false );
			
				m_PlaneTemperaturePointer			= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneTemperaturePointer") );
				m_PlaneFuelPointer					= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneFuelPointer") );
		
				m_PlaneSpeedPointer					= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneSpeedPointer") );
				m_PlaneAltitudePointer				= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneALTPointer") );
		
				m_PlaneSpeedValue					= TextWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneSpeedCurrent") );
				m_PlaneAltitudeValue				= TextWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneALTCurrent") );

				m_PlaneBatteryLight					= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneBatteryLight") );
				m_PlaneEngineLight					= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneEngineLight") );
				m_PlaneOilLight						= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneOilLight") );

				m_PlaneThrottle						= ProgressBarWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneThrottleIndicator") );
				m_PlaneFlaps						= ProgressBarWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneFlapsIndicator") );

				m_PlaneShield					= ImageWidget.Cast( m_PlanePanel.FindAnyWidget("PlaneShieldLight") );
				m_PlaneShield.LoadImageFile(1, "{206DBEAC9C7AD942}DayZExpansion/Core/GUI/icons/hud/connection_lost.edds");
			}
		}
	}
	
	override void ShowVehicleInfo()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "ShowVehicleInfo");
#endif

		super.ShowVehicleInfo();

		if ( !m_CurrentVehicle )
		{
			PlayerBase player;
			if ( !Class.CastTo( player, GetGame().GetPlayer() ) )
			{
				return;
			}

			ExpansionHumanCommandVehicle exHcv = player.GetCommand_ExpansionVehicle();
			if ( exHcv && exHcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				m_ExpansionVehicle = exHcv.GetTransport();
			}
			else
			{
				return;
			}

			m_InVehicleAsDriver	= true;
	
			TFloatArray gears = new TFloatArray;
			GetGame().ConfigGetFloatArray( "CfgVehicles " + m_ExpansionVehicle.GetType() + " SimulationModule Gearbox ratios" , gears );
			
			m_VehicleGearCount = gears.Count() + 1;
				
			m_HudPanelWidget.FindAnyWidget("PlayerPanel").Show( false );
			m_Presence.Show( false );
			m_StancePanel.Show( false );
		}

		ExpansionHelicopterScript d_helicopter;
		ExpansionVehicleHelicopterBase helicopter;
		ExpansionVehiclePlaneBase plane;
		ExpansionBoatScript d_boat;
		ExpansionVehicleBoatBase boat;
		ExpansionVehicleBikeBase bike;
		ExpansionVehicleCarBase exCar;

		if ( Class.CastTo( d_helicopter, m_CurrentVehicle ) )
		{
			m_VehicleGearCount = -1;

			if( !m_VehicleHasOil )
			{
				m_HelicopterBatteryLight.Show( false );
			}
				
			if( !m_VehicleHasCoolant )
			{
				m_HelicopterOilLight.Show( false );
			}

			m_VehiclePanel.Show( false );
		} else if ( Class.CastTo( helicopter, m_ExpansionVehicle ) )
		{
			m_VehicleGearCount = -1;

			if( !m_VehicleHasOil )
			{
				m_HelicopterBatteryLight.Show( false );
			}
				
			if( !m_VehicleHasCoolant )
			{
				m_HelicopterOilLight.Show( false );
			}
		} else if ( Class.CastTo( plane, m_ExpansionVehicle ) )
		{
			m_VehicleGearCount = -1;

			if( !m_VehicleHasOil )
			{
				m_PlaneBatteryLight.Show( false );
			}
				
			if( !m_VehicleHasCoolant )
			{
				m_PlaneOilLight.Show( false );
			}
		} else if ( Class.CastTo( d_boat, m_CurrentVehicle ) )
		{
			float d_b_rpm_value_red = ( d_boat.EngineGetRPMRedline() / d_boat.EngineGetRPMMax() ) ;
			m_BoatRPMDial.SetMaskProgress( d_b_rpm_value_red );
			m_BoatRPMRedline.SetMaskProgress( 1 - d_b_rpm_value_red );

			m_BoatGearPanel.Show( true );

			m_BoatPanel.Show( true );

			m_VehiclePanel.Show( false );
		} else if ( Class.CastTo( boat, m_ExpansionVehicle ) )
		{
			float b_rpm_value_red = ( boat.EngineGetRPMRedline() / boat.EngineGetRPMMax() ) ;
			m_BoatRPMDial.SetMaskProgress( b_rpm_value_red );
			m_BoatRPMRedline.SetMaskProgress( 1 - b_rpm_value_red );

			m_BoatGearPanel.Show( true );

			m_BoatPanel.Show( true );
		} else if ( Class.CastTo( bike, m_ExpansionVehicle ) )
		{
			float bi_rpm_value_red = ( bike.EngineGetRPMRedline() / bike.EngineGetRPMMax() ) ;
			m_VehicleRPMDial.SetMaskProgress( bi_rpm_value_red );
			m_VehicleRPMRedline.SetMaskProgress( 1 - bi_rpm_value_red );

			if ( !m_VehicleHasOil )
			{
				m_VehicleBatteryLight.Show( false );
			}
				
			if ( !m_VehicleHasCoolant )
			{
				m_VehicleOilLight.Show( false );
			}
			
			gears.Clear();
			GetGame().ConfigGetFloatArray( "CfgVehicles " + m_ExpansionVehicle.GetType() + " SimulationModule Gearbox ratios" , gears );
		
			m_VehicleGearCount = gears.Count();

			m_VehiclePanel.Show( true );
		} else if ( Class.CastTo( exCar, m_ExpansionVehicle ) )
		{
			float ex_rpm_value_red = ( exCar.EngineGetRPMRedline() / exCar.EngineGetRPMMax() ) ;
			m_VehicleRPMDial.SetMaskProgress( ex_rpm_value_red );
			m_VehicleRPMRedline.SetMaskProgress( 1 - ex_rpm_value_red );

			if ( !m_VehicleHasOil )
			{
				m_VehicleBatteryLight.Show( false );
			}
				
			if ( !m_VehicleHasCoolant )
			{
				m_VehicleOilLight.Show( false );
			}
			
			gears.Clear();
			GetGame().ConfigGetFloatArray( "CfgVehicles " + m_ExpansionVehicle.GetType() + " SimulationModule Gearbox ratios" , gears );
		
			m_VehicleGearCount = gears.Count();

			m_VehiclePanel.Show( true );
		}
	}
	
	// ------------------------------------------------------------
	// Override HideVehicleInfo
	// ------------------------------------------------------------	
	override void HideVehicleInfo()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "HideVehicleInfo");
#endif

		super.HideVehicleInfo();
		
		m_BoatPanel.Show( false );
		m_PlanePanel.Show( false );
		m_HelicopterPanel.Show( false );
		
		m_ExpansionVehicle = NULL;
	}
	
	// ------------------------------------------------------------
	// Override RefreshVehicleHud
	// ------------------------------------------------------------	
	override void RefreshVehicleHud( float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "RefreshVehicleHud").Add(timeslice);
#endif

		if ( GetGame().GetUIManager().GetMenu() )
		{
			return;
		}

		ExpansionHelicopterScript d_helicopter;
		if ( Class.CastTo( d_helicopter, m_CurrentVehicle ) )
		{
			m_HelicopterPanel.Show( true );
			RefreshHelicopterHud_Deprecated( d_helicopter, timeslice );

			return;
		}

		ExpansionVehicleHelicopterBase helicopter;
		if ( Class.CastTo( helicopter, m_ExpansionVehicle ) )
		{
			m_HelicopterPanel.Show( true );
			RefreshHelicopterHud( helicopter, timeslice );

			return;
		}

		ExpansionVehiclePlaneBase plane;
		if ( Class.CastTo( plane, m_ExpansionVehicle ) )
		{
			m_PlanePanel.Show( true );
			RefreshPlaneHud( plane, timeslice );

			return;
		}

		ExpansionBoatScript d_boat;
		if ( Class.CastTo( d_boat, m_CurrentVehicle ) )
		{
			m_BoatPanel.Show( true );
			RefreshBoatHud_Deprecated( d_boat, timeslice );

			return;
		}

		ExpansionVehicleBoatBase boat;
		if ( Class.CastTo( boat, m_ExpansionVehicle ) )
		{
			m_BoatPanel.Show( true );
			RefreshBoatHud( boat, timeslice );

			return;
		}

		ExpansionVehicleBikeBase bike;
		if ( Class.CastTo( bike, m_ExpansionVehicle ) )
		{
			m_VehiclePanel.Show( true );
			RefreshBikeHud( bike, timeslice );

			return;
		}

		ExpansionVehicleCarBase car;
		if ( Class.CastTo( car, m_ExpansionVehicle ) )
		{
			m_VehiclePanel.Show( true );
			RefreshCarHud( car, timeslice );

			return;
		}

		m_BoatPanel.Show( false );
		m_PlanePanel.Show( false );
		m_HelicopterPanel.Show( false );

		super.RefreshVehicleHud( timeslice );

		if (m_CurrentVehicle && m_CurrentVehicle.m_State.m_IsInvulnerable)
		{
			m_VehicleEngineLight.Show( true );
			m_VehicleEngineLight.SetColor( ARGB(255, 0, 255, 255) );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshHelicopterHud_Deprecated
	// ------------------------------------------------------------	
	private void RefreshHelicopterHud_Deprecated( ExpansionHelicopterScript helicopter, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshHelicopterHud_Deprecated").Add(helicopter).Add(timeslice);
#endif

		float h_speed_value = Math.AbsFloat( helicopter.GetSpeedometer() / 400 );	
		float h_vert_speed_value = ( GetVelocity(helicopter)[1] / 400 );	
		float h_alt_value = ( helicopter.GetPosition()[1] );
		
		m_HelicopterSpeedPointer.SetRotation( 0, 0, h_speed_value * 360 - 130, true );
		m_HelicopterAltitudePointer.SetRotation( 0, 0, ( h_alt_value / 1600) * 360 - 130, true );
		
		m_HelicopterSpeedValue.SetText( Math.AbsInt( helicopter.GetSpeedometer() ).ToString() );

		if (!helicopter.m_State.m_IsSync)
		{
			m_HelicopterShield.SetImage(1);
			if (helicopter.m_State.m_HaltPhysics)
				m_HelicopterShield.SetColor( ARGB(255, 255, 0, 0) );
			else
				m_HelicopterShield.SetColor( ARGB(255, 255, 255, 255) );
		}
		else
		{
			m_HelicopterShield.SetImage(0);
			m_HelicopterShield.SetColor( ARGB(255, 0, 255, 255) );
		}

		bool showShield;
		if (helicopter.m_State.m_IsInvulnerable || !helicopter.m_State.m_IsSync)
			showShield = true;
		
		m_HelicopterShield.Show(showShield);

		m_HelicopterAltitudeValue.SetText( Math.Floor( helicopter.GetPosition()[1] ).ToString() );

		m_HelicopterFuelPointer.SetRotation( 0, 0, helicopter.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_HelicopterTemperaturePointer.SetRotation( 0, 0, helicopter.GetFluidFraction( CarFluid.COOLANT ) * 260 - 130, true );
		
		int h_health = helicopter.GetHealthLevel( "Engine" );
		int h_color;
		
		if (helicopter.m_State.m_IsInvulnerable)
		{
			m_HelicopterEngineLight.Show( true );
			m_HelicopterEngineLight.SetColor( ARGB(255, 0, 255, 255) );
		}
		else if ( h_health > 1 && h_health < 5 )
		{
			m_HelicopterEngineLight.Show( true );
			h_color = ItemManager.GetItemHealthColor( helicopter, "Engine" );
			
			m_HelicopterEngineLight.SetColor( h_color );
			m_HelicopterEngineLight.SetAlpha( 1 );
		} else
		{
			m_HelicopterEngineLight.Show( false );
		}
		
		if ( helicopter.IsAutoHover() )
		{
			m_HelicopterHoverAltitudePanel.Show( true );
			m_HelicopterHoverAltitudeValue.SetText( Math.Round( helicopter.GetAutoHoverTargetHeight() ).ToString() );
		}
		else
		{
			m_HelicopterHoverAltitudePanel.Show( false );
			m_HelicopterHoverAltitudeValue.SetText( "" );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshHelicopterHud
	// ------------------------------------------------------------	
	private void RefreshHelicopterHud( ExpansionVehicleHelicopterBase helicopter, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshHelicopterHud").Add(helicopter).Add(timeslice);
#endif

		float h_speed_value = Math.AbsFloat( helicopter.GetSpeedometer() / 400 );	
		float h_vert_speed_value = ( GetVelocity(helicopter)[1] / 400 );	
		float h_alt_value = ( helicopter.GetPosition()[1] );
		
		m_HelicopterSpeedPointer.SetRotation( 0, 0, h_speed_value * 360 - 130, true );
		m_HelicopterAltitudePointer.SetRotation( 0, 0, ( h_alt_value / 1600) * 360 - 130, true );
		
		m_HelicopterSpeedValue.SetText( Math.AbsInt( helicopter.GetSpeedometer() ).ToString() );
		m_HelicopterAltitudeValue.SetText( Math.Floor( helicopter.GetPosition()[1] ).ToString() );

		m_HelicopterFuelPointer.SetRotation( 0, 0, helicopter.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_HelicopterTemperaturePointer.SetRotation( 0, 0, helicopter.GetFluidFraction( CarFluid.COOLANT ) * 260 - 130, true );
		
		int h_health = helicopter.GetHealthLevel( "Engine" );
		int h_color;
		
		if ( h_health > 1 && h_health < 5 )
		{
			m_HelicopterEngineLight.Show( true );
			h_color = ItemManager.GetItemHealthColor( helicopter, "Engine" );
			
			m_HelicopterEngineLight.SetColor( h_color );
			m_HelicopterEngineLight.SetAlpha( 1 );
		} else
		{
			m_HelicopterEngineLight.Show( false );
		}
		
		if ( helicopter.IsAutoHover() )
		{
			m_HelicopterHoverAltitudePanel.Show( true );
			m_HelicopterHoverAltitudeValue.SetText( Math.Round( helicopter.GetAutoHoverTargetHeight() ).ToString() );
		}
		else
		{
			m_HelicopterHoverAltitudePanel.Show( false );
			m_HelicopterHoverAltitudeValue.SetText( "" );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshPlaneHud
	// ------------------------------------------------------------	
	private void RefreshPlaneHud( ExpansionVehiclePlaneBase plane, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshPlaneHud").Add(plane).Add(timeslice);
#endif
		
		float p_speed_value = Math.AbsFloat( plane.GetSpeedometer() / 400 );	
		float p_vert_speed_value = ( GetVelocity(plane)[1] / 400 );	
		float p_alt_value = ( plane.GetPosition()[1] );
		
		m_PlaneSpeedPointer.SetRotation( 0, 0, p_speed_value * 360 - 130, true );
		m_PlaneAltitudePointer.SetRotation( 0, 0, ( p_alt_value / 1600) * 360 - 130, true );
		
		m_PlaneSpeedValue.SetText( Math.AbsInt( plane.GetSpeedometer() ).ToString() );
		m_PlaneAltitudeValue.SetText( Math.Floor( plane.GetPosition()[1] ).ToString() );

		m_PlaneThrottle.SetCurrent( 50 );
		m_PlaneFlaps.SetCurrent( 50 );
		
		m_PlaneFuelPointer.SetRotation( 0, 0, plane.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_PlaneTemperaturePointer.SetRotation( 0, 0, plane.GetFluidFraction( CarFluid.COOLANT ) * 260 - 130, true );

		int p_health = plane.GetHealthLevel( "Engine" );
		int p_color;
		
		if ( p_health > 1 && p_health < 5 )
		{
			m_PlaneEngineLight.Show( true );
			p_color = ItemManager.GetItemHealthColor( plane, "Engine" );
			
			m_PlaneEngineLight.SetColor( p_color );
			m_PlaneEngineLight.SetAlpha( 1 );
		} else
		{
			m_PlaneEngineLight.Show( false );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshBoatHud_Deprecated
	// ------------------------------------------------------------	
	private void RefreshBoatHud_Deprecated( ExpansionBoatScript boat, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshBoatHud_Deprecated").Add(boat).Add(timeslice);
#endif

		float rpm_value = ( boat.EngineGetRPM() / boat.EngineGetRPMMax() ) ;
		float rpm_value_red = ( boat.EngineGetRPMRedline() / boat.EngineGetRPMMax() ) ;
			
		float speed_value = Math.AbsFloat( boat.GetSpeedometer() / 400 );	

		m_BoatRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_BoatSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		
		m_BoatSpeedValue.SetText( Math.AbsInt( boat.GetSpeedometer() ).ToString() );

		if (!boat.m_State.m_IsSync)
		{
			m_BoatShield.SetImage(1);
			if (boat.m_State.m_HaltPhysics)
				m_BoatShield.SetColor( ARGB(255, 255, 0, 0) );
			else
				m_BoatShield.SetColor( ARGB(255, 255, 255, 255) );
		}
		else
		{
			m_BoatShield.SetImage(0);
			m_BoatShield.SetColor( ARGB(255, 0, 255, 255) );
		}

		bool showShield;
		if (boat.m_State.m_IsInvulnerable || !boat.m_State.m_IsSync)
			showShield = true;

		m_BoatShield.Show(showShield);

		m_BoatFuelPointer.SetRotation( 0, 0, boat.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_BoatTemperaturePointer.SetRotation( 0, 0, boat.GetFluidFraction( CarFluid.COOLANT ) * 260 - 130, true );
		
		//! 1.19
		int engaged_gear = boat.GetGear();
			
		int prev_gear = engaged_gear - 1;
		int next_gear = engaged_gear + 1;
	
		if ( engaged_gear == CarGear.NEUTRAL )
		{
			prev_gear = CarGear.REVERSE;
		}
		else if ( engaged_gear == CarGear.REVERSE )
		{
			prev_gear = -1;
			next_gear = CarGear.NEUTRAL;
		}
		
		m_BoatGearCurrent.SetText( m_VehicleGearTable.Get( engaged_gear ) );
		
		if ( next_gear > m_VehicleGearCount )
		{
			m_BoatGearNext.Show( false );
		} else
		{
			m_BoatGearNext.Show( true );
		}
		
		m_BoatGearNext.SetText( m_VehicleGearTable.Get( next_gear ) );
		m_BoatGearPrev.SetText( m_VehicleGearTable.Get( prev_gear ) );
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshBoatHud
	// ------------------------------------------------------------	
	private void RefreshBoatHud( ExpansionVehicleBoatBase boat, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshBoatHud").Add(boat).Add(timeslice);
#endif

		float rpm_value = ( boat.EngineGetRPM() / boat.EngineGetRPMMax() ) ;
		float rpm_value_red = ( boat.EngineGetRPMRedline() / boat.EngineGetRPMMax() ) ;
			
		float speed_value = Math.AbsFloat( boat.GetSpeedometer() / 400 );	

		m_BoatRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_BoatSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		m_BoatSpeedValue.SetText( Math.AbsInt( boat.GetSpeedometer() ).ToString() );

		m_BoatFuelPointer.SetRotation( 0, 0, boat.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_BoatTemperaturePointer.SetRotation( 0, 0, boat.GetFluidFraction( CarFluid.COOLANT ) * 260 - 130, true );

		int engaged_gear = boat.GetController().GetGear();	
			
		int prev_gear = engaged_gear - 1;
		int next_gear = engaged_gear + 1;
	
		if ( engaged_gear == CarGear.NEUTRAL )
		{
			prev_gear = CarGear.REVERSE;
		}
		else if ( engaged_gear == CarGear.REVERSE )
		{
			prev_gear = -1;
			next_gear = CarGear.NEUTRAL;
		}
		
		m_BoatGearCurrent.SetText( m_VehicleGearTable.Get( engaged_gear ) );
		
		if ( next_gear > m_VehicleGearCount )
		{
			m_BoatGearNext.Show( false );
		} else
		{
			m_BoatGearNext.Show( true );
		}
		
		m_BoatGearNext.SetText( m_VehicleGearTable.Get( next_gear ) );
		m_BoatGearPrev.SetText( m_VehicleGearTable.Get( prev_gear ) );
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshBikeHud
	// ------------------------------------------------------------	
	private void RefreshBikeHud( ExpansionVehicleBikeBase bike, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshBikeHud").Add(bike).Add(timeslice);
#endif

		float rpm_value = ( bike.EngineGetRPM() / bike.EngineGetRPMMax() ) ;
		float rpm_value_red = ( bike.EngineGetRPMRedline() / bike.EngineGetRPMMax() ) ;
		float speed_value = Math.AbsFloat( bike.GetSpeedometer() / 200 );
		
		m_VehicleRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_VehicleSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		m_VehicleSpeedValue.SetText( Math.AbsInt( bike.GetSpeedometer() ).ToString() );

		int engaged_gear = bike.GetController().GetGear();
		
		int prev_gear = engaged_gear - 1;
		int next_gear = engaged_gear + 1;

		if ( engaged_gear == CarGear.NEUTRAL )
		{
			prev_gear = CarGear.REVERSE;
		} else if ( engaged_gear == CarGear.REVERSE )
		{
			prev_gear = -1;
			next_gear = CarGear.NEUTRAL;
		}
		
		bool newHealth = false;
		
		int health = bike.GetHealthLevel( "Engine" );
		float oilAmount = bike.GetFluidFraction( CarFluid.OIL );
		int color;
		if ( bike.EngineGetRPM() > bike.EngineGetRPMRedline() /*|| (bike.EngineIsOn() && oilAmount < 0.25)*/ )
		{
			if ( m_TimeSinceLastEngineLightChange > 0.35 )
			{
				m_VehicleEngineLight.Show( !m_VehicleEngineLight.IsVisible() );
				m_VehicleEngineLight.SetColor( Colors.COLOR_RUINED );
				m_VehicleEngineLight.SetAlpha( 1 );
				m_TimeSinceLastEngineLightChange = 0;
			}

			m_TimeSinceLastEngineLightChange += timeslice;
			newHealth = true;
		} else if ( health > 1 && health < 5 )
		{
			color = ItemManager.GetItemHealthColor( bike, "Engine" );
			
			m_VehicleEngineLight.SetColor( color );
			m_VehicleEngineLight.SetAlpha( 1 );
			m_VehicleEngineLight.Show( true );
		} else
		{
			m_VehicleEngineLight.Show( false );
		}

		m_VehicleCurrentGearValue.SetText( m_VehicleGearTable.Get( engaged_gear ) );
		
		if ( next_gear > m_VehicleGearCount )
		{
			m_VehicleNextGearValue.Show( false );
		} else
		{
			m_VehicleNextGearValue.Show( true );
		}
		
		m_VehicleNextGearValue.SetText( m_VehicleGearTable.Get( next_gear ) );
		m_VehiclePrevGearValue.SetText( m_VehicleGearTable.Get( prev_gear ) );
			
		m_VehicleFuelPointer.SetRotation( 0, 0, bike.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_VehicleTemperaturePointer.SetRotation( 0, 0, -1 * bike.GetFluidFraction( CarFluid.COOLANT ) * 260 + 130, true );
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshCarHud
	// ------------------------------------------------------------	
	private void RefreshCarHud( ExpansionVehicleCarBase car, float timeslice )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "RefreshCarHud").Add(car).Add(timeslice);
#endif

		float rpm_value = ( car.EngineGetRPM() / car.EngineGetRPMMax() ) ;
		float rpm_value_red = ( car.EngineGetRPMRedline() / car.EngineGetRPMMax() ) ;
		float speed_value = Math.AbsFloat( car.GetSpeedometer() / 200 );
		
		m_VehicleRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_VehicleSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		m_VehicleSpeedValue.SetText( Math.AbsInt( car.GetSpeedometer() ).ToString() );

		int engaged_gear = car.GetController().GetGear();
		
		int prev_gear = engaged_gear - 1;
		int next_gear = engaged_gear + 1;

		if ( engaged_gear == CarGear.NEUTRAL )
		{
			prev_gear = CarGear.REVERSE;
		} else if ( engaged_gear == CarGear.REVERSE )
		{
			prev_gear = -1;
			next_gear = CarGear.NEUTRAL;
		}
		
		bool newHealth = false;
		
		int health = car.GetHealthLevel( "Engine" );
		float oilAmount = car.GetFluidFraction( CarFluid.OIL );
		int color;
		if ( car.EngineGetRPM() > car.EngineGetRPMRedline() /*|| (car.EngineIsOn() && oilAmount < 0.25)*/ )
		{
			if ( m_TimeSinceLastEngineLightChange > 0.35 )
			{
				m_VehicleEngineLight.Show( !m_VehicleEngineLight.IsVisible() );
				m_VehicleEngineLight.SetColor( Colors.COLOR_RUINED );
				m_VehicleEngineLight.SetAlpha( 1 );
				m_TimeSinceLastEngineLightChange = 0;
			}

			m_TimeSinceLastEngineLightChange += timeslice;
			newHealth = true;
		} else if ( health > 1 && health < 5 )
		{
			color = ItemManager.GetItemHealthColor( car, "Engine" );
			
			m_VehicleEngineLight.SetColor( color );
			m_VehicleEngineLight.SetAlpha( 1 );
			m_VehicleEngineLight.Show( true );
		} else
		{
			m_VehicleEngineLight.Show( false );
		}

		m_VehicleCurrentGearValue.SetText( m_VehicleGearTable.Get( engaged_gear ) );
		
		if ( next_gear > m_VehicleGearCount + 1 )
		{
			m_VehicleNextGearValue.Show( false );
		} else
		{
			m_VehicleNextGearValue.Show( true );
		}

		m_VehicleNextGearValue.SetText( m_VehicleGearTable.Get( next_gear ) );
		m_VehiclePrevGearValue.SetText( m_VehicleGearTable.Get( prev_gear ) );
			
		m_VehicleFuelPointer.SetRotation( 0, 0, car.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_VehicleTemperaturePointer.SetRotation( 0, 0, -1 * car.GetFluidFraction( CarFluid.COOLANT ) * 260 + 130, true );
	}
};
#endif
