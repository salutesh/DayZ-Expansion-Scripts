/**
 * ExpansionBookSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBookSettings
 * @brief		Book settings class
 **/
class ExpansionBookSettings: ExpansionSettingBase
{
    bool EnableBook;
	bool EnableStatusTab;
	bool DisplayServerUptimeInStatusTab;
	bool DisplayPlayerCountInStatusTab;
	
	bool EnablePartyTab;
	bool EnableServerInfoTab;
	bool EnableTerritoryTab;
	
	ref ExpansionServerInfos ServerInfo;
	
	string ServerLogoPath;

	bool WebsiteButton;
	string WebsiteURL;

	bool ForumsButton;
	string ForumsURL;

	bool DiscordButton;
	string DiscordURL;

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionBookSettings()
	{
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::HandleRPC - Start");
		#endif
		ExpansionBookSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionBookSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Book.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::HandleRPC - End");
		#endif
    }
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionBookSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Book, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
    override bool Copy( ExpansionSettingBase setting )
    {
		ExpansionBookSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionBookSettings s )
	{
		EnableBook = s.EnableBook;
		EnableStatusTab = s.EnableStatusTab;
		DisplayServerUptimeInStatusTab = s.DisplayServerUptimeInStatusTab;
		DisplayPlayerCountInStatusTab = s.DisplayPlayerCountInStatusTab;
		EnablePartyTab = s.EnablePartyTab;
		EnableServerInfoTab = s.EnableServerInfoTab;
		EnableTerritoryTab = s.EnableTerritoryTab;

		ServerInfo = s.ServerInfo;
		ServerLogoPath = s.ServerLogoPath;
		WebsiteButton = s.WebsiteButton;
		WebsiteURL = s.WebsiteURL;
		ForumsButton = s.ForumsButton;
		ForumsURL = s.ForumsURL;
		DiscordButton = s.DiscordButton;
		DiscordURL = s.DiscordURL;
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
		EXPrint("ExpansionBookSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_BOOK_SETTINGS ) )
		{
			JsonFileLoader<ExpansionBookSettings>.JsonLoadFile( EXPANSION_BOOK_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::Load - End");
		#endif
			return true;
		}
		
		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::Load - End");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionBookSettings] Saving settings");
		#endif
		JsonFileLoader<ExpansionBookSettings>.JsonSaveFile( EXPANSION_BOOK_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
    override void Defaults()
	{
        #ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionBookSettings] Loading default settings");
		#endif
		
		EnableBook = true;
       
		EnableStatusTab = true;
		DisplayServerUptimeInStatusTab = true;
		DisplayPlayerCountInStatusTab = true;
       
		EnablePartyTab = true;
		EnableServerInfoTab = true;
		EnableTerritoryTab = true;
		
        ServerInfo = new ExpansionServerInfos("DAYZ EXPANSION TEST SERVER", "DAYZ EXPANSION", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	
		ServerLogoPath = "set:expansion_gui_logos image:expansion_logo_black";
		
        WebsiteButton = false;
        WebsiteURL = "https://www.dayzexpansion.com/page";

        ForumsButton = false;
        ForumsURL = "https://www.dayzexpansion.com/forums";

        DiscordButton = true;
        DiscordURL = "https://discord.io/expansion";
    }
}