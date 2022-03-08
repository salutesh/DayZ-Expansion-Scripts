/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------	
	override void Init( Widget hud_panel_widget )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::Init - Start");
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

		super.ShowVehicleInfo();

		ExpansionHelicopterScript d_helicopter;
		ExpansionBoatScript d_boat;

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
		} else if ( Class.CastTo( d_boat, m_CurrentVehicle ) )
		{
			float d_b_rpm_value_red = ( d_boat.EngineGetRPMRedline() / d_boat.EngineGetRPMMax() ) ;
			m_BoatRPMDial.SetMaskProgress( d_b_rpm_value_red );
			m_BoatRPMRedline.SetMaskProgress( 1 - d_b_rpm_value_red );

			m_BoatGearPanel.Show( true );

			m_BoatPanel.Show( true );

			m_VehiclePanel.Show( false );
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

		super.HideVehicleInfo();

		m_BoatPanel.Show( false );
		m_HelicopterPanel.Show( false );

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

		if ( GetGame().GetUIManager().GetMenu() )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("IngameHud::RefreshVehicleHud - Second Return");
			#endif

			return;
		}

		ExpansionHelicopterScript d_helicopter;
		if ( Class.CastTo( d_helicopter, m_CurrentVehicle ) )
		{
			m_HelicopterPanel.Show( true );
			RefreshHelicopterHud_Deprecated( d_helicopter, timeslice );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionIngameHud::RefreshVehicleHud - Helicopter");
			#endif

			return;
		}

		ExpansionBoatScript d_boat;
		if ( Class.CastTo( d_boat, m_CurrentVehicle ) )
		{
			m_BoatPanel.Show( true );
			RefreshBoatHud_Deprecated( d_boat, timeslice );

			return;
		}

		m_BoatPanel.Show( false );
		m_HelicopterPanel.Show( false );

		super.RefreshVehicleHud( timeslice );

		if (m_CurrentVehicle && !m_CurrentVehicle.m_State.m_IsSync)
		{
			m_VehicleSpeedValue.SetText( "!SYNC" );
			m_VehicleSpeedValue.SetColor( ARGB(255, 255, 0, 0) );
		}
		else
		{
			m_VehicleSpeedValue.SetColor( ARGB(255, 255, 255, 255) );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshVehicleHud - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshHelicopterHud_Deprecated
	// ------------------------------------------------------------	
	private void RefreshHelicopterHud_Deprecated( ExpansionHelicopterScript helicopter, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshHelicopterHud_Deprecated - Start");
		#endif

		float h_speed_value = ( helicopter.GetSpeedometer() / 400 );	
		float h_vert_speed_value = ( GetVelocity(helicopter)[1] / 400 );	
		float h_alt_value = ( helicopter.GetPosition()[1] );
		
		m_HelicopterSpeedPointer.SetRotation( 0, 0, h_speed_value * 360 - 130, true );
		m_HelicopterAltitudePointer.SetRotation( 0, 0, ( h_alt_value / 1600) * 360 - 130, true );
		
		if (!helicopter.m_State.m_IsSync)
		{
			m_HelicopterSpeedValue.SetText( "!SYNC" );
			m_HelicopterSpeedValue.SetColor( ARGB(255, 255, 0, 0) );
		}
		else
		{
			m_HelicopterSpeedValue.SetText( Math.Floor( helicopter.GetSpeedometer() ).ToString() );
			m_HelicopterSpeedValue.SetColor( ARGB(255, 255, 255, 255) );
		}

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
		EXPrint("IngameHud::RefreshHelicopterHud_Deprecated - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshBoatHud_Deprecated
	// ------------------------------------------------------------	
	private void RefreshBoatHud_Deprecated( ExpansionBoatScript boat, float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("IngameHud::RefreshBoatHud - Start");
		#endif

		float rpm_value = ( boat.EngineGetRPM() / boat.EngineGetRPMMax() ) ;
		float rpm_value_red = ( boat.EngineGetRPMRedline() / boat.EngineGetRPMMax() ) ;
			
		float speed_value = ( boat.GetSpeedometer() / 400 );	

		m_BoatRPMPointer.SetRotation( 0, 0, rpm_value * 270 - 130, true );
		m_BoatSpeedPointer.SetRotation( 0, 0, speed_value * 260 - 130, true );
		
		if (!boat.m_State.m_IsSync)
		{
			m_BoatSpeedValue.SetText( "!SYNC" );
			m_BoatSpeedValue.SetColor( ARGB(255, 255, 0, 0) );
		}
		else
		{
			m_BoatSpeedValue.SetText( Math.Floor( boat.GetSpeedometer() ).ToString() );
			m_BoatSpeedValue.SetColor( ARGB(255, 255, 255, 255) );
		}

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
		EXPrint("IngameHud::RefreshBoatHud_Deprecated - End");
		#endif
	}
};
#endif
