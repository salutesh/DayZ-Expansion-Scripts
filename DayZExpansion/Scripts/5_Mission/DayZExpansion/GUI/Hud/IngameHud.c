/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


modded class IngameHud
{
	protected bool				m_ExpansionHUD;

	protected bool				m_HudChat;
	
	protected ref Widget		m_LeftHudPanel;
	protected Widget			m_VehiclePanelsWrapper;
	
	//! Boat
	protected Widget			m_BoatPanel;

	protected ImageWidget		m_BoatRPMPointer;
	protected ImageWidget		m_BoatRPMDial;
	protected ImageWidget		m_BoatRPMRedline;

	protected ImageWidget		m_BoatSpeedPointer;
	protected TextWidget		m_BoatSpeedValue;

	protected ImageWidget		m_BoatTemperaturePointer;
	protected ImageWidget		m_BoatFuelPointer;

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

	protected ImageWidget		m_PlaneBatteryLight;
	protected ImageWidget		m_PlaneEngineLight;
	protected ImageWidget		m_PlaneOilLight;

	protected ProgressBarWidget m_PlaneThrottle;
	protected ProgressBarWidget m_PlaneFlaps;

	protected EntityAI			m_ExpansionVehicle;
	
	//! Player Tag
	protected ImageWidget		m_PlayerTagIcon;
	
	// ------------------------------------------------------------
	// IngameHud Constructor
	// ------------------------------------------------------------	
	void IngameHud()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::IngameHud - Start");
		#endif
		m_ExpansionHUD = false;
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::IngameHud - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion SetExpansionHUD
	// ------------------------------------------------------------	
	void SetExpansionHUD( bool expansionHUD )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::SetExpansionHUD - Start");
		#endif

		m_ExpansionHUD = expansionHUD;

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::SetExpansionHUD - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------	
	override void Init( Widget hud_panel_widget )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::Init - Start");
		#endif
		super.Init( hud_panel_widget );
		
		m_HudPanelWidget = hud_panel_widget;
		m_HudPanelWidget.Show( true );
		
		//! Vehicle Panels
		m_LeftHudPanel = m_HudPanelWidget.FindAnyWidget( "LeftHUDPanel" );
		if ( m_LeftHudPanel )
		{
			//! Boat
			m_BoatPanel								= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/hud/vehicles/boat.layout", m_LeftHudPanel );	
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
			}
			
			//! Helicopter
			m_HelicopterPanel						= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/hud/vehicles/helicopter.layout", m_LeftHudPanel );
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
			}
			
			//! Plane
			m_PlanePanel							= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/hud/vehicles/plane.layout", m_LeftHudPanel );
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
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::Init - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override ShowVehicleInfo
	// ------------------------------------------------------------	
	override void ShowVehicleInfo()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::ShowVehicleInfo - Start");
		#endif

		if ( !m_ExpansionHUD )
		{
			super.ShowVehicleInfo();
			#ifdef EXPANSIONEXPRINT
			EXPrint("IngameHud::ShowVehicleInfo - First Return");
			#endif
			return;
		}

		m_CurrentVehicle = NULL;
		
		PlayerBase player;
		if ( !Class.CastTo( player, GetGame().GetPlayer() ) )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("IngameHud::ShowVehicleInfo - Second Return");
			#endif
			return;
		}

		HumanCommandVehicle hcv = player.GetCommand_Vehicle();
		if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			m_ExpansionVehicle = hcv.GetTransport();

		if ( m_ExpansionVehicle == NULL )
		{
			ExpansionHumanCommandVehicle exHcv = player.GetCommand_ExpansionVehicle();
			if ( exHcv && exHcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				m_ExpansionVehicle = exHcv.GetTransport();
			} else
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("IngameHud::ShowVehicleInfo - Third Return");
				#endif
				return;
			}
		}

		m_InVehicleAsDriver	= true;

		TFloatArray gears = new TFloatArray;
		GetGame().ConfigGetFloatArray( "CfgVehicles " + m_ExpansionVehicle.GetType() + " SimulationModule Gearbox ratios" , gears );
		
		m_VehicleGearCount = gears.Count() + 1;
			
		m_HudPanelWidget.FindAnyWidget("PlayerPanel").Show( false );
		m_Presence.Show( false );
		m_StancePanel.Show( false );

		ExpansionHelicopterScript helicopter;
		ExpansionPlaneScript plane;
		ExpansionBoatScript boat;
		ExpansionBikeScript bike;
		ExpansionCarScript exCar;
		CarScript car;

		if ( Class.CastTo( helicopter, m_ExpansionVehicle ) )
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
		} else if ( Class.CastTo( boat, m_ExpansionVehicle ) )
		{
			float b_rpm_value_red = ( boat.EngineGetRPMRedline() / boat.EngineGetRPMMax() ) ;
			m_BoatRPMDial.SetMaskProgress( b_rpm_value_red );
			m_BoatRPMRedline.SetMaskProgress( 1 - b_rpm_value_red );

			m_BoatGearPanel.Show( true );

			m_BoatPanel.Show( true );
		} else if ( Class.CastTo( car, m_ExpansionVehicle ) )
		{
			float rpm_value_red = ( car.EngineGetRPMRedline() / car.EngineGetRPMMax() ) ;
			m_VehicleRPMDial.SetMaskProgress( rpm_value_red );
			m_VehicleRPMRedline.SetMaskProgress( 1 - rpm_value_red );

			if ( !m_VehicleHasOil )
			{
				m_VehicleBatteryLight.Show( false );
			}
				
			if ( !m_VehicleHasCoolant )
			{
				m_VehicleOilLight.Show( false );
			}
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
			GetGame().ConfigGetFloatArray( "CfgVehicles " + m_ExpansionVehicle.GetType() + " VehicleSimulation Gearbox ratios" , gears );
		
			m_VehicleGearCount = gears.Count() + 1;
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
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::ShowVehicleInfo - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override HideVehicleInfo
	// ------------------------------------------------------------	
	override void HideVehicleInfo()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::HideVehicleInfo - Start");
		#endif

		if ( !m_ExpansionHUD )
		{
			super.HideVehicleInfo();

			#ifdef EXPANSIONEXPRINT
			EXPrint("IngameHud::HideVehicleInfo - First Return");
			#endif
			return;
		}

		m_HudPanelWidget.FindAnyWidget( "PlayerPanel" ).Show( true );
		m_Presence.Show( true );
		m_StancePanel.Show( true );
		
		m_VehiclePanel.Show( false );
		m_BoatPanel.Show( false );
		m_PlanePanel.Show( false );
		m_HelicopterPanel.Show( false );
		
		m_InVehicleAsDriver	= false;
		m_ExpansionVehicle = NULL;
		m_VehicleGearCount = -1;

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::HideVehicleInfo - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override RefreshVehicleHud
	// ------------------------------------------------------------	
	override void RefreshVehicleHud( float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshVehicleHud - Start");
		#endif

		if ( !m_ExpansionHUD )
		{
			super.RefreshVehicleHud( timeslice );

			#ifdef EXPANSIONEXPRINT
			EXPrint("IngameHud::RefreshVehicleHud - First Return");
			#endif
			return;
		}

		if ( GetGame().GetUIManager().GetMenu() )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("IngameHud::RefreshVehicleHud - Second Return");
			#endif
			return;
		}

		ExpansionHelicopterScript helicopter;
		if ( Class.CastTo( helicopter, m_ExpansionVehicle ) )
		{
			m_HelicopterPanel.Show( true );
			RefreshHelicopterHud( helicopter, timeslice );
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionIngameHud::RefreshVehicleHud - Helicopter");
			#endif
			return;
		}

		ExpansionPlaneScript plane;
		if ( Class.CastTo( plane, m_ExpansionVehicle ) )
		{
			m_PlanePanel.Show( true );
			RefreshPlaneHud( plane, timeslice );
			return;
		}

		ExpansionBoatScript boat;
		if ( Class.CastTo( boat, m_ExpansionVehicle ) )
		{
			m_BoatPanel.Show( true );
			RefreshBoatHud( boat, timeslice );
			return;
		}

		ExpansionBikeScript bike; //! inherits from ExpansionCarScript
		if ( Class.CastTo( bike, m_ExpansionVehicle ) )
		{
			m_VehiclePanel.Show( true );
			RefreshBikeHud( bike, timeslice );
			return;
		}

		ExpansionCarScript car;
		if ( Class.CastTo( car, m_ExpansionVehicle ) )
		{
			m_VehiclePanel.Show( true );
			RefreshCarHud( car, timeslice );
			return;
		}

		if ( Class.CastTo( m_CurrentVehicle, m_ExpansionVehicle ) )
		{
			m_VehiclePanel.Show( true );
			super.RefreshVehicleHud( timeslice );
			return;
		}
		
		m_VehiclePanel.Show( false );
		m_BoatPanel.Show( false );
		m_PlanePanel.Show( false );
		m_HelicopterPanel.Show( false );

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshVehicleHud - End");
		#endif
	}

	private bool PositionOutOfScreen(vector posRelative)
	{
		if ( posRelative[0] >= 1 || posRelative[0] <= 0 || posRelative[1] >= 1 || posRelative[1] <= 0 || posRelative[2] <= 0 ) return true;

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshHelicopterHud
	// ------------------------------------------------------------	
	private void RefreshHelicopterHud( ExpansionHelicopterScript helicopter, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshHelicopterHud - Start");
		#endif

		float h_speed_value = ( helicopter.GetSpeedometer() / 400 );	
		float h_vert_speed_value = ( GetVelocity(helicopter)[1] / 400 );	
		float h_alt_value = ( helicopter.GetPosition()[1] );
		
		m_HelicopterSpeedPointer.SetRotation( 0, 0, h_speed_value * 360 - 130, true );
		m_HelicopterAltitudePointer.SetRotation( 0, 0, ( h_alt_value / 1600) * 360 - 130, true );
		
		m_HelicopterSpeedValue.SetText( Math.Floor( helicopter.GetSpeedometer() ).ToString() );
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshHelicopterHud - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshPlaneHud
	// ------------------------------------------------------------	
	private void RefreshPlaneHud( ExpansionPlaneScript plane, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshPlaneHud - Start");
		#endif

		float p_rpm_value = ( plane.EngineGetRPM() / plane.EngineGetRPMMax() ) ;
		float p_rpm_value_red = ( plane.EngineGetRPM() / plane.EngineGetRPMRedline() ) ;
		
		float p_speed_value = ( plane.GetSpeedometer() / 400 );	
		float p_vert_speed_value = ( GetVelocity(plane)[1] / 400 );	
		float p_alt_value = ( plane.GetPosition()[1] );
		
		m_PlaneSpeedPointer.SetRotation( 0, 0, p_speed_value * 360 - 130, true );
		m_PlaneAltitudePointer.SetRotation( 0, 0, ( p_alt_value / 1600) * 360 - 130, true );
		
		m_PlaneSpeedValue.SetText( Math.Floor( plane.GetSpeedometer() ).ToString() );
		m_PlaneAltitudeValue.SetText( Math.Floor( plane.GetPosition()[1] ).ToString() );

		m_PlaneThrottle.SetCurrent( 50 );
		m_PlaneFlaps.SetCurrent( 50 );
		
		m_PlaneFuelPointer.SetRotation( 0, 0, plane.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_PlaneTemperaturePointer.SetRotation( 0, 0, plane.GetFluidFraction( CarFluid.COOLANT ) * 260 - 130, true );

		int p_health = plane.GetHealthLevel( "Engine" );
		int p_color;
		if ( p_rpm_value_red > 1 )
		{
			if ( m_TimeSinceLastEngineLightChange > 0.35 )
			{
				m_PlaneEngineLight.Show( !m_PlaneEngineLight.IsVisible() );
				m_PlaneEngineLight.SetColor( Colors.COLOR_RUINED );
				m_PlaneEngineLight.SetAlpha( 1 );
				m_TimeSinceLastEngineLightChange = 0;
			}

			m_TimeSinceLastEngineLightChange += timeslice;
		} else if ( p_health > 1 && p_health < 5 )
		{
			m_PlaneEngineLight.Show( true );
			p_color = ItemManager.GetItemHealthColor( plane, "Engine" );
			
			m_PlaneEngineLight.SetColor( p_color );
			m_PlaneEngineLight.SetAlpha( 1 );
		} else
		{
			m_PlaneEngineLight.Show( false );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshPlaneHud - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshBoatHud
	// ------------------------------------------------------------	
	private void RefreshBoatHud( ExpansionBoatScript boat, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshBoatHud - Start");
		#endif

		float rpm_value = ( boat.EngineGetRPM() / boat.EngineGetRPMMax() ) ;
		float rpm_value_red = ( boat.EngineGetRPMRedline() / boat.EngineGetRPMMax() ) ;
			
		float speed_value = ( boat.GetSpeedometer() / 400 );	

		m_BoatRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_BoatSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		m_BoatSpeedValue.SetText( Math.Floor( boat.GetSpeedometer() ).ToString() );

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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshBoatHud - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshBikeHud
	// ------------------------------------------------------------	
	private void RefreshBikeHud( ExpansionBikeScript car, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshBikeHud - Start");
		#endif

		float rpm_value = ( car.EngineGetRPM() / car.EngineGetRPMMax() ) ;
		float rpm_value_red = ( car.EngineGetRPMRedline() / car.EngineGetRPMMax() ) ;
		float speed_value = ( car.GetSpeedometer() / 200 );
		
		m_VehicleRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_VehicleSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		m_VehicleSpeedValue.SetText( Math.Floor( car.GetSpeedometer() ).ToString() );

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
		
		if ( next_gear > m_VehicleGearCount )
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshBikeHud - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshCarHud
	// ------------------------------------------------------------	
	private void RefreshCarHud( ExpansionCarScript car, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshCarHud - Start");
		#endif

		float rpm_value = ( car.EngineGetRPM() / car.EngineGetRPMMax() ) ;
		float rpm_value_red = ( car.EngineGetRPMRedline() / car.EngineGetRPMMax() ) ;
		float speed_value = ( car.GetSpeedometer() / 200 );
		
		m_VehicleRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_VehicleSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		m_VehicleSpeedValue.SetText( Math.Floor( car.GetSpeedometer() ).ToString() );

		int engaged_gear = 0; // car.GetController().GetGear();	
		
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

		m_VehicleNextGearValue.SetText( m_VehicleGearTable.Get( next_gear ) );
		m_VehiclePrevGearValue.SetText( m_VehicleGearTable.Get( prev_gear ) );
			
		m_VehicleFuelPointer.SetRotation( 0, 0, car.GetFluidFraction( CarFluid.FUEL ) * 260 - 130, true );
		m_VehicleTemperaturePointer.SetRotation( 0, 0, -1 * car.GetFluidFraction( CarFluid.COOLANT ) * 260 + 130, true );

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshCarHud - End");
		#endif
	}
	
	//============================================
	// Expansion Update
	//============================================
	override void Update( float timeslice )
	{
		super.Update(timeslice);

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::Update - Start");
		#endif
		
		//! Player Tags
		if (GetExpansionSettings().GetGeneral() && GetExpansionSettings().GetGeneral().EnablePlayerTags )
		{
			RefreshPlayerTags();
			ShowPlayerTag(timeslice);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::Update - End");
		#endif
	}
	
	//============================================
	// Override RefreshPlayerTags
	//============================================
	override void RefreshPlayerTags()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshPlayerTags - Start");
		#endif
		if ( GetGame().GetPlayer() )
		{
			
			int viewrange = GetExpansionSettings().GetGeneral().PlayerTagViewRange;
			bool found = false;
			vector head_pos = GetGame().GetCurrentCameraPosition();
			float distance;
			foreach ( Man player : ClientData.m_PlayerBaseList )
			{
				vector target_player = player.GetPosition();
				distance = vector.Distance( head_pos, target_player );
				
				target_player[1] = target_player[1] + 1.2;
				
				if ( distance <= viewrange && player != GetGame().GetPlayer() )
				{
					vector screen_pos = GetGame().GetScreenPosRelative( target_player );
					vector end_pos = head_pos + GetGame().GetCurrentCameraDirection() * 25;
					RaycastRVParams params = new RaycastRVParams( head_pos, end_pos, GetGame().GetPlayer(), 0 );
					params.sorted = true;
					
					array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
					DayZPhysics.RaycastRVProxy( params, results );
					if ( results.Count() > 0 )
					{
						if ( results.Get( 0 ).obj == player )
						{
							m_CurrentTaggedPlayer = PlayerBase.Cast( player );
							found = true;
						}
					}
				}
			}
			if ( !found )
			{
				m_CurrentTaggedPlayer = null;
			}
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshPlayerTags - End");
		#endif
	}
	
	//============================================
	// Override ShowPlayerTag
	//============================================
	override void ShowPlayerTag( float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::ShowPlayerTag - Start");
		#endif
		if ( m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() )
		{
			if ( !m_PlayerTag )
			{
				m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/hud/expansion_hud_player_tag.layout");
				m_PlayerTagText = TextWidget.Cast( m_PlayerTag.FindAnyWidget( "TagText" ) );
				m_PlayerTagIcon = ImageWidget.Cast( m_PlayerTag.FindAnyWidget( "TagIcon" ) );
			}

			m_PlayerSpineIndex = m_CurrentTaggedPlayer.GetBoneIndex( "Spine2" );
			vector player_pos = m_CurrentTaggedPlayer.GetBonePositionWS( m_PlayerSpineIndex );
			vector screen_pos = GetGame().GetScreenPosRelative( player_pos );
			
			if ( screen_pos[2] > 0 )
			{
				if ( screen_pos[0] > 0 && screen_pos[0] < 1 )
				{
					if ( screen_pos[1] > 0 && screen_pos[1] < 1 )
					{
						m_PlayerTagText.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1 ) );
						m_PlayerTag.SetPos( 0.55, 0.55 );
						m_PlayerTagText.SetText( m_CurrentTaggedPlayer.GetIdentityName() );
						m_PlayerTagIcon.SetAlpha( Math.Clamp( m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1 ) );
						m_PlayerTagIcon.Show( true );
						return;
					}
				}
			}
		}
		
		if ( m_PlayerTag )
		{
			float new_alpha = Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 );
			m_PlayerTagText.SetAlpha( Math.Clamp( m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1 ) );
			m_PlayerTagIcon.SetAlpha( Math.Clamp( m_PlayerTagIcon.GetAlpha() - timeslice * 10, 0, 1 ) );
			if ( new_alpha == 0 )
			{
				m_PlayerTagText.SetText( "" );
				m_PlayerTagIcon.Show( false );
				m_CurrentTaggedPlayer = null;
			}
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::ShowPlayerTag - End");
		#endif
	}
}
