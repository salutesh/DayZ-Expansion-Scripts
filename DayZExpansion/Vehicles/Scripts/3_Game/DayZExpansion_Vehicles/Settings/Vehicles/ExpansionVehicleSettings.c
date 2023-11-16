/**
 * ExpansionVehicleSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	bool Towing;						//! If enabled, allow vehicle to tow other vehicles
	bool EnableHelicopterExplosions;	//! If enabled, allow Helicopters to explode
	bool DisableVehicleDamage;			//! If disabled, vehicles (cars, trucks) won't take any damages
	float VehicleCrewDamageMultiplier;	//! Damage multiplier for the crew. How fast they will blackout or die.
	float VehicleSpeedDamageMultiplier; //! Damage multiplier for the speed of the car. Above 0 is weaker and below 0 is stronger.
	float VehicleRoadKillDamageMultiplier;
	bool CollisionDamageIfEngineOff;	//! Should the vehicle be able to receive damage if the engine is off?
	float CollisionDamageMinSpeedKmh;	//! Minimum speed in km/h for vehicle to take collision damage
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

/**
 * @class		ExpansionVehicleSettings
 * @brief		Vehicle settings class
 */
class ExpansionVehicleSettings : ExpansionVehicleSettingsV2
{
	static const int VERSION = 19;

	ExpansionPPOGORIVMode PlacePlayerOnGroundOnReconnectInVehicle;
	bool RevvingOverMaxRPMRuinsEngineInstantly;
	bool VehicleDropsRuinedDoors;
	bool ExplodingVehicleDropsAttachments;
	//float ForcePilotSyncIntervalSeconds;
	float DesyncInvulnerabilityTimeoutSeconds;  //! Timeout for temporary vehicle godmode during desync. Set to 0 to disable.

	float DamagedEngineStartupChancePercent;

	bool EnableVehicleCovers;
	bool AllowCoveringDEVehicles;  //! Allow covering of vehicles spawned via dynamic events (events.xml)
	bool CanCoverWithCargo;
	bool UseVirtualStorageForCoverCargo;
	float VehicleAutoCoverTimeSeconds;
	bool VehicleAutoCoverRequireCamonet;
	bool EnableAutoCoveringDEVehicles;

	string CFToolsHeliCoverIconName;
	string CFToolsBoatCoverIconName;
	string CFToolsCarCoverIconName;

	bool ShowVehicleOwners;

	ref array<ref ExpansionVehiclesConfig> VehiclesConfig;

	[NonSerialized()]
	bool m_IsLoaded;

	void ExpansionVehicleSettings()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionVehicleSettings");
#endif

		PickLockTools = new TStringArray;
		ChangeLockTools = new TStringArray;

