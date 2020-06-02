/**
 * ExpansionLocatorModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLocatorModule
 * @brief		This class handle expansion locator system
 **/
class ExpansionLocatorModule: JMModuleBase
{
	protected ref array<ref ExpansionLocatorArray> m_AreaArray;
	protected string m_CurrentAreaName;
	protected ref ExpansionLocatorUI m_UICallback;
		
	// ------------------------------------------------------------
	// ExpansionLocatorModule Constructor
	// ------------------------------------------------------------
	void ExpansionLocatorModule()
	{
		m_AreaArray = new array< ref ExpansionLocatorArray >;
		m_CurrentAreaName = "";
		m_UICallback = NULL;
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionLocatorModule()
	{
		delete m_AreaArray;
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{		
		if ( IsMissionClient() && m_UICallback == NULL )
		{
			m_UICallback = new ExpansionLocatorUI();
			GetWorldLocations();
			
			GetGame().GameScript.Call( this, "ThreadLocator", NULL );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule IsServer
	// ------------------------------------------------------------
	override bool IsServer()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule GetRadius
	// ------------------------------------------------------------
	private int GetRadius( string type )
	{
		switch( type )
		{
			case "Capital":
			{
				return 1000;
			}
			case "City":
			{
				return 500;
			}
			case "Village":
			{
				return 200;
			}
			case "Camp":
			{
				return 100;
			}
			case "Local":
			{
				return 100;
			}
			default:
			{
				return 100;
			}
		}
		return 100;
	}

	// ------------------------------------------------------------
	// ExpansionLocatorModule GetCurrentAreaName
	// ------------------------------------------------------------
	private string GetCurrentAreaName()
	{
		return m_CurrentAreaName;
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule OnUpdate
	// ------------------------------------------------------------
	private void ThreadLocator()
	{
		int nmbError = 0;
		while ( true )
		{
			if ( !GetGame() )
			{
				return;
			}
			
			if ( nmbError > 20 )
			{
				return;
			}

			if ( GetExpansionSettings() && !GetExpansionSettings().GetGeneral().PlayerLocationNotifier )
			{
				nmbError++;
				Sleep( 10000 );
				continue;
			}

			CheckPlayer();

			Sleep( 2000 );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule CheckPlayer
	// ------------------------------------------------------------
	void CheckPlayer()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player )
		{
			return;
		}
		
		ExpansionLocatorArray currentLocation;
		float shortestDistance = 1000000;

		for ( int j = 0; j < m_AreaArray.Count(); j++ )
		{
			float distance = vector.Distance( player.GetPosition(), m_AreaArray[j].position );

			if ( distance <= shortestDistance )
			{
				shortestDistance = distance;
				currentLocation = m_AreaArray[j];
			}
		}

		if ( currentLocation == null )
		{
			m_CurrentAreaName = "";
		} 
		else if ( m_CurrentAreaName != currentLocation.name )
		{
			if ( shortestDistance <= GetRadius( currentLocation.type ) )
			{
				m_CurrentAreaName = currentLocation.name;

				m_UICallback.OnShowCityClient( m_CurrentAreaName );
			} 
			else
			{
				m_CurrentAreaName = "";
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule GetWorldLocations
	// ------------------------------------------------------------
	private void GetWorldLocations()
    {    
        string location_config_path = "CfgWorlds " + GetGame().GetWorldName() + " Names";
        int classNamesCount = g_Game.ConfigGetChildrenCount( location_config_path );
        
		for ( int l = 0; l < classNamesCount; ++l ) 
        {
			string location_class_name;
			g_Game.ConfigGetChildName( location_config_path, l, location_class_name );

			string location_class_name_path = location_config_path + " " + location_class_name;

			string location_type_path = location_class_name_path + " " + "type";
			string location_name_path = location_class_name_path + " " + "name";
			string location_position_path = location_class_name_path + " " + "position";

			string location_type;
			string location_name;
			GetGame().ConfigGetText( location_type_path, location_type );
			GetGame().ConfigGetText( location_name_path, location_name );

			vector location_position = GetGame().ConfigGetVector( location_position_path );

			if ( location_type.Contains( "LocalOffice" ) )
				return;
			
			if ( location_type.Contains( "ViewPoint" ) )
				return;
			
			if ( location_type.Contains( "RailroadStation" ) )
				return;

			if ( location_class_name.Contains( "ZalivGuba" ) )
				return;
			
			if ( location_class_name.Contains( "Dubovo" ) )
				location_type = "Village";
			
			if ( location_class_name.Contains( "Novoselki" ) )
				location_type = "Village";
			
			if ( location_class_name.Contains( "Vysotovo" ) )
				location_type = "Village";

			m_AreaArray.Insert( new ExpansionLocatorArray( Vector( location_position[0], 0, location_position[1] ), location_name, location_type ) );
		}
    }
}