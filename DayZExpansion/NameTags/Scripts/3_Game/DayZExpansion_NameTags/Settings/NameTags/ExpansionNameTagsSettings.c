/**
 * ExpansionNameTagsSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	bool ShowPlayerTagsInSafeZones;
	bool ShowPlayerTagsInTerritories;
}

/**@class		ExpansionNameTagsSettings
 * @brief		Name tags settings class
 **/
class ExpansionNameTagsSettings: ExpansionNameTagsSettingsBase
{
	static const int VERSION = 0;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnRecieve
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::OnRecieve - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::OnRecieve - End");
		#endif

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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.NameTags, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::Send - End and return");
		#endif
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::CopyInternal - Start");
		#endif
				
		ExpansionNameTagsSettingsBase sb = s;
		CopyInternal( sb );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::CopyInternal - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings CopyInternal
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionNameTagsSettingsBase s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::CopyInternal - Start");
		#endif
		
		EnablePlayerTags = s.EnablePlayerTags;
		PlayerTagViewRange = s.PlayerTagViewRange;
		PlayerTagsIcon = s.PlayerTagsIcon;
		ShowPlayerTagsInSafeZones = s.ShowPlayerTagsInSafeZones;
		ShowPlayerTagsInTerritories = s.ShowPlayerTagsInTerritories;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::CopyInternal - End");
		#endif
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::Load - Start");
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
				//! Nothing to do here yet
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNameTagsSettings::Load - End - Loaded: " + nameTagsSettingsExist);
		#endif
		
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
		ShowPlayerTagsInSafeZones = true;
		ShowPlayerTagsInTerritories= true;
	}
		
	// ------------------------------------------------------------
	// ExpansionBookSettings SettingName
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Name Tags Settings";
	}
};