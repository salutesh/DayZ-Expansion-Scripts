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
	
	bool EnablePartyTab;
	bool EnableServerInfoTab;
	bool EnableServerRulesTab;
	bool EnableTerritoryTab;
	
	bool ShowServerSettings;
	
	ref ExpansionServerInfos ServerInfo;
	ref ExpansionServerRules ServerRules;
	
	string ServerLogoPath;

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
		EnablePartyTab = s.EnablePartyTab;
		EnableServerInfoTab = s.EnableServerInfoTab;
		EnableServerRulesTab = s.EnableServerRulesTab;
		EnableTerritoryTab = s.EnableTerritoryTab;

		ShowServerSettings = s.ShowServerSettings;
		
		ServerInfo = s.ServerInfo;
		ServerRules = s.ServerRules;
		ServerLogoPath = s.ServerLogoPath;
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
       
		EnablePartyTab = true;
		EnableServerInfoTab = true;
		EnableServerRulesTab = true;
		EnableTerritoryTab = true;
		
		ShowServerSettings = true;
		
		array<ref ExpansionServerInfoButtonData> ServerButtons = new array<ref ExpansionServerInfoButtonData>;
		ExpansionServerInfoButtonData button = new ExpansionServerInfoButtonData( "set:expansion_iconset image:icon_group", "https://exp.thurston.pw", "Feedback", ARGB(255,255,255,255) );
		ServerButtons.Insert( button );
		
		button = new ExpansionServerInfoButtonData( "set:expansion_iconset image:icon_discord", "https://discord.io/expansion", "Discord", ARGB(255,255,255,255) );
		ServerButtons.Insert( button );
		
		button = new ExpansionServerInfoButtonData( "set:expansion_iconset image:icon_home", "https://www.google.com", "Homepage", ARGB(255,255,255,255) );
		ServerButtons.Insert( button );
		
		button = new ExpansionServerInfoButtonData( "set:expansion_iconset image:icon_forums", "https://www.google.com", "Forums", ARGB(255,255,255,255) );
		ServerButtons.Insert( button );
		
		button = new ExpansionServerInfoButtonData( "set:expansion_iconset image:icon_patreon", "https://www.patreon.com/dayzexpansion", "Patreon", ARGB(255,255,255,255) );
		ServerButtons.Insert( button );
		
		array<ref ExpansionServerInfoSection> ServerSections = new array<ref ExpansionServerInfoSection>;
		ExpansionServerInfoSection section = new ExpansionServerInfoSection(true, "PLACEHOLDER", "<p>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vestibulum tortor quam, feugiat vitae, ultricies eget, tempor sit amet, ante. Donec eu libero sit amet quam egestas semper. Aenean ultricies mi vitae est. Mauris placerat eleifend leo.</p>");
		ServerSections.Insert( section );
		
		section = new ExpansionServerInfoSection(true, "PLACEHOLDER 2", "<p>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vestibulum tortor quam, feugiat vitae, ultricies eget, tempor sit amet, ante. Donec eu libero sit amet quam egestas semper. Aenean ultricies mi vitae est. Mauris placerat eleifend leo. Quisque sit amet est et sapien ullamcorper pharetra. Vestibulum erat wisi, condimentum sed, commodo vitae, ornare sit amet, wisi. Aenean fermentum, elit eget tincidunt condimentum, eros ipsum rutrum orci, sagittis tempus lacus enim ac dui. Donec non enim in turpis pulvinar facilisis. Ut felis. Praesent dapibus, neque id cursus faucibus, tortor neque egestas augue, eu vulputate magna eros eu erat. Aliquam erat volutpat. Nam dui mi, tincidunt quis, accumsan porttitor, facilisis luctus, metus</p>");
		ServerSections.Insert( section );
		
		section = new ExpansionServerInfoSection(true, "PLACEHOLDER 3", "<p>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vestibulum tortor quam, feugiat vitae, ultricies eget, tempor sit amet, ante. Donec eu libero sit amet quam egestas semper. Aenean ultricies mi vitae est. Mauris placerat eleifend leo. Quisque sit amet est et sapien ullamcorper pharetra. Vestibulum erat wisi, condimentum sed, commodo vitae, ornare sit amet, wisi. Aenean fermentum, elit eget tincidunt condimentum, eros ipsum rutrum orci, sagittis tempus lacus enim ac dui. Donec non enim in turpis pulvinar facilisis. Ut felis. Praesent dapibus, neque id cursus faucibus, tortor neque egestas augue, eu vulputate magna eros eu erat. Aliquam erat volutpat. Nam dui mi, tincidunt quis, accumsan porttitor, facilisis luctus, metus</p>");
		ServerSections.Insert( section );
		
		section = new ExpansionServerInfoSection(true, "PLACEHOLDER 4", "<p>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vestibulum tortor quam, feugiat vitae, ultricies eget, tempor sit amet, ante. Donec eu libero sit amet quam egestas semper. Aenean ultricies mi vitae est. Mauris placerat eleifend leo. Quisque sit amet est et sapien ullamcorper pharetra. Vestibulum erat wisi, condimentum sed, commodo vitae, ornare sit amet, wisi. Aenean fermentum, elit eget tincidunt condimentum, eros ipsum rutrum orci, sagittis tempus lacus enim ac dui. Donec non enim in turpis pulvinar facilisis. Ut felis. Praesent dapibus, neque id cursus faucibus, tortor neque egestas augue, eu vulputate magna eros eu erat. Aliquam erat volutpat. Nam dui mi, tincidunt quis, accumsan porttitor, facilisis luctus, metus</p>");
		ServerSections.Insert( section );
		
		section = new ExpansionServerInfoSection(true, "PLACEHOLDER 5", "<p>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vestibulum tortor quam, feugiat vitae, ultricies eget, tempor sit amet, ante. Donec eu libero sit amet quam egestas semper. Aenean ultricies mi vitae est. Mauris placerat eleifend leo.</p>");
		ServerSections.Insert( section );
		
		string servername = "SERVER NAME";
		
		ServerInfo = new ExpansionServerInfos(ServerButtons, ServerSections, servername);
		
		array<ref ExpansionServerRulesPage> RulesPages = new array<ref ExpansionServerRulesPage>;
		ExpansionServerRulesPage rule = new ExpansionServerRulesPage("Rule 1", "This is a simple test text.");
		RulesPages.Insert( rule );
		
		rule = new ExpansionServerRulesPage("Rule 2", "This is a simple test text.");
		RulesPages.Insert( rule );
		
		array<ref ExpansionServerRulesSection> RulesSections = new array<ref ExpansionServerRulesSection>;		
		ExpansionServerRulesSection rulesection = new ExpansionServerRulesSection("General Rules", RulesPages);
		RulesSections.Insert( rulesection );
		
		ServerRules = new ExpansionServerRules(RulesSections);
		
		ServerLogoPath = "set:expansion_gui_logos image:expansion_logo_black";
	}
}