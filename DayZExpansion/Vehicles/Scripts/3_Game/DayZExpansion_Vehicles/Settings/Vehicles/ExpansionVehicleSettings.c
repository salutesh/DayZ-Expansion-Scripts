/**
 * ExpansionVehicleSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionVehicleSettingsBase
 * @brief		Vehicle settings base class
 **/
class ExpansionVehicleSettingsBase : ExpansionSettingBase
{
	ExpansionVehicleNetworkMode VehicleSync; //! 0 = SERVER | 1 = CLIENT

	ExpansionVehicleKeyStartMode VehicleRequireKeyToStart; //! 0 = Disabled | 1 = Require key to start the engine (will check hands, cargo, inventory) | 2 = check only in the hand
	bool VehicleRequireAllDoors;						   //! If enabled, you will need all the doors to lock/unlock the car
	bool VehicleLockedAllowInventoryAccess;				   //! If enabled, will be able to see the vehicle inventory regardless of the car have all his doors
	bool VehicleLockedAllowInventoryAccessWithoutDoors;	   //! If enabled, will be able to see the vehicle inventory only if at least one car door is missing
	int MasterKeyPairingMode;							   //! -1 = infinite | 0 = disabled | 1 = limited (will use MasterKeyUses) | 2 = renewable with a electronicrepairkit and KeyGrinder (use MasterKeyUses) | 3 = renewable only with a KeyGrinder (use MasterKeyUses)
	int MasterKeyUses;									   //! Amount of times the masterkey can pair unpaired keys
	bool CanPickLock;
	autoptr TStringArray PickLockTools;
	float PickLockChancePercent;
	int PickLockTimeSeconds;
	float PickLockToolDamagePercent;
	bool EnableWindAerodynamics;		//! If enabled, wind simulation will be enabled
	bool EnableTailRotorDamage;			//! If enabled, the rotor will be damageable
	bool PlayerAttachment;				//! If enabled, allow players to be attached to vehicles while in movement
	bool Towing;						//! If enabled, allow vehicle to tow other vehicles
	bool EnableHelicopterExplosions;	//! If enabled, allow Helicopters to explode
	bool DisableVehicleDamage;			//! If disabled, vehicles (cars, trucks) won't take any damages
	float VehicleCrewDamageMultiplier;	//! Damage multiplier for the crew. How fast they will blackout or die.
	float VehicleSpeedDamageMultiplier; //! Damage multiplier for the speed of the car. Above 0 is weaker and below 0 is stronger.
}

/**@class		ExpansionVehicleSettingsV2
 * @brief		Vehicle settings v2 class
 **/
class ExpansionVehicleSettingsV2 : ExpansionVehicleSettingsBase
{
	bool CanChangeLock;
	autoptr TStringArray ChangeLockTools;
	int ChangeLockTimeSeconds;
	float ChangeLockToolDamagePercent;
}

/**@class		ExpansionVehicleSettings
 * @brief		Vehicle settings class
 **/
class ExpansionVehicleSettings : ExpansionVehicleSettingsV2
{
	static const int VERSION = 8;

	ExpansionPPOGORIVMode PlacePlayerOnGroundOnReconnectInVehicle;
	bool RevvingOverMaxRPMRuinsEngineInstantly;
	bool VehicleDropsRuinedDoors;
	bool ExplodingVehicleDropsAttachments;
	float ForcePilotSyncIntervalSeconds;

	ref array<ref ExpansionVehiclesConfig> VehiclesConfig;

	[NonSerialized()] bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionVehicleSettings()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::ExpansionVehicleSettings - Start");
#endif

		PickLockTools = new TStringArray;
		ChangeLockTools = new TStringArray;

		VehiclesConfig = new array<ref ExpansionVehiclesConfig>;

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::ExpansionVehicleSettings - End");
#endif
	}

	// ------------------------------------------------------------
	override bool OnRecieve(ParamsReadContext ctx)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::OnRecieve - Start");
