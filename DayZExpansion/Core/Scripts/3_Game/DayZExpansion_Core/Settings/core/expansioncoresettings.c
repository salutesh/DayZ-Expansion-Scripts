/**
 * ExpansionCoreSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

typedef ExpansionCoreSettings ExpansionDebugSettings;  //! Legacy compat

class ExpansionCoreSettings: ExpansionSettingBase
{
	static const int VERSION = 9;
	
	int ServerUpdateRateLimit;
	bool ForceExactCEItemLifetime;
	bool EnableInventoryCargoTidy;

	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		if (!ctx.Read(EnableInventoryCargoTidy))
		{
			Error("ExpansionCoreSettings::OnRecieve EnableInventoryCargoTidy");
			return false;
		}

		float foodDecayModifier;
		if ( !ctx.Read( foodDecayModifier ) )
		{
			Error("ExpansionCoreSettings::OnRecieve foodDecayModifier");
			return false;
		}

		g_Game.Expansion_SetFoodDecayModifier(foodDecayModifier);

		m_IsLoaded = true;

		ExpansionSettings.SI_Core.Invoke();
		
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write(EnableInventoryCargoTidy);
		ctx.Write(g_Game.GetFoodDecayModifier());
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
		OnSend( rpc );
		rpc.Expansion_Send(true, identity);
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionCoreSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionCoreSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		ServerUpdateRateLimit = s.ServerUpdateRateLimit;
		ForceExactCEItemLifetime = s.ForceExactCEItemLifetime;
		EnableInventoryCargoTidy = s.EnableInventoryCargoTidy;
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
		
		if (FileExist(EXPANSION_DEBUG_SETTINGS))
			MoveSettings(EXPANSION_DEBUG_SETTINGS, EXPANSION_CORE_SETTINGS);

		bool save;
		
		bool coreSettingsExist = FileExist(EXPANSION_CORE_SETTINGS);
		
		if (coreSettingsExist)
		{
			EXPrint("[ExpansionCoreSettings] Load existing setting file:" + EXPANSION_CORE_SETTINGS);

			string errorMsg;
			if (!JsonFileLoader<ExpansionCoreSettings>.LoadFile(EXPANSION_CORE_SETTINGS, this, errorMsg))
				EXError.Error(this, errorMsg, {});

			if (m_Version < VERSION)
			{
				auto settingsDefaults = new ExpansionCoreSettings();
				settingsDefaults.Defaults();

				if (m_Version < 8 && !EnableInventoryCargoTidy)
					EnableInventoryCargoTidy = settingsDefaults.EnableInventoryCargoTidy;

				m_Version = VERSION;

				save = true;
			}

			if (ForceExactCEItemLifetime)
				EXError.Warn(this, "ForceExactCEItemLifetime is enabled and staggered item de-/respawning is essentially no longer in effect as a result. This may negatively affect server and loot economy performance. Recommended setting for ForceExactCEItemLifetime is 0 or false unless you're sure you still want it despite the potential drawback.", {});
		}
		else
		{
			EXPrint("[ExpansionCoreSettings] No existing setting file:" + EXPANSION_CORE_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (ServerUpdateRateLimit > 0)
		{
			if (ServerUpdateRateLimit >= 40)
			{
				EXPrint("[ExpansionCoreSettings] Limiting server script update rate to " + ServerUpdateRateLimit + " per second");
				GetDayZGame().Expansion_SetServerUpdateRateLimit(ServerUpdateRateLimit);
			}
			else
			{
				EXPrint("[ExpansionCoreSettings] ERROR: ServerUpdateRateLimit " + ServerUpdateRateLimit + " is too low!");
			}
		}
		
		if (save)
			Save();

		return coreSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionCoreSettings] Saving settings");

		string errorMsg;
		if (!JsonFileLoader<ExpansionCoreSettings>.SaveFile(EXPANSION_CORE_SETTINGS, this, errorMsg))
			EXError.Error(this, errorMsg, {});

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Core.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		ServerUpdateRateLimit = 0;
		ForceExactCEItemLifetime = false;

		EnableInventoryCargoTidy = false;
	}
	
	override string SettingName()
	{
		return "Core Settings";
	}
};