/**
 * ExpansionSafeZoneSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSafeZoneSettingsBase
 * @brief		Safezone settings base class
 **/
class ExpansionSafeZoneSettingsBase: ExpansionSettingBase
{
	bool Enabled;																	// Enable Safezone when set to 1
	int  FrameRateCheckSafeZoneInMs;												// How often in ms the server need to check if the player is inside a Safezone
	autoptr array<ref ExpansionSafeZoneCircle> CircleZones = new array< ref ExpansionSafeZoneCircle >;
	autoptr array<ref ExpansionSafeZonePolygon> PolygonZones = new array< ref ExpansionSafeZonePolygon >;
	
	// ------------------------------------------------------------
	void ExpansionSafeZoneSettingsBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettingsBase::ExpansionSafeZoneSettingsBase - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettingsBase::ExpansionSafeZoneSettingsBase - Start");
		#endif
	}
}

/**@class		ExpansionSafeZoneSettingsBase
 * @brief		Safezone settings v0 class
 **/
class ExpansionSafeZoneSettingsV0: ExpansionSafeZoneSettingsBase
{
	bool EnableVehicleinvincibleInsideSafeZone;
}

/**@class		ExpansionSafeZoneSettings
 * @brief		SafeZone settings class
 **/
class ExpansionSafeZoneSettings: ExpansionSafeZoneSettingsBase
{
	static const int VERSION = 2;

	bool DisableVehicleDamageInSafeZone;

	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettings::OnRecieve - Start");
		#endif

		//Not sent to client under normal operation
		m_IsLoaded = true;

		ExpansionSafeZoneSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionSafeZoneSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionSafeZoneSettings thisSetting = this;

		ctx.Write( thisSetting );
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
	private void CopyInternal(  ExpansionSafeZoneSettings s )
	{
		DisableVehicleDamageInSafeZone = s.DisableVehicleDamageInSafeZone;
		
		ExpansionSafeZoneSettingsBase sb = s;
		CopyInternal( sb );
	}
	
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionSafeZoneSettingsBase s)
	{
		int i = 0;

		Enabled = s.Enabled;
		
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

		bool save;

		bool safezoneSettingsExist = FileExist(EXPANSION_SAFE_ZONES_SETTINGS);

		if (safezoneSettingsExist)
		{
			ExpansionSafeZoneSettings settingsDefault = new ExpansionSafeZoneSettings;
			settingsDefault.Defaults();

			ExpansionSafeZoneSettingsBase settingsBase;

			JsonFileLoader<ExpansionSafeZoneSettingsBase>.JsonLoadFile(EXPANSION_SAFE_ZONES_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionSafeZoneSettingsBase] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_SAFE_ZONES_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 2)
				{
					ExpansionSafeZoneSettingsV0 settings_v0;
					JsonFileLoader<ExpansionSafeZoneSettingsV0>.JsonLoadFile(EXPANSION_SAFE_ZONES_SETTINGS, settings_v0);

					DisableVehicleDamageInSafeZone = settings_v0.EnableVehicleinvincibleInsideSafeZone;
				}
				
				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionSafeZoneSettings>.JsonLoadFile(EXPANSION_SAFE_ZONES_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionSafeZoneSettings] No existing setting file:" + EXPANSION_SAFE_ZONES_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneSettings::Load - End - Loaded: " + safezoneSettingsExist);
		#endif
		
		return safezoneSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionSafeZoneSettings] Saving settings");
		
		JsonFileLoader<ExpansionSafeZoneSettings>.JsonSaveFile( EXPANSION_SAFE_ZONES_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
	#ifdef EXPANSIONMODMARKET
		Enabled = true;
	#else
		Enabled = false;
	#endif
		DisableVehicleDamageInSafeZone = true;
		FrameRateCheckSafeZoneInMs = 5000;


		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		//! Vanilla Maps
		if ( world_name == "chernarusplus" || world_name == "chernarusplusgloom" )
		{
		#ifdef EXPANSIONMODMARKET
			DefaultChernarusSafeZones();
		#endif
		}
		else if ( world_name == "namalsk" )
		{
		#ifdef EXPANSIONMODMARKET
			DefaultNamalskSafeZones();
		#endif
		}
		else if ( world_name == "takistanplus" )
		{
		#ifdef EXPANSIONMODMARKET
			DefaultTakistanSafeZones();
		#endif
		}
	}

#ifdef EXPANSIONMODMARKET
	void DefaultChernarusSafeZones()
	{
		//! Krasnostav Trader Camp
		TVectorArray points = new TVectorArray;
		points.Insert( Vector(12288.9, 142.4, 12804.4) );
		points.Insert( Vector(12068.4, 139.8, 12923.4) );
		points.Insert( Vector(11680.6, 141.1, 12650.6) );
		points.Insert( Vector(11805.3, 146.3, 12258.9) );
		points.Insert( Vector(12327.7, 140.0, 12453.8) );
		PolygonZones.Insert( new ExpansionSafeZonePolygon( points ) );
		//CircleZones.Insert( new ExpansionSafeZoneCircle( Vector(12013.4, 140.0, 12556.7), 700 ) );
		//! Green Mountain Trader Camp
		CircleZones.Insert( new ExpansionSafeZoneCircle( Vector(3728.27, 403, 6003.6), 500 ) );
		//! Kamenka Trader Camp
		CircleZones.Insert( new ExpansionSafeZoneCircle( Vector(1143.14, 6.9, 2423.27), 700 ) );
	}
	
	void DefaultNamalskSafeZones()
	{		
		//! Jalovisko Trader Camp
		CircleZones.Insert( new ExpansionSafeZoneCircle( Vector(8583.67, 29, 10515), 400 ) );
	}
	
	void DefaultTakistanSafeZones()
	{
		//! Marastar Oasis
		CircleZones.Insert( new ExpansionSafeZoneCircle( Vector(4611.26, 4.6, 12334.0), 500 ) );
	}
#endif

	// ------------------------------------------------------------	
	override string SettingName()
	{
		return "Safe-Zone Settings";
	}
};