#endif

		ctx.Read(VehicleSync);
		ctx.Read(VehicleRequireKeyToStart);
		ctx.Read(VehicleRequireAllDoors);
		ctx.Read(VehicleLockedAllowInventoryAccess);
		ctx.Read(VehicleLockedAllowInventoryAccessWithoutDoors);
		ctx.Read(MasterKeyPairingMode);
		ctx.Read(MasterKeyUses);
		ctx.Read(CanPickLock);
		ctx.Read(PickLockTools);
		EXPrint("PickLockTools: " + PickLockTools.Count());
		ctx.Read(PickLockChancePercent);
		ctx.Read(PickLockTimeSeconds);
		ctx.Read(PickLockToolDamagePercent);
		ctx.Read(EnableWindAerodynamics);
		ctx.Read(EnableTailRotorDamage);
		ctx.Read(PlayerAttachment);
		ctx.Read(Towing);
		ctx.Read(EnableHelicopterExplosions);
		ctx.Read(DisableVehicleDamage);
		ctx.Read(VehicleCrewDamageMultiplier);
		ctx.Read(VehicleSpeedDamageMultiplier);
		ctx.Read(CanChangeLock);
		ctx.Read(ChangeLockTools);
		EXPrint("ChangeLockTools: " + ChangeLockTools.Count());
		ctx.Read(ChangeLockTimeSeconds);
		ctx.Read(ChangeLockToolDamagePercent);

		ctx.Read(PlacePlayerOnGroundOnReconnectInVehicle);
		ctx.Read(RevvingOverMaxRPMRuinsEngineInstantly);

		ctx.Read(ForcePilotSyncIntervalSeconds);

		m_IsLoaded = true;

		Update(NULL);

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::OnRecieve - End");
#endif

		return true;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(VehicleSync);
		ctx.Write(VehicleRequireKeyToStart);
		ctx.Write(VehicleRequireAllDoors);
		ctx.Write(VehicleLockedAllowInventoryAccess);
		ctx.Write(VehicleLockedAllowInventoryAccessWithoutDoors);
		ctx.Write(MasterKeyPairingMode);
		ctx.Write(MasterKeyUses);
		ctx.Write(CanPickLock);
		ctx.Write(PickLockTools);
		ctx.Write(PickLockChancePercent);
		ctx.Write(PickLockTimeSeconds);
		ctx.Write(PickLockToolDamagePercent);
		ctx.Write(EnableWindAerodynamics);
		ctx.Write(EnableTailRotorDamage);
		ctx.Write(PlayerAttachment);
		ctx.Write(Towing);
		ctx.Write(EnableHelicopterExplosions);
		ctx.Write(DisableVehicleDamage);
		ctx.Write(VehicleCrewDamageMultiplier);
		ctx.Write(VehicleSpeedDamageMultiplier);
		ctx.Write(CanChangeLock);
		ctx.Write(ChangeLockTools);
		ctx.Write(ChangeLockTimeSeconds);
		ctx.Write(ChangeLockToolDamagePercent);

		ctx.Write(PlacePlayerOnGroundOnReconnectInVehicle);
		ctx.Write(RevvingOverMaxRPMRuinsEngineInstantly);

		ctx.Write(ForcePilotSyncIntervalSeconds);

		//! Don't send VehiclesConfig
	}

	// ------------------------------------------------------------
	override int Send(PlayerIdentity identity)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Send - Start");
