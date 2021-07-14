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
	ExpansionVehicleNetworkMode VehicleSync;				//! 0 = SERVER_ONLY | 1 = PREDICTION | 2 = CLIENT

	ExpansionVehicleKeyStartMode VehicleRequireKeyToStart; 	//! 0 = Disabled | 1 = Require key to start the engine (will check hands, cargo, inventory) | 2 = check only in the hand
	bool VehicleRequireAllDoors;							//! If enabled, you will need all the doors to lock/unlock the car
	bool VehicleLockedAllowInventoryAccess;					//! If enabled, will be able to see the vehicle inventory regardless of the car have all his doors
	bool VehicleLockedAllowInventoryAccessWithoutDoors;		//! If enabled, will be able to see the vehicle inventory only if at least one car door is missing

	int MasterKeyPairingMode;								//! -1 = infinite | 0 = disabled | 1 = limited (will use MasterKeyUses) | 2 = renewable with a electronicrepairkit and KeyGrinder (use MasterKeyUses) | 3 = renewable only with a KeyGrinder (use MasterKeyUses)
	int MasterKeyUses;										//! Amount of times the masterkey can pair unpaired keys

	bool CanPickLock;
	autoptr TStringArray PickLockTools;
	float PickLockChancePercent;
	int PickLockTimeSeconds;
	float PickLockToolDamagePercent;

	bool CanChangeLock;
	autoptr TStringArray ChangeLockTools;
	int ChangeLockTimeSeconds;
	float ChangeLockToolDamagePercent;

	bool EnableWindAerodynamics;							//! If enabled, wind simulation will be enabled
	bool EnableTailRotorDamage;								//! If enabled, the rotor will be damageable

	bool PlayerAttachment;									//! If enabled, allow players to be attached to vehicles while in movement
	bool Towing;											//! If enabled, allow vehicle to tow other vehicles

	bool EnableHelicopterExplosions;						//! If enabled, allow Helicopters to explode
	bool DisableVehicleDamage;								//! If disabled, vehicles (cars, trucks) won't take any damages
	float VehicleCrewDamageMultiplier;						//! Damage multiplier for the crew. How fast they will blackout or die.
	float VehicleSpeedDamageMultiplier;						//! Damage multiplier for the speed of the car. Above 0 is weaker and below 0 is stronger.

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionVehicleSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::ExpansionVehicleSettings - Start");
		#endif

		PickLockTools = new TStringArray;
		ChangeLockTools = new TStringArray;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::ExpansionVehicleSettings - End");
		#endif
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
		//! TODO: Need to find where in the code it tries to copy from itself
		if ( s == this )
			return;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - Start");
		#endif

		VehicleSync = s.VehicleSync; // Not ready

		VehicleRequireKeyToStart = s.VehicleRequireKeyToStart;
		VehicleRequireAllDoors = s.VehicleRequireAllDoors;
		VehicleLockedAllowInventoryAccess = s.VehicleLockedAllowInventoryAccess;
		VehicleLockedAllowInventoryAccessWithoutDoors = s.VehicleLockedAllowInventoryAccessWithoutDoors;

		MasterKeyPairingMode = s.MasterKeyPairingMode;
		MasterKeyUses = s.MasterKeyUses;

		CanPickLock = s.CanPickLock;
		PickLockChancePercent = s.PickLockChancePercent;
		PickLockTimeSeconds = s.PickLockTimeSeconds;
		PickLockToolDamagePercent = s.PickLockToolDamagePercent;

		PickLockTools.Copy(s.PickLockTools);

		CanChangeLock = s.CanChangeLock;
		ChangeLockTimeSeconds = s.ChangeLockTimeSeconds;
		ChangeLockToolDamagePercent = s.ChangeLockToolDamagePercent;

		ChangeLockTools.Copy(s.ChangeLockTools);
		
		EnableWindAerodynamics = s.EnableWindAerodynamics;
		EnableTailRotorDamage = s.EnableTailRotorDamage;

		PlayerAttachment = s.PlayerAttachment;
		Towing = s.Towing;

		EnableHelicopterExplosions = s.EnableHelicopterExplosions;
		
		DisableVehicleDamage = s.DisableVehicleDamage;
		VehicleCrewDamageMultiplier = s.VehicleCrewDamageMultiplier;
		VehicleSpeedDamageMultiplier = s.VehicleSpeedDamageMultiplier;

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

		VehicleRequireKeyToStart = ExpansionVehicleKeyStartMode.REQUIREDINVENTORY;
		VehicleRequireAllDoors = true;
		VehicleLockedAllowInventoryAccess = false;
		VehicleLockedAllowInventoryAccessWithoutDoors = true;

		MasterKeyPairingMode = 2;
		MasterKeyUses = 2;

		CanPickLock = false;
		PickLockChancePercent = 40;
		PickLockTimeSeconds = 120;
		PickLockToolDamagePercent = 10;
		PickLockTools.Insert( "Lockpick" );

		CanChangeLock = false;
		ChangeLockTimeSeconds = 120;
		ChangeLockToolDamagePercent = 10;
		ChangeLockTools.Insert( "Screwdriver" );

		EnableWindAerodynamics = false; // Not ready, need tweaking
		EnableTailRotorDamage = true;

		EnableHelicopterExplosions = true;
		
		#ifdef EXPANSION_PLAYER_ATTACHMENT
		PlayerAttachment = true;
		s_ExpansionPlayerAttachment = true;
		#else
		PlayerAttachment = false;
		s_ExpansionPlayerAttachment = false;
		#endif

		Towing = true;
		
		DisableVehicleDamage = false;
		VehicleCrewDamageMultiplier = 1.0;
		VehicleSpeedDamageMultiplier = 0.0;
	}
	
	override string SettingName()
	{
		return "Vehicle Settings";
	}
};

static bool s_ExpansionPlayerAttachment;