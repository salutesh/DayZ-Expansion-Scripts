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

	bool EnableWindAerodynamics;
	bool EnableTailRotorDamage;

	bool PlayerAttachment;
	bool Towing;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionVehicleSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::OnRecieve - Start");
		#endif
		ExpansionVehicleSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionVehicleSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		Update( this );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionVehicleSettings thisSetting = this;

		ctx.Write( thisSetting );
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
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

		VehicleSync = s.VehicleSync; // Not ready

		VehicleRequireKeyToStart = s.VehicleRequireKeyToStart;
		VehicleRequireAllDoors = s.VehicleRequireAllDoors;
		VehicleLockedAllowInventoryAccess = s.VehicleLockedAllowInventoryAccess;
		VehicleLockedAllowInventoryAccessWithoutDoors = s.VehicleLockedAllowInventoryAccessWithoutDoors;
		
		EnableWindAerodynamics = s.EnableWindAerodynamics;
		EnableTailRotorDamage = s.EnableTailRotorDamage;

		PlayerAttachment = s.PlayerAttachment;
		Towing = s.Towing;

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
			Print("[ExpansionVehicleSettings] Loading settings");

			JsonFileLoader<ExpansionVehicleSettings>.JsonLoadFile( EXPANSION_VEHICLE_SETTINGS, this );
	
			Update( this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionVehicleSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		Update( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Load - End - Not Loaded");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionVehicleSettings] Saving settings");

		JsonFileLoader<ExpansionVehicleSettings>.JsonSaveFile( EXPANSION_VEHICLE_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Vehicle.Invoke();

		#ifdef EXPANSION_PLAYER_ATTACHMENT
		s_ExpansionPlayerAttachment = PlayerAttachment;
		#else
		s_ExpansionPlayerAttachment = false;
		#endif
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		Print("[ExpansionVehicleSettings] Loading default settings");

		VehicleSync = ExpansionVehicleNetworkMode.PREDICTION;

		VehicleRequireKeyToStart = 1;
		VehicleRequireAllDoors = true;
		VehicleLockedAllowInventoryAccess = false;
		VehicleLockedAllowInventoryAccessWithoutDoors = true;

		EnableWindAerodynamics = false; // Not ready, need tweaking
		EnableTailRotorDamage = true;

		PlayerAttachment = true;
		Towing = true;
	}
}

static bool s_ExpansionPlayerAttachment;