/**
 * ExpansionVehicleSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionVehicleSettings
 * @brief		Vehicle settings class
 **/
class ExpansionVehicleSettings: ExpansionSettingBase
{
	ExpansionVehicleNetworkMode VehicleSync;

	int VehicleRequireKeyToStart; // 1.02: 0 = no, 1 = yes. TODO: 2 = in hands only
	bool VehicleRequireAllDoors;
	bool VehicleLockedAllowInventoryAccess;
	bool VehicleLockedAllowInventoryAccessWithoutDoors;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionVehicleSettings()
	{
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::HandleRPC - Start");
		#endif
		ExpansionVehicleSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionVehicleSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Vehicle.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::HandleRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionVehicleSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Vehicle, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionVehicleSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionVehicleSettings s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - Start");
		#endif

		VehicleSync = s.VehicleSync;

		VehicleRequireKeyToStart = s.VehicleRequireKeyToStart;
		VehicleRequireAllDoors = s.VehicleRequireAllDoors;
		VehicleLockedAllowInventoryAccess = s.VehicleLockedAllowInventoryAccess;
		VehicleLockedAllowInventoryAccessWithoutDoors = s.VehicleLockedAllowInventoryAccessWithoutDoors;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - End");
		#endif
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
		EXPrint("ExpansionVehicleSettings::Load - Start");
		#endif
		
		m_IsLoaded = true;
		
		if ( FileExist( EXPANSION_VEHICLE_SETTINGS ) )
		{
			JsonFileLoader<ExpansionVehicleSettings>.JsonLoadFile( EXPANSION_VEHICLE_SETTINGS, this );
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionVehicleSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Load - End - Not Loaded");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Save - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionVehicleSettings>.JsonSaveFile( EXPANSION_VEHICLE_SETTINGS, this );
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionVehicleSettings::Save - Settings saved!");
			#endif

			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Save - End");
		#endif
		return false;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_General.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionVehicleSettings] Loading default settings..");
		#endif

		VehicleRequireKeyToStart = true;
		VehicleRequireAllDoors = true;
		VehicleLockedAllowInventoryAccess = false;
		VehicleLockedAllowInventoryAccessWithoutDoors = true;
	}
}