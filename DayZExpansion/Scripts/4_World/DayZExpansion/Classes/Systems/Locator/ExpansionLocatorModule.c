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
	protected autoptr array<ref ExpansionLocatorArray> m_AreaArray;
	protected string m_CurrentAreaName;
	protected autoptr ExpansionLocatorUI m_UICallback;
		
	// ------------------------------------------------------------
	// ExpansionLocatorModule Constructor
	// ------------------------------------------------------------
	void ExpansionLocatorModule()
	{
		m_AreaArray = new array< ref ExpansionLocatorArray >;
		m_CurrentAreaName = "";
		m_UICallback = null;
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionLocatorModule()
	{
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{		
		if ( IsMissionClient() && !m_UICallback )
		{
			m_UICallback = new ExpansionLocatorUI();
			GetWorldLocations();
			
			GetGame().GameScript.Call( this, "ThreadLocator", null );
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
		switch ( type )
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

			if ( GetExpansionSettings().GetGeneral() && !GetExpansionSettings().GetGeneral().PlayerLocationNotifier )
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
			return;
		
		ref ExpansionLocatorArray shortestLocation;
		float shortestDistance = 2147483645;

		for ( int j = 0; j < m_AreaArray.Count(); ++j )
		{
			float distance = vector.Distance( player.GetPosition() * "1 0 1", m_AreaArray[j].position );

			if ( distance <= shortestDistance )
			{
				shortestDistance = distance;
				shortestLocation = m_AreaArray[j];
			}
		}

		if ( !shortestLocation )
		{
			m_CurrentAreaName = "";
		} 
		else if ( shortestDistance <= GetRadius( shortestLocation.type ) )
		{
			if ( m_CurrentAreaName != shortestLocation.name )
			{
				m_CurrentAreaName = shortestLocation.name;
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
        int classNamesCount = GetGame().ConfigGetChildrenCount( location_config_path );
        
		for ( int l = 0; l < classNamesCount; ++l ) 
        {
			string location_class_name;
			GetGame().ConfigGetChildName( location_config_path, l, location_class_name );

			string location_class_name_path = location_config_path + " " + location_class_name;

			string location_type_path = location_class_name_path + " type";
			string location_name_path = location_class_name_path + " name";
			string location_position_path = location_class_name_path + " position";

			string location_type;
			string location_name;
			GetGame().ConfigGetText( location_type_path, location_type );
			GetGame().ConfigGetText( location_name_path, location_name );

			TFloatArray location_position = new TFloatArray;
			GetGame().ConfigGetFloatArray( location_position_path, location_position );
			
			if (location_position.Count() != 2)
			{
				Error("ExpansionLocatorModule::GetWorldLocations location_position.Count() != 2 count : " + location_position.Count());
				continue;
			}

			if ( location_type.Contains( "LocalOffice" ) )
				continue;
			
			if ( location_type.Contains( "ViewPoint" ) )
				continue;
			
			if ( location_type.Contains( "RailroadStation" ) )
				continue;

			if ( location_class_name.Contains( "ZalivGuba" ) )
				continue;
			
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