#endif

		if (!IsMissionHost())
		{
			return 0;
		}

		ScriptRPC rpc = new ScriptRPC;
		OnSend(rpc);
		rpc.Send(null, ExpansionSettingsRPC.Vehicle, true, identity);

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Send - End and return");
#endif

		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy(ExpansionSettingBase setting)
	{
		ExpansionVehicleSettings s;
		if (!Class.CastTo(s, setting))
			return false;

		CopyInternal(s);
		return true;
	}

	// ------------------------------------------------------------
	void CopyInternal(ExpansionVehicleSettings s)
	{
		//#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - Start - " + s);
		//#endif

		PlacePlayerOnGroundOnReconnectInVehicle = s.PlacePlayerOnGroundOnReconnectInVehicle;
		RevvingOverMaxRPMRuinsEngineInstantly = s.RevvingOverMaxRPMRuinsEngineInstantly;

		ForcePilotSyncIntervalSeconds = s.ForcePilotSyncIntervalSeconds;

		VehiclesConfig.Clear();
		for (int i = 0; i < s.VehiclesConfig.Count(); i++)
		{
			VehiclesConfig.Insert(s.VehiclesConfig[i]);
		}

		ExpansionVehicleSettingsV2 settings_v2 = s;
		CopyInternal(settings_v2);

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - End");
#endif
	}

	// ------------------------------------------------------------
	void CopyInternal(ExpansionVehicleSettingsBase s)
	{
		//#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - Start - Base - " + s);
		//#endif

		VehicleSync = s.VehicleSync; // Not ready
		VehicleRequireKeyToStart = s.VehicleRequireKeyToStart;
		VehicleRequireAllDoors = s.VehicleRequireAllDoors;
		VehicleLockedAllowInventoryAccess = s.VehicleLockedAllowInventoryAccess;
		VehicleLockedAllowInventoryAccessWithoutDoors = s.VehicleLockedAllowInventoryAccessWithoutDoors;
		MasterKeyPairingMode = s.MasterKeyPairingMode;
		MasterKeyUses = s.MasterKeyUses;
		PickLockChancePercent = s.PickLockChancePercent;
		PickLockTimeSeconds = s.PickLockTimeSeconds;
		PickLockToolDamagePercent = s.PickLockToolDamagePercent;
		PickLockTools.Copy(s.PickLockTools);
		CanPickLock = s.CanPickLock;
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
	void CopyInternal(ExpansionVehicleSettingsV2 s)
	{
		//#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::CopyInternal - Start - V2 - " + s);
		//#endif

		CanChangeLock = s.CanChangeLock;
		ChangeLockTimeSeconds = s.ChangeLockTimeSeconds;
		ChangeLockToolDamagePercent = s.ChangeLockToolDamagePercent;
		ChangeLockTools.Copy(s.ChangeLockTools);

		ExpansionVehicleSettingsBase sb = s;
		CopyInternal(sb);

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

		bool save;

		bool vehicleSettingsExist = FileExist(EXPANSION_VEHICLE_SETTINGS);

		if (vehicleSettingsExist)
		{
			EXPrint("[ExpansionVehicleSettings] Load existing setting file:" + EXPANSION_VEHICLE_SETTINGS);

			ExpansionVehicleSettings settingsDefault = new ExpansionVehicleSettings;
			settingsDefault.Defaults();

			ExpansionVehicleSettingsBase settingsBase;

			JsonFileLoader<ExpansionVehicleSettingsBase>.JsonLoadFile(EXPANSION_VEHICLE_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionVehicleSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_VEHICLE_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 2)
				{
					//! CanChangeLock, ChangeLockTools, ChangeLockTimeSeconds, ChangeLockToolDamagePercent added with v2
					CopyInternal(settingsDefault);
				}
				else if (settingsBase.m_Version < 4)
				{
					ExpansionVehicleSettingsV2 settings_v2;
					JsonFileLoader<ExpansionVehicleSettingsV2>.JsonLoadFile(EXPANSION_VEHICLE_SETTINGS, settings_v2);

					CopyInternal(settings_v2);

					//! VehiclesConfig added with v4
					VehiclesConfig = settingsDefault.VehiclesConfig;
				}
				else
				{
					JsonFileLoader<ExpansionVehicleSettings>.JsonLoadFile(EXPANSION_VEHICLE_SETTINGS, this);
				}

				if (settingsBase.m_Version < 5)
					PlacePlayerOnGroundOnReconnectInVehicle = settingsDefault.PlacePlayerOnGroundOnReconnectInVehicle;

				if (settingsBase.m_Version < 6)
					RevvingOverMaxRPMRuinsEngineInstantly = settingsDefault.RevvingOverMaxRPMRuinsEngineInstantly;

				if (settingsBase.m_Version < 7)
				{
					VehicleDropsRuinedDoors = settingsDefault.VehicleDropsRuinedDoors;
					ExplodingVehicleDropsAttachments = settingsDefault.ExplodingVehicleDropsAttachments;
				}

				if (settingsBase.m_Version < 8)
					ForcePilotSyncIntervalSeconds = settingsDefault.ForcePilotSyncIntervalSeconds;

				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionVehicleSettings>.JsonLoadFile(EXPANSION_VEHICLE_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionVehicleSettings] No existing setting file:" + EXPANSION_VEHICLE_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		Update(NULL);

		if (save)
			Save();

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleSettings::Load - End - Loaded: " + vehicleSettingsExist);
#endif

		return vehicleSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionVehicleSettings] Saving settings");

		JsonFileLoader<ExpansionVehicleSettings>.JsonSaveFile(EXPANSION_VEHICLE_SETTINGS, this);

		return true;
	}

	// ------------------------------------------------------------
	override void Update(ExpansionSettingBase setting)
	{
		super.Update(setting);

		ExpansionSettings.SI_Vehicle.Invoke();

#ifdef EXPANSION_PLAYER_ATTACHMENT
		s_ExpansionPlayerAttachment = PlayerAttachment;
#endif
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

		VehicleSync = ExpansionVehicleNetworkMode.SERVER;

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
		PickLockTools.Insert("Lockpick");

		CanChangeLock = false;
		ChangeLockTimeSeconds = 120;
		ChangeLockToolDamagePercent = 10;
		ChangeLockTools.Insert("Screwdriver");

		EnableWindAerodynamics = false; // Not ready, need tweaking
		EnableTailRotorDamage = true;

		EnableHelicopterExplosions = true;

#ifdef EXPANSION_PLAYER_ATTACHMENT
		PlayerAttachment = true;
#else
		PlayerAttachment = false;
#endif

		Towing = true;

		DisableVehicleDamage = false;
		VehicleCrewDamageMultiplier = 1.0;
		VehicleSpeedDamageMultiplier = 1.0;

		PlacePlayerOnGroundOnReconnectInVehicle = ExpansionPPOGORIVMode.OnlyOnServerRestart;
		RevvingOverMaxRPMRuinsEngineInstantly = false;
		VehicleDropsRuinedDoors = true;
		ExplodingVehicleDropsAttachments = true;

		ForcePilotSyncIntervalSeconds = 1.0;

		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionUAZCargoRoofless", true));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionBus", true));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionVodnik", true));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionUtilityBoat", true));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionZodiacBoat", true));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionLHD", true));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionMerlin", true));
	}

	override string SettingName()
	{
		return "Vehicle Settings";
	}
};

#ifdef EXPANSION_PLAYER_ATTACHMENT
static bool s_ExpansionPlayerAttachment = true;
#else
static bool s_ExpansionPlayerAttachment;
#endif

enum ExpansionPPOGORIVMode
{
	Disabled,
	Always,
	OnlyOnServerRestart
};
