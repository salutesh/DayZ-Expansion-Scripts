/**
 * ExpansionSafeZoneSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSafeZoneSettings
 * @brief		SafeZone settings class
 **/
class ExpansionSafeZoneSettings: ExpansionSettingBase
{
	bool Enabled;												// Enable Safezone when set to 1
	bool EnableVehicleinvincibleInsideSafeZone;					// When enabled, Vehicle damage is disabled
	int  FrameRateCheckSafeZoneInMs;							// How often in ms the server need to check if the player is inside a Safezone
	autoptr array<ref ExpansionSafeZoneCircle> CircleZones;		// 
	autoptr array<ref ExpansionSafeZonePolygon> PolygonZones;	// 

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionSafeZoneSettings()
	{
		CircleZones = new array< ref ExpansionSafeZoneCircle >;
		PolygonZones = new array< ref ExpansionSafeZonePolygon >;
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		//Not sent to client

		m_IsLoaded = true;
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		//Not sent to client
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionSafeZoneSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionSafeZoneSettings s )
	{
		int i = 0;

		CircleZones.Clear();
		PolygonZones.Clear();

		for (i = 0; i < s.PolygonZones.Count(); i++)
		{
			PolygonZones.Insert( s.PolygonZones[i] );
		}
		for (i = 0; i < s.CircleZones.Count(); i++)
		{
			CircleZones.Insert( s.CircleZones[i] );
		}

		Enabled = s.Enabled;
		EnableVehicleinvincibleInsideSafeZone = s.EnableVehicleinvincibleInsideSafeZone;
		FrameRateCheckSafeZoneInMs = s.FrameRateCheckSafeZoneInMs;
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool OnLoad()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_SAFE_ZONES_SETTINGS ) )
		{
			JsonFileLoader<ExpansionSafeZoneSettings>.JsonLoadFile( EXPANSION_SAFE_ZONES_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionSafeZoneSettings::Load - End");
			#endif
			
			return true;
		}
		
		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettings::Load - End");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionSafeZoneSettings] Saving settings");
		#endif
		
		JsonFileLoader<ExpansionSafeZoneSettings>.JsonSaveFile( EXPANSION_SAFE_ZONES_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionSafeZoneSettings] Loading default settings");
		#endif
		
		Enabled = false;
		EnableVehicleinvincibleInsideSafeZone = true;
		FrameRateCheckSafeZoneInMs = 5000;

		//Example for adding a safezone with a polygon zone
		TVectorArray points = new TVectorArray;
		points.Insert( Vector(6345, 0, 2181) );
		points.Insert( Vector(6198, 0, 2433) );
		points.Insert( Vector(6565, 0, 2945) );
		points.Insert( Vector(7000, 0, 2521) );
		PolygonZones.Insert( new ExpansionSafeZonePolygon( points ) );

	   	CircleZones.Insert( new ExpansionSafeZoneCircle( Vector(11849.6, 0, 12471.6), 500 ) );
	}

	// ------------------------------------------------------------
	void RemoveSafeZonePolygon(ref ExpansionSafeZonePolygon zone)
	{
		int index = -1;

		for (int i = 0; i < PolygonZones.Count(); ++i)
		{
			if ( PolygonZones[i] == zone )
			{
				index = i;
				break;
			}
		}

		if ( index > -1 )
			PolygonZones.Remove(index);
	}
}