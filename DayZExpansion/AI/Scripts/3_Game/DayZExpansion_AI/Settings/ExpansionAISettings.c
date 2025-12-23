/**
 * ExpansionAISettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAISettingsV11: ExpansionSettingBase
{
	ref TStringArray ExcludedRoamingLocations = {};
	ref TStringArray ExcludedRoamingBuildings = {};
}

/**@class		ExpansionAISettings
 * @brief		Spawn settings class
 **/
class ExpansionAISettings: ExpansionSettingBase
{
	static const int VERSION = 19;
	static ref ExpansionAISettings s_Instance;

	float AccuracyMin;
	float AccuracyMax;

	float ThreatDistanceLimit;
	float NoiseInvestigationDistanceLimit;
	float MaxFlankingDistance;
	bool EnableFlankingOutsideCombat;
	float DamageMultiplier;
	float DamageReceivedMultiplier;

	autoptr TStringArray Admins = {};

	bool Vaulting;

	float SniperProneDistanceThreshold;

	float AggressionTimeout;
	float GuardAggressionTimeout;

	bool Manners;
	int MemeLevel;
	
	bool CanRecruitFriendly;
	bool CanRecruitGuards;
	int MaxRecruitableAI;

	ref TStringArray PreventClimb = {};

	float FormationScale;

	autoptr TStringArray PlayerFactions = {};  //! If non-empty, player will automatically join one of these factions on connect

	bool LogAIHitBy;
	bool LogAIKilled;

	bool EnableZombieVehicleAttackHandler;
	bool EnableZombieVehicleAttackPhysics;

	bool OverrideClientWeaponFiring;
	bool RecreateWeaponNetworkRepresentation;

	ref map<int, float> LightingConfigMinNightVisibilityMeters = new map<int, float>;

	[NonSerialized()]
	private bool m_IsAdmin;

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		auto reader = GetReader(ctx);

		if (!reader.ReadBool(CanRecruitGuards))
		{
			Error("Couldn't read CanRecruitGuards");
			return false;
		}

		if (!reader.ReadBool(CanRecruitFriendly))
		{
			Error("Couldn't read CanRecruitFriendly");
			return false;
		}

		if (!reader.ReadUInt(MaxRecruitableAI, 8))
		{
			Error("Couldn't read MaxRecruitableAI");
			return false;
		}

		if (!reader.ReadBool(OverrideClientWeaponFiring))
		{
			Error("Couldn't read OverrideClientWeaponFiring");
			return false;
		}

		if (!reader.ReadBool(RecreateWeaponNetworkRepresentation))
		{
			Error("Couldn't read RecreateWeaponNetworkRepresentation");
			return false;
		}

		if (!reader.ReadBool(m_IsAdmin))
		{
			Error("Couldn't read m_IsAdmin");
			return false;
		}

	#ifdef DIAG_DEVELOPER
		Print(CanRecruitGuards);
		Print(CanRecruitFriendly);
		Print(MaxRecruitableAI);
		Print(OverrideClientWeaponFiring);
		Print(RecreateWeaponNetworkRepresentation);
		Print(m_IsAdmin);
	#endif

		m_IsLoaded = true;

		EXLogPrint("Received AI settings");

		ExpansionSettings.SI_AI.Invoke();

		return true;
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif
		if ( !IsMissionHost() )
		{
			return 0;
		}

		auto rpc = CreateRPC();

		rpc.WriteBool(CanRecruitGuards);
		rpc.WriteBool(CanRecruitFriendly);
		rpc.WriteUInt(MaxRecruitableAI, 8);
		rpc.WriteBool(OverrideClientWeaponFiring);
		rpc.WriteBool(RecreateWeaponNetworkRepresentation);
		rpc.WriteBool(IsAdmin(identity));