		VehiclesConfig = new array<ref ExpansionVehiclesConfig>;
	}

	// ------------------------------------------------------------
	override bool OnRecieve(ParamsReadContext ctx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRecieve");
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

		ctx.Read(PickLockChancePercent);
		ctx.Read(PickLockTimeSeconds);
		ctx.Read(PickLockToolDamagePercent);
		ctx.Read(EnableWindAerodynamics);
		ctx.Read(EnableTailRotorDamage);
		ctx.Read(Towing);
		ctx.Read(EnableHelicopterExplosions);
		ctx.Read(DisableVehicleDamage);
		ctx.Read(VehicleCrewDamageMultiplier);
		ctx.Read(VehicleSpeedDamageMultiplier);
		ctx.Read(CanChangeLock);
		ctx.Read(ChangeLockTools);

		ctx.Read(ChangeLockTimeSeconds);
		ctx.Read(ChangeLockToolDamagePercent);

		ctx.Read(PlacePlayerOnGroundOnReconnectInVehicle);
		ctx.Read(RevvingOverMaxRPMRuinsEngineInstantly);

		ctx.Read(DesyncInvulnerabilityTimeoutSeconds);
		ctx.Read(VehicleRoadKillDamageMultiplier);

		ctx.Read(EnableVehicleCovers);
		ctx.Read(CanCoverWithCargo);
		ctx.Read(AllowCoveringDEVehicles);

		ctx.Read(ShowVehicleOwners);

		m_IsLoaded = true;

		Update(NULL);

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

		ctx.Write(DesyncInvulnerabilityTimeoutSeconds);
		ctx.Write(VehicleRoadKillDamageMultiplier);

		ctx.Write(EnableVehicleCovers);
		ctx.Write(CanCoverWithCargo);
		ctx.Write(AllowCoveringDEVehicles);

		ctx.Write(ShowVehicleOwners);

		//! Don't send VehiclesConfig
	}

	override int Send(PlayerIdentity identity)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if (!IsMissionHost())
		{
			return 0;
		}

		auto rpc = CreateRPC();
		OnSend( rpc );
		rpc.Expansion_Send(true, identity);

		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy(ExpansionSettingBase setting)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionVehicleSettings s;
		if (!Class.CastTo(s, setting))
			return false;

		CopyInternal(s);
		return true;
	}

	void CopyInternal(ExpansionVehicleSettings s)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		
		PlacePlayerOnGroundOnReconnectInVehicle = s.PlacePlayerOnGroundOnReconnectInVehicle;
		RevvingOverMaxRPMRuinsEngineInstantly = s.RevvingOverMaxRPMRuinsEngineInstantly;

		//ForcePilotSyncIntervalSeconds = s.ForcePilotSyncIntervalSeconds;

		VehiclesConfig.Clear();
		for (int i = 0; i < s.VehiclesConfig.Count(); i++)
		{
			VehiclesConfig.Insert(s.VehiclesConfig[i]);
		}

		DamagedEngineStartupChancePercent = s.DamagedEngineStartupChancePercent;
		VehicleRoadKillDamageMultiplier = s.VehicleRoadKillDamageMultiplier;
		ShowVehicleOwners = s.ShowVehicleOwners;
		ExpansionVehicleSettingsV2 settings_v2 = s;
		CopyInternal(settings_v2);
	}

	void CopyInternal(ExpansionVehicleSettingsBase s)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		
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
		Towing = s.Towing;
		EnableHelicopterExplosions = s.EnableHelicopterExplosions;
		DisableVehicleDamage = s.DisableVehicleDamage;
		VehicleCrewDamageMultiplier = s.VehicleCrewDamageMultiplier;
		VehicleSpeedDamageMultiplier = s.VehicleSpeedDamageMultiplier;
	}

	void CopyInternal(ExpansionVehicleSettingsV2 s)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		
		CanChangeLock = s.CanChangeLock;
		ChangeLockTimeSeconds = s.ChangeLockTimeSeconds;
		ChangeLockToolDamagePercent = s.ChangeLockToolDamagePercent;
		ChangeLockTools.Copy(s.ChangeLockTools);

		ExpansionVehicleSettingsBase sb = s;
		CopyInternal(sb);
	}

	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
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

				if (settingsBase.m_Version >= 4)
				{
					JsonFileLoader<ExpansionVehicleSettings>.JsonLoadFile(EXPANSION_VEHICLE_SETTINGS, this);
				}
				else
				{
					//! Copy over old settings that haven't changed
					CopyInternal(settingsBase);
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

				//! Not used, keeping it just in case
				//if (settingsBase.m_Version < 8)
					//ForcePilotSyncIntervalSeconds = settingsDefault.ForcePilotSyncIntervalSeconds;

				if (settingsBase.m_Version < 9)
				{
					foreach (auto vehicleCfg: VehiclesConfig)
					{
						vehicleCfg.LockComplexity = 1.0;  //! Fallback
						foreach (auto vehicleCfgDefault: settingsDefault.VehiclesConfig)
						{
							if (vehicleCfg.ClassName == vehicleCfgDefault.ClassName)
							{
								vehicleCfg.LockComplexity = vehicleCfgDefault.LockComplexity;
								break;
							}
						}
					}
				}

				if (settingsBase.m_Version < 10)
				{
					DesyncInvulnerabilityTimeoutSeconds = settingsDefault.DesyncInvulnerabilityTimeoutSeconds;
				}

				if (settingsBase.m_Version < 11)
				{
					VehicleRoadKillDamageMultiplier = settingsDefault.VehicleRoadKillDamageMultiplier;
					DamagedEngineStartupChancePercent = settingsDefault.DamagedEngineStartupChancePercent;
				}

				if (settingsBase.m_Version < 12)
					EnableVehicleCovers = settingsDefault.EnableVehicleCovers;

				if (settingsBase.m_Version < 13)
				{
					VehicleAutoCoverTimeSeconds = settingsDefault.VehicleAutoCoverTimeSeconds;
					VehicleAutoCoverRequireCamonet = settingsDefault.VehicleAutoCoverRequireCamonet;
				}

				if (settingsBase.m_Version < 15)
				{
					CFToolsHeliCoverIconName = settingsDefault.CFToolsHeliCoverIconName;
					CFToolsBoatCoverIconName = settingsDefault.CFToolsBoatCoverIconName;
					CFToolsCarCoverIconName = settingsDefault.CFToolsCarCoverIconName;
				}

				if (settingsBase.m_Version < 16)
				{
					CollisionDamageIfEngineOff = settingsDefault.CollisionDamageIfEngineOff;
					CollisionDamageMinSpeedKmh = settingsDefault.CollisionDamageMinSpeedKmh;
				}

				if (settingsBase.m_Version < 18)
					CanCoverWithCargo = settingsDefault.CanCoverWithCargo;

				if (settingsBase.m_Version < 19)
					ShowVehicleOwners = settingsDefault.ShowVehicleOwners;

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

		return vehicleSettingsExist;
	}

	override bool OnSave()
	{
		Print("[ExpansionVehicleSettings] Saving settings");

		JsonFileLoader<ExpansionVehicleSettings>.JsonSaveFile(EXPANSION_VEHICLE_SETTINGS, this);

		return true;
	}

	override void Update(ExpansionSettingBase setting)
	{
		super.Update(setting);

		ExpansionSettings.SI_Vehicle.Invoke();
	}

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

		Towing = true;

		DisableVehicleDamage = false;
		VehicleCrewDamageMultiplier = 1.0;
		VehicleSpeedDamageMultiplier = 1.0;
		VehicleRoadKillDamageMultiplier = 1.0;

		CollisionDamageIfEngineOff = false;
		CollisionDamageMinSpeedKmh = 30;

		PlacePlayerOnGroundOnReconnectInVehicle = ExpansionPPOGORIVMode.OnlyOnServerRestart;
		RevvingOverMaxRPMRuinsEngineInstantly = false;
		VehicleDropsRuinedDoors = true;
		ExplodingVehicleDropsAttachments = true;

		//ForcePilotSyncIntervalSeconds = 1.0;
		DesyncInvulnerabilityTimeoutSeconds = 3.0;

		DamagedEngineStartupChancePercent = 100.0;

