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

// ------------------------------------------------------------
class ExpansionRulesCategory
{
	autoptr array< ref ExpansionRuleButton > RuleButtons;

	string DisplayName;

	void ExpansionRulesCategory()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRulesCategory::ExpansionRulesCategory - Start");
		#endif
		
		RuleButtons = new array< ref ExpansionRuleButton >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRulesCategory::ExpansionRulesCategory - End");
		#endif
	}
}

// ------------------------------------------------------------
class ExpansionRuleButton
{
	ref ExpansionRuleSection RuleSection;
	
	string DetailLabel;
	string DetailContent;
	string Icon;
	
	void ExpansionRuleButton()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRulesButton::ExpansionRulesButton - Start");
		#endif
		
		RuleSection = new ExpansionRuleSection;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRulesButton::ExpansionRulesButton - End");
		#endif
	}
}

// ------------------------------------------------------------
class ExpansionRuleSection
{
	autoptr array< ref ExpansionRule > Rules;
	
	string DetailLabel;
	
	void ExpansionRuleSection()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRule::ExpansionRuleSection - Start");
		#endif
		
		Rules = new array< ref ExpansionRule >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRule::ExpansionRuleSection - End");
		#endif
	}
}

// ------------------------------------------------------------
class ExpansionRule
{
	string DetailContent;
	
	void ExpansionRule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRule::ExpansionRule - Start");
		#endif
		
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRule::ExpansionRule - End");
		#endif
	}
}

// ------------------------------------------------------------
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
	
	string ServerLogoPath;
	
	autoptr array< ref ExpansionRulesCategory > RuleCategorys;
	int RuleParagraphColor;
	int RuleButtonColor;
	bool ShowTooltipOnRuleButton;
	
	[NonSerialized()]
	private ExpansionRulesCategory m_CurrentCategory;

	[NonSerialized()]
	private ExpansionRuleButton m_CurrentButton;
	
	[NonSerialized()]
	private ExpansionRuleSection m_CurrentSection;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	// ExpansionBookSettings
	// ------------------------------------------------------------
	void ExpansionBookSettings()
	{
		RuleCategorys = new array< ref ExpansionRulesCategory >;
	}
	
	// ------------------------------------------------------------
	// OnRecieve
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::OnRecieve - Start");
		#endif
		ExpansionBookSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionBookSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Book.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionBookSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	// Send
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Book, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::Send - End and return");
		#endif
		
		return 0;
	}
	
	// ------------------------------------------------------------
	// Copy
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
	// CopyInternal
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
		ServerLogoPath = s.ServerLogoPath;
		
		RuleCategorys = s.RuleCategorys;
		RuleParagraphColor = s.RuleParagraphColor;
		RuleButtonColor = s.RuleButtonColor;
		ShowTooltipOnRuleButton = s.ShowTooltipOnRuleButton;
	}
	
	// ------------------------------------------------------------
	// IsLoaded
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	// Unload
	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}
	
	// ------------------------------------------------------------
	// OnLoad
	// ------------------------------------------------------------
	override bool OnLoad()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_BOOK_SETTINGS ) )
		{
			Print("[ExpansionBookSettings] Loading settings");

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
	// OnSave
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionBookSettings] Saving settings");

		JsonFileLoader<ExpansionBookSettings>.JsonSaveFile( EXPANSION_BOOK_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	// Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		Print("[ExpansionBookSettings] Loading default settings");
		
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
		
		button = new ExpansionServerInfoButtonData( "set:expansion_iconset image:icon_discord", "https://www.google.com", "Discord", ARGB(255,255,255,255) );
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
		
		ServerLogoPath = "set:expansion_gui_logos image:expansion_logo_black";
		
		DefaultRules();
		
		RuleParagraphColor = ARGB(255,52,152,219);
		RuleButtonColor = ARGB(255,52,152,219);
		ShowTooltipOnRuleButton = false;
	}
	
	// ------------------------------------------------------------
	// Defaults
	// ------------------------------------------------------------
	void DefaultRules()
	{		
		//! Category 1
		CreateRuleCategory("Hacking and Cheating");
		
		//! Category 1 - Rule Section 1
		CreateRuleButton("Glitching", "Rule section 1 description", ExpansionIcons.GetPath("Tent"));
		CreateRuleSection("Glitching");
		
		//! Category 1 - Rule Section 1 Rules
		CreateRule("Glichting into bases is a ban reason and will not be tolerated.");
		CreateRule("Glichting under the map or clipping thrue objects will not be tolerated.");
		
		//! Category 1 - Rule Section 2
		CreateRuleButton("Cheating", "Rule section 2 description", ExpansionIcons.GetPath("Skull 2"));
		CreateRuleSection("Cheating");
		
		//! Category 1 - Rule Section 2 Rules
		CreateRule("Any kind of cheats or third-party programms are not allowed and will be treated accordingly.");
	
		//! Category 2
		CreateRuleCategory("Etiquette");
		
		//! Category 2 - Rule Section 1
		CreateRuleButton("Insults", "Rule section 1 description", ExpansionIcons.GetPath("Info"));
		CreateRuleSection("Insults");
		
		//! Category 2 - Rule Section 1 Rules
		CreateRule("Insults of all kinds will not be tolerated unless they are part of an roleplay.");
		CreateRule("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut et mi non purus varius maximus. Pellentesque interdum, justo id luctus euismod, urna sapien convallis justo, non feugiat sapien orci a leo. Mauris lobortis sapien in erat interdum pretium. Integer ultrices odio lobortis lectus gravida, ac congue velit consequat. Nullam tempor lectus mollis, vulputate nibh sed, interdum ante. Donec finibus turpis purus. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Aliquam non diam id odio varius dignissim. Nulla nec est ut libero imperdiet suscipit. Maecenas et arcu non purus sollicitudin venenatis.");
	}
	
	// ------------------------------------------------------------
	// CreateRuleCategory
	// ------------------------------------------------------------
	private void CreateRuleCategory( string displayName )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRuleCategory - Start");
		#endif
		
		ExpansionRulesCategory category = new ref ExpansionRulesCategory;

		category.DisplayName = displayName;

		RuleCategorys.Insert( category );
		m_CurrentCategory = category;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRuleCategory - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// CreateRuleButton
	// ------------------------------------------------------------
	private void CreateRuleButton( string detailLabel, string detailContent, string icon )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRuleButton - Start");
		#endif
		
		ExpansionRuleButton button = new ref ExpansionRuleButton;

		button.DetailLabel = detailLabel;
		button.DetailContent = detailContent;
		button.Icon = icon;
				
		m_CurrentCategory.RuleButtons.Insert( button );
		m_CurrentButton = button;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRuleButton - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// CreateRuleSection
	// ------------------------------------------------------------
	private void CreateRuleSection( string detailLabel )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRuleSection - Start");
		#endif
		
		ExpansionRuleSection rulesection = new ref ExpansionRuleSection;

		rulesection.DetailLabel = detailLabel;

		m_CurrentButton.RuleSection = rulesection;
		m_CurrentSection = rulesection;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRuleSection - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// CreateRule
	// ------------------------------------------------------------
	private void CreateRule( string detailContent )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRule - Start");
		#endif
		
		ExpansionRule rule = new ref ExpansionRule;

		rule.DetailContent = detailContent;

		m_CurrentSection.Rules.Insert( rule );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabServerRules::CreateRule - End");
		#endif
	}
}