		rpc.Expansion_Send(true, identity);

		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionAISettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );

		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionAISettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		AccuracyMin = s.AccuracyMin;
		AccuracyMax = s.AccuracyMax;
		ThreatDistanceLimit = s.ThreatDistanceLimit;
		NoiseInvestigationDistanceLimit = s.NoiseInvestigationDistanceLimit;
		DamageMultiplier = s.DamageMultiplier;
		DamageReceivedMultiplier = s.DamageReceivedMultiplier;
		Admins.Copy(s.Admins);
		Vaulting = s.Vaulting;
		SniperProneDistanceThreshold = s.SniperProneDistanceThreshold;
		Manners = s.Manners;
		MemeLevel = s.MemeLevel;
		CanRecruitFriendly = s.CanRecruitFriendly;
		CanRecruitGuards = s.CanRecruitGuards;
		PreventClimb.Copy(s.PreventClimb);
		FormationScale = s.FormationScale;
		PlayerFactions.Copy(s.PlayerFactions);
		LogAIHitBy = s.LogAIHitBy;
		LogAIKilled = s.LogAIKilled;
		EnableZombieVehicleAttackHandler = s.EnableZombieVehicleAttackHandler;
		EnableZombieVehicleAttackPhysics = s.EnableZombieVehicleAttackPhysics;
		LightingConfigMinNightVisibilityMeters.Copy(s.LightingConfigMinNightVisibilityMeters);
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool AISettingsExist = FileExist(EXPANSION_AI_SETTINGS);

		//! Copy over old eAI settings
		string eAISettings = "$profile:eAI\\eAISettings.json";
		if (!AISettingsExist && FileExist(eAISettings))
		{
			EXLogPrint("[ExpansionAISettings] Copying " + eAISettings + " to " + EXPANSION_AI_SETTINGS);
			AISettingsExist = CopyFile(eAISettings, EXPANSION_AI_SETTINGS);
		}

		if (AISettingsExist)
		{
			EXPrint("[ExpansionAISettings] Load existing setting file:" + EXPANSION_AI_SETTINGS);

			if (!ExpansionJsonFileParser<ExpansionAISettings>.Load(EXPANSION_AI_SETTINGS, this))
			{
				//! Use defaults, but DON'T save them
				Defaults();
			}
			else if (m_Version < VERSION)
			{
				EXPrint("[ExpansionAISettings] Load - Converting v" + m_Version + " \"" + EXPANSION_AI_SETTINGS + "\" to v" + VERSION);

				ExpansionAISettings settingsDefault = new ExpansionAISettings;
				settingsDefault.Defaults();

				if (m_Version < 1)
				{
					//! Old eAI settings
					if (!AccuracyMin)
						AccuracyMin = settingsDefault.AccuracyMin;
					if (!AccuracyMax)
						AccuracyMax = settingsDefault.AccuracyMax;
				}

				if (m_Version < 2)
				{
					Manners = settingsDefault.Manners;
				}

				if (m_Version < 4)
				{
					ThreatDistanceLimit = settingsDefault.ThreatDistanceLimit;
					DamageMultiplier = settingsDefault.DamageMultiplier;
				}

				if (m_Version < 7)
				{
					CanRecruitFriendly = settingsDefault.CanRecruitFriendly;
					PreventClimb = settingsDefault.PreventClimb;
					MemeLevel = settingsDefault.MemeLevel;
				}

				if (m_Version < 8)
					NoiseInvestigationDistanceLimit = settingsDefault.NoiseInvestigationDistanceLimit;

				if (m_Version < 9 && !DamageReceivedMultiplier)
					DamageReceivedMultiplier = settingsDefault.DamageReceivedMultiplier;

				if (m_Version < 10)
				{
					LogAIHitBy = settingsDefault.LogAIHitBy;
					LogAIKilled = settingsDefault.LogAIKilled;
				}

				if (m_Version < 12)
				{
					ExpansionAISettingsV11 settingsV11;
					if (ExpansionJsonFileParser<ExpansionAISettingsV11>.Load(EXPANSION_AI_SETTINGS, settingsV11))
					{
						auto locationSettings = GetExpansionSettings().GetAILocation();

						locationSettings.ExcludedRoamingBuildings.InsertAll(settingsV11.ExcludedRoamingBuildings);

						foreach (auto loc: locationSettings.RoamingLocations)
						{
							if (settingsV11.ExcludedRoamingLocations.Find(loc.m_ClassName) > -1)
								loc.Enabled = false;
						}

						locationSettings.Save();
					}
				}

				if (m_Version < 13 && !FormationScale)
					FormationScale = settingsDefault.FormationScale;

				if (m_Version < 15 && LightingConfigMinNightVisibilityMeters.Count() == 0)
					LightingConfigMinNightVisibilityMeters.Copy(settingsDefault.LightingConfigMinNightVisibilityMeters);

				if (m_Version < 16 && !MaxRecruitableAI)
					MaxRecruitableAI = settingsDefault.MaxRecruitableAI;

				if (m_Version < 17)
				{
					if (!OverrideClientWeaponFiring)
						OverrideClientWeaponFiring = settingsDefault.OverrideClientWeaponFiring;

					if (!RecreateWeaponNetworkRepresentation)
						RecreateWeaponNetworkRepresentation = settingsDefault.RecreateWeaponNetworkRepresentation;
				}

				if (m_Version < 18)
				{
					if (!AggressionTimeout)
						AggressionTimeout = settingsDefault.AggressionTimeout;

					if (!GuardAggressionTimeout)
						GuardAggressionTimeout = settingsDefault.GuardAggressionTimeout;
				}

				if (!MaxFlankingDistance)
					MaxFlankingDistance = settingsDefault.MaxFlankingDistance;

				if (m_Version < 19 && !EnableFlankingOutsideCombat)
					EnableFlankingOutsideCombat = settingsDefault.EnableFlankingOutsideCombat;

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionAISettings] No existing setting file:" + EXPANSION_AI_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (save)
			Save();

		s_Instance = this;

		return AISettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionAISettings] Saving settings");

		JsonFileLoader<ExpansionAISettings>.JsonSaveFile( EXPANSION_AI_SETTINGS, this );
		return true;
	}

	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_AI.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

		AccuracyMin = 0.35;
		AccuracyMax = 0.95;

		ThreatDistanceLimit = 1000.0;
		NoiseInvestigationDistanceLimit = 500.0;
		MaxFlankingDistance = 200.0;
		EnableFlankingOutsideCombat = false;
		DamageMultiplier = 1.0;
		DamageReceivedMultiplier = 1.0;

		Admins.Clear();

		Vaulting = true;

		SniperProneDistanceThreshold = 0.0;

		AggressionTimeout = 120;
		GuardAggressionTimeout = 150;

		Manners = false;
		MemeLevel = 1;

		CanRecruitFriendly = true;
		CanRecruitGuards = false;

	#ifdef DIAG_DEVELOPER
		MaxRecruitableAI = 255;
	#else
		MaxRecruitableAI = 1;
	#endif

		PreventClimb.Clear();

		FormationScale = 1.0;

		PlayerFactions.Clear();

		LogAIHitBy = true;
		LogAIKilled = true;

		EnableZombieVehicleAttackHandler = false;
		EnableZombieVehicleAttackPhysics = false;

		OverrideClientWeaponFiring = true;
		RecreateWeaponNetworkRepresentation = true;

		LightingConfigMinNightVisibilityMeters.Clear();
		LightingConfigMinNightVisibilityMeters[0] = 100;  //! Assume bright night, min visibility 100 m
		LightingConfigMinNightVisibilityMeters[1] = 10;  //! Assume dark night, min visibility 10 m
	}

	float GetDefaultMinVisibility(int lightingConfig)
	{
		switch (lightingConfig)
		{
			case 1:
				return 0.01;  //! Assume dark night, min visibility 10 m
				break;
		}

		return 0.1;  //! Assume bright night, min visibility 100 m
	}

	// ------------------------------------------------------------
	override string SettingName()
	{
		return "AI Settings";
	}

	bool IsAdmin(PlayerIdentity identity = null)
	{
		if (IsMissionOffline())
			return true;

		if (IsMissionClient())
			return m_IsAdmin;

		return identity && Admins.Find(identity.GetPlainId()) > -1;
	}
	
};
