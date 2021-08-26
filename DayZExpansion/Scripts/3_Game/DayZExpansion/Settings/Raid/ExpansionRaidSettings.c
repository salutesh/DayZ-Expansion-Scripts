/**
 * ExpansionRaidSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionRaidSettingsBase
 * @brief		Spawn settings class
 **/
class ExpansionRaidSettingsBase: ExpansionSettingBase
{
	float ExplosionTime;												//! Ammount of time it takes for explosive to explode.
	autoptr TStringArray ExplosiveDamageWhitelist;		//! List of damage sources allowed to damage bases when whitelist is enabled. 
	bool EnableExplosiveWhitelist;   								//! If enabled, only damage sources listed in ExplosiveDamageWhitelist will be able to damage walls. 
	float ExplosionDamageMultiplier;								//! Damage multiplier from explosion.
	float ProjectileDamageMultiplier;								//! Damage multiplier from projectiles.
	bool CanRaidSafes;												//! If enabled, make safes raidable
	float SafeExplosionDamageMultiplier;						//! Damage multiplier from explosion on safes.
	float SafeProjectileDamageMultiplier;						//! Damage multiplier from explosion on safes.
	autoptr TStringArray SafeRaidTools;						//! List of tools allowed for raiding safes
	int SafeRaidToolTimeSeconds;								//! Time needed to raid safe with tool
	int SafeRaidToolCycles;											//! Number of cycles needed to raid safe
	float SafeRaidToolDamagePercent;							//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)
	bool CanRaidBarbedWire;										//! If enabled, make barbed wire raidable
	autoptr TStringArray BarbedWireRaidTools;			//! List of tools allowed for raiding barbed wire
	int BarbedWireRaidToolTimeSeconds;						//! Time needed to raid barbed wire with tool
	int BarbedWireRaidToolCycles;								//! Number of cycles needed to raid barbed wire
	float BarbedWireRaidToolDamagePercent;				//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)
	RaidLocksOnWallsEnum CanRaidLocksOnWalls;			//! If set to 1 make locks (both vanilla and Expansion) raidable on walls | 2 = only doors | 3 = only gates
	bool CanRaidLocksOnFences;									//! If enabled, make locks (both vanilla and Expansion) raidable on fences
	bool CanRaidLocksOnTents;									//! If enabled, make locks (both vanilla and Expansion) raidable on tents
	autoptr TStringArray LockRaidTools;						//! List of tools allowed for raiding locks
	int LockOnWallRaidToolTimeSeconds;						//! Time needed to raid lock on wall with tool. Disabled <= 0
	int LockOnFenceRaidToolTimeSeconds;					//! Time needed to raid lock on fence with tool. Disabled <= 0
	int LockOnTentRaidToolTimeSeconds;						//! Time needed to raid lock on tent with tool. Disabled <= 0
	int LockRaidToolCycles;											//! Number of cycles needed to raid lock
	float LockRaidToolDamagePercent;							//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)
	BaseBuildingRaidEnum BaseBuildingRaidMode;			//! 0 = Default, everything can take dmg | 1 = doors and gates | 2 = doors, gates and windows
	
	// ------------------------------------------------------------
	void ExpansionRaidSettingsBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettingsBase::ExpansionRaidSettingsBase - Start");
		#endif

		ExplosiveDamageWhitelist = new TStringArray;
		SafeRaidTools = new TStringArray;
		BarbedWireRaidTools = new TStringArray;
		LockRaidTools = new TStringArray;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettingsBase::ExpansionRaidSettingsBase - End");
		#endif
	}
}

/**@class		ExpansionRaidSettings
 * @brief		Spawn settings class
 **/
class ExpansionRaidSettings: ExpansionRaidSettingsBase
{
	static const int VERSION = 0;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionRaidSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::ExpansionRaidSettings - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::ExpansionRaidSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void ~ExpansionRaidSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::~ExpansionRaidSettings - Start");
		#endif
		
		ExplosiveDamageWhitelist.Clear();
		delete ExplosiveDamageWhitelist;
		SafeRaidTools.Clear();
		delete SafeRaidTools;
		BarbedWireRaidTools.Clear();
		delete BarbedWireRaidTools;
		LockRaidTools.Clear();
		delete LockRaidTools;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::~ExpansionRaidSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::OnRecieve - Start");
		#endif
		
		ExpansionRaidSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionRaidSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );
		
		m_IsLoaded = true;

		ExpansionSettings.SI_Raid.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionRaidSettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}

		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Raid, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::Send - End and return");
		#endif

		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionRaidSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionRaidSettings s )
	{
		//!Nothing to do here yet
		
		ExpansionRaidSettingsBase sb = s;
		CopyInternal( sb );
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionRaidSettingsBase s )
	{
		/*
		RaidValues.Clear();

		for (int i = 0; i < s.RaidValues.Count(); i++)
		{
			RaidValues.Insert( s.RaidValues[i] );
		}
		*/	

		ExplosiveDamageWhitelist.Clear();
		for (int i = 0; i < s.ExplosiveDamageWhitelist.Count(); i++)
		{
			ExplosiveDamageWhitelist.Insert( s.ExplosiveDamageWhitelist[i] );
		}

		ExplosionTime = s.ExplosionTime;

		EnableExplosiveWhitelist = s.EnableExplosiveWhitelist;
		ExplosionDamageMultiplier = s.ExplosionDamageMultiplier;
		ProjectileDamageMultiplier = s.ProjectileDamageMultiplier;	
		
		CanRaidSafes = s.CanRaidSafes;
		SafeExplosionDamageMultiplier = s.SafeExplosionDamageMultiplier;
		SafeProjectileDamageMultiplier = s.SafeProjectileDamageMultiplier;

		SafeRaidTools.Copy( s.SafeRaidTools );
		SafeRaidToolTimeSeconds = s.SafeRaidToolTimeSeconds;
		SafeRaidToolCycles = s.SafeRaidToolCycles;
		SafeRaidToolDamagePercent = s.SafeRaidToolDamagePercent;
		
		CanRaidBarbedWire = s.CanRaidBarbedWire;

		BarbedWireRaidTools.Copy( s.BarbedWireRaidTools );
		BarbedWireRaidToolTimeSeconds = s.BarbedWireRaidToolTimeSeconds;
		BarbedWireRaidToolCycles = s.BarbedWireRaidToolCycles;
		BarbedWireRaidToolDamagePercent = s.BarbedWireRaidToolDamagePercent;
		
		CanRaidLocksOnWalls = s.CanRaidLocksOnWalls;
		CanRaidLocksOnFences = s.CanRaidLocksOnFences;
		CanRaidLocksOnTents = s.CanRaidLocksOnTents;

		LockRaidTools.Copy( s.LockRaidTools );
		LockOnWallRaidToolTimeSeconds = s.LockOnWallRaidToolTimeSeconds;
		LockOnFenceRaidToolTimeSeconds = s.LockOnFenceRaidToolTimeSeconds;
		LockOnTentRaidToolTimeSeconds = s.LockOnTentRaidToolTimeSeconds;
		LockRaidToolCycles = s.LockRaidToolCycles;
		LockRaidToolDamagePercent = s.LockRaidToolDamagePercent;

		BaseBuildingRaidMode = s.BaseBuildingRaidMode;
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
		EXPrint("ExpansionRaidSettings::Load - Start");
		#endif

		m_IsLoaded = true;
		
		bool save;
		
		bool raidSettingsExist = FileExist(EXPANSION_RAID_SETTINGS);

		if (raidSettingsExist)
		{
			EXPrint("[ExpansionRaidSettings] Load existing setting file:" + EXPANSION_RAID_SETTINGS);
			
			ExpansionRaidSettings settingsDefault = new ExpansionRaidSettings;
			settingsDefault.Defaults();

			ExpansionRaidSettingsBase settingsBase;

			JsonFileLoader<ExpansionRaidSettingsBase>.JsonLoadFile(EXPANSION_RAID_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					EXPrint("[ExpansionRaidSettings] Load - Converting v1 \"" + EXPANSION_RAID_SETTINGS + "\" to v" + VERSION);
					//!Nothing to do here yet
				}
				
				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionRaidSettings>.JsonLoadFile(EXPANSION_RAID_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionRaidSettings] No existing setting file:" + EXPANSION_RAID_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::Load - End");
		#endif
		
		return raidSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionRaidSettings] Saving settings");
		
		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionRaidSettings>.JsonSaveFile( EXPANSION_RAID_SETTINGS, this );
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Raid.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		ExplosionTime = 30;

		ExplosiveDamageWhitelist.Insert("Expansion_C4_Explosion");
		ExplosiveDamageWhitelist.Insert("Expansion_RPG_Explosion");
		ExplosiveDamageWhitelist.Insert("Expansion_LAW_Explosion");
		
		ExplosiveDamageWhitelist.Insert("RGD5Grenade_Ammo");
		ExplosiveDamageWhitelist.Insert("M67Grenade_Ammo");
		ExplosiveDamageWhitelist.Insert("FlashGrenade_Ammo");
		ExplosiveDamageWhitelist.Insert("LandFuelFeed_Ammo");

		EnableExplosiveWhitelist = false;
		ExplosionDamageMultiplier = 50;
		ProjectileDamageMultiplier = 1;
		
		CanRaidSafes = true;
		SafeExplosionDamageMultiplier = 17;
		SafeProjectileDamageMultiplier = 1;

		SafeRaidTools.Insert( "ExpansionPropaneTorch" );
		SafeRaidToolTimeSeconds = 10 * 60;
		SafeRaidToolCycles = 5;
		SafeRaidToolDamagePercent = 100;
		
		CanRaidBarbedWire = true;

		BarbedWireRaidTools.Insert( "ExpansionBoltCutters" );
		BarbedWireRaidToolTimeSeconds = 5 * 60;
		BarbedWireRaidToolCycles = 5;
		BarbedWireRaidToolDamagePercent = 100;
		
		CanRaidLocksOnWalls = RaidLocksOnWallsEnum.Disabled;
		CanRaidLocksOnFences = false;
		CanRaidLocksOnTents = false;

		LockOnWallRaidToolTimeSeconds = 15 * 60;
		LockOnFenceRaidToolTimeSeconds = 15 * 60;
		LockOnTentRaidToolTimeSeconds = 10 * 60;
		LockRaidToolCycles = 5;
		LockRaidToolDamagePercent = 100;

		BaseBuildingRaidMode = BaseBuildingRaidEnum.Default;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Raid Settings";
	}
};