/**
 * ExpansionNameTagsSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionNameTagsSettingsBase
 * @brief		Name tags settings base class
 **/
class ExpansionNameTagsSettingsBase: ExpansionSettingBase
{
	bool EnablePlayerTags;
	int PlayerTagViewRange;
	string PlayerTagsIcon;
}

class ExpansionNameTagsSettingsV0: ExpansionNameTagsSettingsBase
{
	bool ShowPlayerTagsInSafeZones;
	bool ShowPlayerTagsInTerritories;
}

/**@class		ExpansionNameTagsSettings
 * @brief		Name tags settings class
 **/
class ExpansionNameTagsSettings: ExpansionNameTagsSettingsBase
{
	static const int VERSION = 2;

	int PlayerTagsColor;
	int PlayerNameColor;

	bool OnlyInSafeZones;
	bool OnlyInTerritories;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnRecieve
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		ExpansionNameTagsSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_NameTags.Invoke();
	
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnSend
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionNameTagsSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Send
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.NameTags, true, identity );
		
		return 0;
	}

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Copy
	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionNameTagsSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionNameTagsSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		ExpansionNameTagsSettingsBase sb = s;
		CopyInternal( sb );
		PlayerTagsColor = s.PlayerTagsColor;
		PlayerNameColor = s.PlayerNameColor;

		OnlyInSafeZones = s.OnlyInSafeZones;
		OnlyInTerritories = s.OnlyInTerritories;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings CopyInternal
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionNameTagsSettingsBase s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		EnablePlayerTags = s.EnablePlayerTags;

		PlayerTagViewRange = s.PlayerTagViewRange;
		PlayerTagsIcon = s.PlayerTagsIcon;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings IsLoaded
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings Unload
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

		bool nameTagsSettingsExist = FileExist(EXPANSION_NAMETAGS_SETTINGS);

		if (nameTagsSettingsExist)
		{
			EXPrint("[ExpansionNameTagsSettings] Load existing setting file:" + EXPANSION_NAMETAGS_SETTINGS);
			
			ExpansionNameTagsSettings settingsDefault = new ExpansionNameTagsSettings;
			settingsDefault.Defaults();

			ExpansionNameTagsSettingsBase settingsBase;
			JsonFileLoader<ExpansionNameTagsSettingsBase>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionNotificationSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_NAMETAGS_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 1)
				{
					CopyInternal(settingsBase);

					PlayerTagsColor = settingsDefault.PlayerTagsColor;
					PlayerNameColor = settingsDefault.PlayerNameColor;

					ExpansionNameTagsSettingsV0 settingsV0;
					JsonFileLoader<ExpansionNameTagsSettingsV0>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, settingsV0);
					OnlyInSafeZones = settingsV0.ShowPlayerTagsInSafeZones;
					OnlyInTerritories = settingsV0.ShowPlayerTagsInTerritories;
				}
				else
				{
					JsonFileLoader<ExpansionNameTagsSettings>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, this);
				}

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionNameTagsSettings>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, this);
			}
		}
		else
		{
			EXPrint("[ExpansionNameTagsSettings] No existing setting file:" + EXPANSION_NAMETAGS_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return nameTagsSettingsExist;
	}

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnSave
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionNameTagsSettings] Saving settings");

		JsonFileLoader<ExpansionNameTagsSettings>.JsonSaveFile( EXPANSION_NAMETAGS_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Update
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_NameTags.Invoke();
	}

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;	
		
		EnablePlayerTags = true;
		PlayerTagViewRange = 5;
		PlayerTagsIcon = EXPANSION_NOTIFICATION_ICON_PERSONA;
		PlayerTagsColor = -1;
		PlayerNameColor = -1;
		OnlyInSafeZones = false;
		OnlyInTerritories = false;
	}
		
	// ------------------------------------------------------------
	// ExpansionBookSettings SettingName
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Name Tags Settings";
	}
};