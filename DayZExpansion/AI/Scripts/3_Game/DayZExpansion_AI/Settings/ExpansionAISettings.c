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

/**@class		ExpansionAISettings
 * @brief		Spawn settings class
 **/
class ExpansionAISettings: ExpansionSettingBase
{
	static const int VERSION = 5;

	float AccuracyMin;
	float AccuracyMax;

	float ThreatDistanceLimit;
	float DamageMultiplier;

	autoptr TStringArray Admins

	int MaximumDynamicPatrols;

	bool Vaulting;

	bool Manners;

	bool CanRecruitGuards;

#ifdef DIAG
	float FormationScale;
#endif

	autoptr TStringArray PlayerFactions;  //! If non-empty, player will automatically join one of these factions on connect

	[NonSerialized()]
	private bool m_IsAdmin;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	void ExpansionAISettings()
	{
		Admins = new TStringArray;
		PlayerFactions = new TStringArray;
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		if ( !ctx.Read( AccuracyMin ) )
		{
			Error("ExpansionAISettings::OnRecieve AccuracyMin");
			return false;
		}

		if ( !ctx.Read( AccuracyMax ) )
		{
			Error("ExpansionAISettings::OnRecieve AccuracyMax");
			return false;
		}

		if ( !ctx.Read( Vaulting ) )
		{
			Error("ExpansionAISettings::OnRecieve Vaulting");
			return false;
		}

		if ( !ctx.Read( Manners ) )
		{
			Error("ExpansionAISettings::OnRecieve Manners");
			return false;
		}

		if ( !ctx.Read( CanRecruitGuards ) )
		{
			Error("ExpansionAISettings::OnRecieve CanRecruitGuards");
			return false;
		}

#ifdef DIAG
		if ( !ctx.Read( FormationScale ) )
		{
			Error("ExpansionAISettings::OnRecieve FormationScale");
			return false;
		}
#endif

		if ( !ctx.Read( m_IsAdmin ) )
		{
			Error("ExpansionAISettings::OnRecieve m_IsAdmin");
			return false;
		}
		
		m_IsLoaded = true;
		
		EXLogPrint("Received AI settings");

		ExpansionSettings.SI_AI.Invoke();

		return true;
	}
	
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
#endif

		ctx.Write( AccuracyMin );
		ctx.Write( AccuracyMax );
		ctx.Write( Vaulting );
		ctx.Write( Manners );
		ctx.Write( CanRecruitGuards );

#ifdef DIAG
		ctx.Write( FormationScale );
#endif
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
		
		auto rpc = ExpansionScriptRPC.Create();
		OnSend( rpc );
		rpc.Write( IsAdmin( identity ) );
		rpc.Send( null, ExpansionSettingsRPC.AI, true, identity );
		
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
		Admins.Copy(s.Admins);
		MaximumDynamicPatrols = s.MaximumDynamicPatrols;
		Vaulting = s.Vaulting;
		Manners = s.Manners;

#ifdef DIAG
		FormationScale = s.FormationScale;
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
					if (!MaximumDynamicPatrols)
						MaximumDynamicPatrols = settingsDefault.MaximumDynamicPatrols;
				}

				if (m_Version < 2)
				{
					Manners = settingsDefault.Manners;
#ifdef DIAG
					FormationScale = settingsDefault.FormationScale;
#endif
				}

				if (m_Version < 4)
				{
					ThreatDistanceLimit = settingsDefault.ThreatDistanceLimit;
					DamageMultiplier = settingsDefault.DamageMultiplier;
				}

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

		//! Copy over eAI loadouts
		string humanLoadout = "$profile:eAI\\HumanLoadout.json";
		string humanLoadoutAI = EXPANSION_LOADOUT_FOLDER + "HumanLoadout.json";
		if (!FileExist(humanLoadoutAI) && FileExist(humanLoadout))
		{
			EXLogPrint("[ExpansionAISettings] Copying loadout " + humanLoadout + " to " + humanLoadoutAI);
			CopyFile(humanLoadout, humanLoadoutAI);
		}
		
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

		AccuracyMin = 0.15;
		AccuracyMax = 0.75;

		ThreatDistanceLimit = 1000.0;
		DamageMultiplier = 1.0;

		MaximumDynamicPatrols = -1;
		Vaulting = true;
		Manners = false;

#ifdef DIAG
		FormationScale = 0.15;
#endif
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
