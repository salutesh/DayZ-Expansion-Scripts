/**
 * ExpansionBookTabStatus.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabStatus extends ExpansionBookTabBase
{
	protected int COLOR_EXPANSION_STATUS_BOOKMARK_ICON = ARGB( 255, 231, 76, 60 );
	protected int COLOR_EXPANSION_STATUS_TOXICITY_NORMAL = ARGB( 255, 255, 255, 255 );
	protected int COLOR_EXPANSION_STATUS_TOXICITY_POISONED = ARGB( 255, 255, 255, 255 );
	protected int COLOR_EXPANSION_STATUS_TOXICITY_HPOISONED = ARGB( 255, 255, 255, 255 );
	
	protected TextWidget m_ProfilePlayerKillsVal;
	protected TextWidget m_ProfileZombieKillsVal;
	protected TextWidget m_ProfileAnimalKillsVal;
	protected TextWidget m_ProfileTimeSurviedVal;
	protected TextWidget m_ProfileLongestShotVal;
	protected TextWidget m_ProfileDistanceTravelledVal;
	protected TextWidget m_ProfileWeightVal;
	protected TextWidget m_stat_HealthValue;
	protected ProgressBarWidget m_stat_HealthBar;
	protected TextWidget m_stat_BloodValue;
	protected ProgressBarWidget m_stat_BloodBar;
	protected TextWidget m_stat_WaterValue;
	protected ProgressBarWidget m_stat_WaterBar;
	protected TextWidget m_stat_EnergyValue;
	protected ProgressBarWidget m_stat_EnergyBar;
	protected TextWidget m_stat_StaminaValue;
	protected ProgressBarWidget m_stat_StaminaBar;
	protected PlayerPreviewWidget m_book_PlayerPreview;
	
	protected float m_Time = 0;
	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected vector m_CharacterOrientation;
	
	protected int m_Health;
	protected int m_Blood;
	protected int m_Water;
	protected int m_Energy;
	protected int m_Stamina;
	protected int m_Karma;
	
	protected float m_Distance;
	protected float m_Playtime;
	protected float m_PlayersKilled;
	protected float m_InfectedKilled;
	protected float m_AnimalsKilled;
	protected float m_LongestShot;
	
	protected int m_Weight;
	
	protected ref ExpansionMonitorModule m_MonitorModule;
	
	// ------------------------------------------------------------
	// ExpansionBookTabStatus Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabStatus()
	{
		SetButtonIconPath( "DayZExpansion/GUI/icons/hud/statistic_64x64.edds" );
		SetButtonText( "#STR_BOOK_STATUSMENU" );
		SetButtonColor( COLOR_EXPANSION_STATUS_BOOKMARK_ICON );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_status.layout" );
	}
	
	// ------------------------------------------------------------
	// Override OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		m_book_PlayerPreview					= PlayerPreviewWidget.Cast( m_RootLayout.FindAnyWidget( "player_preview" ) );
		
		// CHARACTER
		m_ProfileTimeSurviedVal					= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_time_value" ) );
		m_ProfilePlayerKillsVal					= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_player_kills_value" ) );
		m_ProfileZombieKillsVal					= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_zombie_kills_value" ) );
		m_ProfileAnimalKillsVal					= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_animal_kills_value" ) );
		m_ProfileLongestShotVal					= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_shot_value" ) );
		m_ProfileDistanceTravelledVal			= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_distance_value" ) );
		m_ProfileWeightVal						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "profile_weight_value" ) );
		
		// STATS
		m_stat_HealthValue						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "stat_health_value" ) );
		m_stat_HealthBar						= ProgressBarWidget.Cast( m_RootLayout.FindAnyWidget( "stat_health_bar" ) );
		m_stat_BloodValue						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "stat_blood_value" ) );
		m_stat_BloodBar							= ProgressBarWidget.Cast( m_RootLayout.FindAnyWidget( "stat_blood_bar" ) );
		m_stat_WaterValue						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "stat_water_value" ) );
		m_stat_WaterBar							= ProgressBarWidget.Cast( m_RootLayout.FindAnyWidget( "stat_water_bar" ) );
		m_stat_EnergyValue						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "stat_energy_value" ) );
		m_stat_EnergyBar						= ProgressBarWidget.Cast( m_RootLayout.FindAnyWidget( "stat_energy_bar" ) );
		m_stat_StaminaValue						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "stat_stamina_value" ) );
		m_stat_StaminaBar						= ProgressBarWidget.Cast( m_RootLayout.FindAnyWidget( "stat_stamina_bar" ) );
		
		m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		m_book_PlayerPreview.SetPlayer(m_Player);
		
		m_Time = 0;

		UpdateValues();
	}
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		return GetExpansionSettings().GetBook().EnableStatusTab;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonDown
	// ------------------------------------------------------------
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if ( w == m_book_PlayerPreview )
		{
			GetGame().GetMousePos( m_CharacterRotationX, m_CharacterRotationY );
			GetGame().GetDragQueue().Call( this, "UpdateRotation" );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonUp
	// ------------------------------------------------------------
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		super.OnMouseButtonUp( w, x, y, button);
		
		if ( w == m_book_PlayerPreview )
		{
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		m_Time += timeslice;
		if ( m_Time >= 2.0 )
		{
			UpdateValues();
			m_Time = 0;
		}
	}
	
	// ------------------------------------------------------------
	// Override OnHidedLayout
	// ------------------------------------------------------------
	override void OnHidedLayout()
	{
		m_Time = 0;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateRotation
	// ------------------------------------------------------------
	void UpdateRotation( int mouse_x, int mouse_y, bool is_dragging )
	{
		vector orientation = m_CharacterOrientation;
		orientation[1] = orientation[1] - ( m_CharacterRotationX - mouse_x );
		m_book_PlayerPreview.SetModelOrientation( orientation );

		if ( !is_dragging )
		{	
			m_CharacterOrientation = orientation;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion String BloodType
	// ------------------------------------------------------------
	string BloodType(PlayerBase player)
	{
		string blood_name;
		bool positive;
		string blood_type_name;
		
		if (m_Player.HasBloodTypeVisible())
		{
			blood_type_name = BloodTypes.GetBloodTypeName( m_Player.GetBloodType(), blood_name, positive );
		}
		else
		{
			blood_type_name = "Unknown";
		}
		
		return blood_type_name;
	}
		
	// ------------------------------------------------------------
	// Expansion GetValuesFromMonitor
	// ------------------------------------------------------------
	void GetValuesFromMonitor()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabStatus::GetValuesFromMonitor:: - Start");
		#endif		
		
		if ( !m_MonitorModule && !Class.CastTo( m_MonitorModule, GetModuleManager().GetModule( ExpansionMonitorModule ) ) )
			return;
		
		ExpansionSyncedPlayerStats player_stats = m_MonitorModule.GetStats();
		if ( !player_stats )
			return;

		m_Health = player_stats.m_Health;
		m_Blood = player_stats.m_Blood;
		m_Water = player_stats.m_Water;
		m_Energy = player_stats.m_Energy;
		m_Stamina = player_stats.m_Stamina;
		m_Karma = player_stats.m_Karma;
		
		m_Distance = player_stats.m_Distance;
		m_Playtime = player_stats.m_Playtime;
		m_PlayersKilled = player_stats.m_PlayersKilled;
		m_InfectedKilled = player_stats.m_InfectedKilled;
		m_AnimalsKilled = player_stats.m_AnimalsKilled;
		m_LongestShot = player_stats.m_LongestShot;
		m_Weight = player_stats.m_Weight;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateValues
	// ------------------------------------------------------------
	void UpdateValues()
	{
		GetValuesFromMonitor();
		
		// Health
		m_stat_HealthValue.SetText( m_Health.ToString() + "%" );
		m_stat_HealthBar.SetCurrent( m_Health );
		
		// Blood
		m_stat_BloodValue.SetText( m_Blood.ToString() + "%" );
		m_stat_BloodBar.SetCurrent( m_Blood );
		
		// Water
		m_stat_WaterValue.SetText( m_Water.ToString() + "%" );
		m_stat_WaterBar.SetCurrent( m_Water );
		
		// Energy
		m_stat_EnergyValue.SetText( m_Energy.ToString() + "%" );
		m_stat_EnergyBar.SetCurrent( m_Energy );
		
		// StaminaUpEnd
		m_stat_StaminaValue.SetText( m_Stamina.ToString() + "%" );
		m_stat_StaminaBar.SetCurrent( m_Stamina );

		// Profile Stats
		m_ProfileTimeSurviedVal.SetText( GetTimeString( m_Playtime ) );
		m_ProfilePlayerKillsVal.SetText( GetValueString( m_PlayersKilled ) );
		m_ProfileZombieKillsVal.SetText( GetValueString( m_InfectedKilled ) );
		m_ProfileAnimalKillsVal.SetText( GetValueString( m_AnimalsKilled ) );
		m_ProfileLongestShotVal.SetText( GetDistanceString( m_LongestShot ) );
		m_ProfileDistanceTravelledVal.SetText( GetDistanceString( m_Distance ) );
		m_ProfileWeightVal.SetText( GetWeightString( m_Weight ) );
	}
	
	// ------------------------------------------------------------
	// Expansion GetTimeString
	// ------------------------------------------------------------
	protected string GetTimeString( float total_time )
	{
		string time_string;

		if( total_time < 0 )
		{
			time_string =  "0" + " " + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";
			return time_string;
		}
	
		int time_seconds = total_time; 									//convert total time to int
		
		int hours = time_seconds / 3600;
		if ( hours > 0 )
		{
			time_string += GetValueString( hours ) + " " + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_HOURS";			//hours
		}
		
		time_string += " ";												//separator
		
		int minutes = ( time_seconds % 3600 ) / 60;
		time_string += GetValueString( minutes ) + " " + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_MINUTES";			//minutes
		
		return time_string;
	}
	
	// ------------------------------------------------------------
	// Expansion GetWeightString
	// ------------------------------------------------------------
	protected string GetWeightString(int weight, bool grams_only = false)
	{
		string weight_string;
		
		if( weight < 0 )
		{
			weight_string =  "0" + " " + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_GRAMS";
			return weight_string;
		}
		
		int weight_grams = weight;
		
		int kilograms = weight_grams / 1000;
		if ( kilograms > 0 && !grams_only )
		{
			weight_string += GetValueString( kilograms ) + " " + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_KG";				//kilograms
			weight_string += " ";												//separator
		}
		else
		{
			weight_string += GetValueString( weight_grams ) + " " + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_GRAMS";	//grams
		}
		
		return weight_string;
	}
	
	// ------------------------------------------------------------
	// Expansion GetDistanceString
	// ------------------------------------------------------------
	protected string GetDistanceString( float total_distance, bool meters_only = false )
	{
		string distance_string;
		
		if( total_distance < 0 )
		{
			distance_string =  "0" + " "  + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_METERS";
			return distance_string;
		}
	
		int distance_meters = total_distance;
		
		int kilometers = distance_meters / 1000;
		if ( kilometers > 0 && !meters_only )
		{
			distance_string += GetValueString( kilometers ) + " " + "#STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_KM";				//kilometers
			distance_string += " ";												//separator
		}
		else
		{
			distance_string += GetValueString( distance_meters ) + " " + " #STR_EXPANSION_BOOK_STATUS_CHARACTER_STATS_METERS";	//meters
		}
		
		return distance_string;
	}
	
	// ------------------------------------------------------------
	// Expansion GetValueString
	// ------------------------------------------------------------
	protected string GetValueString( float total_value )
	{
		if( total_value < 0 )
			return "0";
	
		int value = total_value;
		string out_string;
		
		if ( value >= 1000 )
		{
			string value_string = value.ToString();
			
			int count;		
			int first_length = value_string.Length() % 3;		//calculate position of the first separator
			if ( first_length > 0 )
			{
				count = 3 - first_length;
			}
			
			for ( int i = 0; i < value_string.Length(); ++i )
			{
				out_string += value_string.Get( i );
				count ++;
				
				if ( count >= 3 )
				{
					out_string += " ";			//separator
					count = 0;
				}
			}
		}
		else
		{
			out_string = value.ToString();
		}
		
		return out_string;
	}
}