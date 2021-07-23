/**
 * ExpansionSocialMediaSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSocialMediaSettingsBase
 * @brief		Social Media settings base class
 **/
class ExpansionSocialMediaSettingsBase: ExpansionSettingBase
{
	string Discord;
	string Homepage;
	string Forums;
	string YouTube;
	string Steam;
	string Twitter;
	string Guilded;
}

/**@class		ExpansionSocialMediaSettings
 * @brief		Social Media settings class
 **/
class ExpansionSocialMediaSettings: ExpansionSocialMediaSettingsBase
{
	static const int VERSION = 0;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::OnRecieve - Start");
		#endif
		
		ExpansionSocialMediaSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionSocialMediaSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_SocialMedia.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionSocialMediaSettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.SocialMedia, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::Copy - Start");
		#endif
		
		ref ExpansionSocialMediaSettings settings = ExpansionSocialMediaSettings.Cast( setting );
		if ( !settings )
			return false;

		CopyInternal( settings );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::Send - End and return");
		#endif
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionSocialMediaSettings s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::CopyInternal - Start");
		#endif
		
		//! Nothing to do here yet
		
		ExpansionSocialMediaSettingsBase sb = s;
		CopyInternal( sb );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::CopyInternal - End");
		#endif
	}
	
	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionSocialMediaSettingsBase s )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::CopyInternal - Start");
		#endif
		
		Discord = s.Discord;
		Homepage = s.Homepage;
		Forums = s.Forums;
		YouTube = s.YouTube;
		Steam = s.Steam;
		Twitter = s.Twitter;
		Guilded = s.Guilded;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::CopyInternal - End");
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
		EXPrint("ExpansionSocialMediaSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		bool save;

		bool socialMediaSettingsExist = FileExist(EXPANSION_SOCIALMEDIA_SETTINGS);

		if (socialMediaSettingsExist)
		{
			ExpansionSocialMediaSettings settingsDefault = new ExpansionSocialMediaSettings;
			settingsDefault.Defaults();

			ExpansionSocialMediaSettingsBase settingsBase;

			JsonFileLoader<ExpansionSocialMediaSettingsBase>.JsonLoadFile(EXPANSION_SOCIALMEDIA_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					EXPrint("[ExpansionSocialMediaSettings] Load - Converting v1 \"" + EXPANSION_SOCIALMEDIA_SETTINGS + "\" to v" + VERSION);
					//! Nothing to do here yet
				}
				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionSocialMediaSettings>.JsonLoadFile(EXPANSION_SOCIALMEDIA_SETTINGS, this);
			}
		}
		else
		{
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::Load - End - Loaded: " + socialMediaSettingsExist);
		#endif
		
		return socialMediaSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionSocialMediaSettings] Saving settings");

		JsonFileLoader<ExpansionSocialMediaSettings>.JsonSaveFile( EXPANSION_SOCIALMEDIA_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_SocialMedia.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		Print("[ExpansionSocialMediaSettings] Loading default settings");
	
		m_Version = VERSION;
		
		Discord = "";
		Guilded = "";
		Homepage = "";
		Forums = "";
		YouTube = "";
		Steam = "";
		Twitter = "";
	}
	
	override string SettingName()
	{
		return "Social Media Settings";
	}
};