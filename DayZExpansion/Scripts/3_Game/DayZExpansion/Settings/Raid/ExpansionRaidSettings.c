/**
 * ExpansionRaidSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionRaidSettings
 * @brief		Spawn settings class
 **/
class ExpansionRaidSettings: ExpansionSettingBase
{
	float ExplosionTime;							//! Ammount of time it takes for explosive to explode.

	autoptr TStringArray ExplosiveDamageWhitelist;	//! List of damage sources allowed to damage bases when whitelist is enabled. 
	bool EnableExplosiveWhitelist;   				//! If enabled, only damage sources listed in ExplosiveDamageWhitelist will be able to damage walls. 
	float ExplosionDamageMultiplier;				//! Damage multiplier from explosion.
	float ProjectileDamageMultiplier;				//! Damage multiplier from projectiles.

	bool CanRaidSafes;								//! If enabled, make safes raidable
	float SafeExplosionDamageMultiplier;			//! Damage multiplier from explosion on safes.
	float SafeProjectileDamageMultiplier;			//! Damage multiplier from explosion on safes.
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionRaidSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::ExpansionRaidSettings - Start");
		#endif

		ExplosiveDamageWhitelist = new TStringArray;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::ExpansionRaidSettings - End");
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
		ref ExpansionRaidSettings thisSetting = this;

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
	private void CopyInternal( ref ExpansionRaidSettings s )
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
		SafeExplosionDamageMultiplier = s.ExplosionDamageMultiplier;
		SafeProjectileDamageMultiplier = s.ProjectileDamageMultiplier;
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

		if ( FileExist( EXPANSION_RAID_SETTINGS ) )
		{
			JsonFileLoader<ExpansionRaidSettings>.JsonLoadFile( EXPANSION_RAID_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionRaidSettings::Load - End");
			#endif

			return true;
		}
		
		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::Load - End");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionRaidSettings] Saving settings");
		#endif
		
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::Defaults - Start");
		#endif

		ExplosionTime = 30;

		ExplosiveDamageWhitelist.Insert("Expansion_C4_Explosion");
		ExplosiveDamageWhitelist.Insert("Expansion_RPG_Explosion");

		EnableExplosiveWhitelist = false;
		ExplosionDamageMultiplier = 50;
		ProjectileDamageMultiplier = 1;
		
		CanRaidSafes = true;
		SafeExplosionDamageMultiplier = 17;
		SafeProjectileDamageMultiplier = 1;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRaidSettings::Defaults - End");
		#endif
	}
};