#ifndef CARCOVER
		EnableVehicleCovers = true;
#endif
		CanCoverWithCargo = true;
		AllowCoveringDEVehicles = false;
		VehicleAutoCoverTimeSeconds = 0;  //! Lower than or equal to zero = disabled
		VehicleAutoCoverRequireCamonet = false;  //! Require camonet attachment on vehicle

		//! CFTools icons. Any FontAwesome icon name should work.
		CFToolsHeliCoverIconName = "helicopter";
		CFToolsBoatCoverIconName = "ship";
		CFToolsCarCoverIconName = "car";

		ShowVehicleOwners = false;

		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionUAZCargoRoofless", true, 1.0));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionUAZ", false, 1.0));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionBus", true, 1.5));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionVodnik", true, 2.0));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionUtilityBoat", true, 1.25));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionZodiacBoat", true, 0.5));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionLHD", true, 100.0));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionMerlin", true, 4.0));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionMh6", false, 3.0));
		VehiclesConfig.Insert(new ExpansionVehiclesConfig("ExpansionUh1h", false, 2.5));
	}

	override string SettingName()
	{
		return "Vehicle Settings";
	}
};

enum ExpansionPPOGORIVMode
{
	Disabled,
	Always,
	OnlyOnServerRestart
};
