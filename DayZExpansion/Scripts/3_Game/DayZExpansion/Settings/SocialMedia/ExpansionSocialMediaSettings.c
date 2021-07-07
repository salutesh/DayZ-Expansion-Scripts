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

/**@class		ExpansionSocialMediaSettings
 * @brief		Social Media settings class
 **/
class ExpansionSocialMediaSettings: ExpansionSettingBase
{	
	string Discord;
	string Guilded;
	string Homepage;
	string Forums;
	string YouTube;
	string Steam;
	string Twitter;

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionSocialMediaSettings()
	{
	}

	// ------------------------------------------------------------
	void ~ExpansionSocialMediaSettings()
	{
	}
	
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
		
		Discord = s.Discord;
		Guilded = s.Guilded;
		Homepage = s.Homepage;
		Forums = s.Forums;
		YouTube = s.YouTube;
		Steam = s.Steam;
		Twitter = s.Twitter;
		
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
		
		if ( FileExist( EXPANSION_SOCIALMEDIA_SETTINGS ) )
		{
			Print("[ExpansionSocialMediaSettings] Loading settings");

			JsonFileLoader<ExpansionSocialMediaSettings>.JsonLoadFile( EXPANSION_SOCIALMEDIA_SETTINGS, this );
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionSocialMediaSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSocialMediaSettings::Load - End - Not Loaded");
		#endif
		return false;
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
		
		Discord = "";
		Guilded = "";
		Homepage = "";
		Forums = "";
		YouTube = "";
		Steam = "";
		Twitter = "";

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionSocialMediaSettings] Default settings loaded!");
		#endif
	}
	
	override string SettingName()
	{
		return "Social Media Settings";
	}
};