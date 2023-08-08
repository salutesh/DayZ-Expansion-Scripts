/**
 * ExpansionBookSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBookSettingsBase
 * @brief		Book settings base class
 **/
class ExpansionBookSettingsBase: ExpansionSettingBase
{
	bool EnableStatusTab;
	bool EnablePartyTab;
	bool EnableServerInfoTab;
	bool EnableServerRulesTab;
	bool EnableTerritoryTab;
}

class ExpansionBookSettingsV1: ExpansionBookSettingsBase
{
	bool EnableBook;
	ref ExpansionServerInfos ServerInfo;
	autoptr array< ref ExpansionRulesCategory > RuleCategories;
}

/**
 * @class		ExpansionBookSettings
 * @brief		Book settings class
 */
class ExpansionBookSettings: ExpansionBookSettingsBase
{
	static const int VERSION = 4;

	bool EnableBookMenu;
	bool CreateBookmarks;

	bool ShowHaBStats;
	
#ifdef EXPANSIONMODAI
	bool ShowPlayerFaction;
#endif

	ref array<ref ExpansionBookRuleCategory> RuleCategories = new array<ref ExpansionBookRuleCategory>;
	bool DisplayServerSettingsInServerInfoTab;
	ref array<ref ExpansionBookSettingCategory> SettingCategories = new array<ref ExpansionBookSettingCategory>;
	ref array<ref ExpansionBookLink> Links = new array<ref ExpansionBookLink>;
	ref array<ref ExpansionBookDescriptionCategory> Descriptions = new array<ref ExpansionBookDescriptionCategory>;
	ref array<ref ExpansionBookCraftingCategory> CraftingCategories = new array<ref ExpansionBookCraftingCategory>;

	[NonSerialized()]
	private bool m_IsLoaded;

	override bool OnRecieve( ParamsReadContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
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

		return true;
	}

	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
	#endif

		ExpansionBookSettings thisSetting = this;
		ctx.Write( thisSetting );
	}

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
		rpc.Send( null, ExpansionSettingsRPC.Book, true, identity );

		return 0;
	}

	override bool Copy( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
	#endif

		ExpansionBookSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	protected void CopyInternal( ExpansionBookSettings s )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		EnableBookMenu = s.EnableBookMenu;
		CreateBookmarks = s.CreateBookmarks;
		RuleCategories = s.RuleCategories;
		DisplayServerSettingsInServerInfoTab = s.DisplayServerSettingsInServerInfoTab;
		SettingCategories = s.SettingCategories;
		Links = s.Links;
		Descriptions = s.Descriptions;
		CraftingCategories = s.CraftingCategories;
		ShowHaBStats = s.ShowHaBStats;
		
	#ifdef EXPANSIONMODAI
		ShowPlayerFaction = s.ShowPlayerFaction;
	#endif

		ExpansionBookSettingsBase sb = s;
		CopyInternal( sb );
	}

	protected void CopyInternal( ExpansionBookSettingsBase s )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		EnableStatusTab = s.EnableStatusTab;
		EnablePartyTab = s.EnablePartyTab;
		EnableServerInfoTab = s.EnableServerInfoTab;
		EnableServerRulesTab = s.EnableServerRulesTab;
		EnableTerritoryTab = s.EnableTerritoryTab;
	}

	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
	#endif

		m_IsLoaded = true;

		bool save;

		bool bookSettingsExist = FileExist(EXPANSION_BOOK_SETTINGS);

		if (bookSettingsExist)
		{
			CF_Log.Info("[ExpansionBookSettings] Load existing setting file:" + EXPANSION_BOOK_SETTINGS);

			ExpansionBookSettings settingsDefault = new ExpansionBookSettings;
			settingsDefault.Defaults();

			ExpansionBookSettingsBase settingsBase;

			JsonFileLoader<ExpansionBookSettingsBase>.JsonLoadFile(EXPANSION_BOOK_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					CF_Log.Info("[ExpansionBookSettings] Load - Converting v1 \"" + EXPANSION_BOOK_SETTINGS + "\" to v" + VERSION);

					ExpansionBookSettingsV1 settings_v1;
					JsonFileLoader<ExpansionBookSettingsV1>.JsonLoadFile(EXPANSION_BOOK_SETTINGS, settings_v1);

					//! Port old settings to v2
					EnableBookMenu = settings_v1.EnableBook;

					//! Convert old rules to new book rules v2
					ExpansionBookRuleCategory category;
					int paragraph = 1;
					foreach (ExpansionRulesCategory oldRuleCat : settings_v1.RuleCategories)
					{
						foreach (ExpansionRuleButton oldRuleButton : oldRuleCat.RuleButtons)
						{
							ExpansionRuleSection section = oldRuleButton.RuleSection;
							category = new ExpansionBookRuleCategory(section.DetailLabel);

							if (category)
							{
								for (int r = 0; r < section.Rules.Count(); r++)
								{
									ExpansionRule oldRule = section.Rules[r];
									category.AddRule(string.Format("%1.%2.", paragraph, r + 1), oldRule.DetailContent);
								}
							}
						}

						paragraph++;

						RuleCategories.Insert(category);
					}

					//! Convert old Server Buttons to new book links v2
					if (!Links)
						Links = new array<ref ExpansionBookLink>;

					if (Links.Count() > 0)
						Links.Clear();

					foreach (ExpansionServerInfoButtonData oldserverButton : settings_v1.ServerInfo.ServerButtons)
					{
						ExpansionBookLink link = new ExpansionBookLink(oldserverButton.Tooltip, oldserverButton.URL, "Homepage", oldserverButton.Color);
						Links.Insert(link);
					}

					//! Convert old Server Sections to new book descriptions v2
					if (!Descriptions)
						Descriptions = new array<ref ExpansionBookDescriptionCategory>;

					if (Descriptions.Count() > 0)
						Descriptions.Clear();

					foreach (ExpansionServerInfoSection oldserverSection : settings_v1.ServerInfo.ServerSections)
					{
						ExpansionBookDescriptionCategory description = new ExpansionBookDescriptionCategory(oldserverSection.HeadText);
						description.AddDescription(oldserverSection.BodyText);
						Descriptions.Insert(description);
					}

					//! New with v2
					CreateBookmarks = settingsDefault.CreateBookmarks;
					DisplayServerSettingsInServerInfoTab = settingsDefault.DisplayServerSettingsInServerInfoTab;

					if (!SettingCategories)
						SettingCategories = new array<ref ExpansionBookSettingCategory>;

					foreach (ExpansionBookSettingCategory settingCat : settingsDefault.SettingCategories)
					{
						SettingCategories.Insert(settingCat);
					}

					if (!CraftingCategories)
						CraftingCategories = new array<ref ExpansionBookCraftingCategory>;

					foreach (ExpansionBookCraftingCategory craftingCat : settingsDefault.CraftingCategories)
					{
						CraftingCategories.Insert(craftingCat);
					}
				}
				else
				{
					JsonFileLoader<ExpansionBookSettings>.JsonLoadFile(EXPANSION_BOOK_SETTINGS, this);
				}

				if (settingsBase.m_Version < 3)
					ShowHaBStats = settingsDefault.ShowHaBStats;

				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionBookSettings>.JsonLoadFile(EXPANSION_BOOK_SETTINGS, this);
			}

			foreach (ExpansionBookCraftingCategory cat : CraftingCategories)
			{
				for (int i = 0; i < cat.Results.Count(); i++)
				{
					string result = cat.Results[i];
					result.ToLower();
					cat.Results[i] = result;
				}
			}
		}
		else
		{
			CF_Log.Info("[ExpansionBookSettings] No existing setting file:" + EXPANSION_BOOK_SETTINGS + ". Creating defaults!");

			Defaults();
			save = true;
		}

		if (save)
		{
			Save();
		}

		return bookSettingsExist;
	}

	override bool OnSave()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnSave");
	#endif

		JsonFileLoader<ExpansionBookSettings>.JsonSaveFile( EXPANSION_BOOK_SETTINGS, this );

		return true;
	}

	override void Update( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_Book.Invoke();
	}

	override void Defaults()
	{
		m_Version = VERSION;

		EnableBookMenu = true;
		CreateBookmarks = false;
		EnableStatusTab = true;
		ShowHaBStats = true;
		EnablePartyTab = true;
		EnableServerInfoTab = true;
		EnableServerRulesTab = true;
		EnableTerritoryTab = true;
		DefaultRules();
		DisplayServerSettingsInServerInfoTab = true;
		DefaultSettings();
		DefaultLinks();
		DefaultDescriptions();
		DefaultCraftingCategories();
		
	#ifdef EXPANSIONMODAI
		ShowPlayerFaction = false;
	#endif
	}

	void DefaultRules()
	{
		ExpansionBookRuleCategory category = new ExpansionBookRuleCategory("General");
		category.AddRule("1.1.", "Insults, discrimination, extremist and racist statements or texts are taboo.");
		category.AddRule("1.2.", "We reserve the right to exclude people from the server who share extremist or racist ideas or who clearly disturb the server harmony.");
		category.AddRule("1.3.", "Decisions of the team members, both the supporter and the admin are to be accepted without discussion.");
		category.AddRule("1.4.", "Provocations and toxic behavior will not be tolerated and punished! Be friendly to fellow players and your team, both in chat and in voice!");
		category.AddRule("1.5.", "The use of external programs, scripts and cheats is not tolerated and is punished with a permanent exclusion.");

		RuleCategories.Insert(category);
	}

	void DefaultSettings()
	{
		ExpansionBookSettingCategory category;

	#ifdef EXPANSIONMODBASEBUILDING
		category = new ExpansionBookSettingCategory("Base-Building Settings");
		category.AddSetting("Expansion.Settings.BaseBuilding.CanCraftVanillaBasebuilding");
		category.AddSetting("Expansion.Settings.BaseBuilding.CanCraftExpansionBasebuilding");
		SettingCategories.Insert(category);

		category = new ExpansionBookSettingCategory("Raid Settings");
		category.AddSetting("Expansion.Settings.Raid.CanRaidSafes");
		category.AddSetting("Expansion.Settings.Raid.SafeExplosionDamageMultiplier");
		category.AddSetting("Expansion.Settings.Raid.SafeProjectileDamageMultiplier");
		category.AddSetting("Expansion.Settings.Raid.ExplosionTime");
		category.AddSetting("Expansion.Settings.Raid.ExplosionDamageMultiplier");
		category.AddSetting("Expansion.Settings.Raid.ProjectileDamageMultiplier");
		SettingCategories.Insert(category);
		
		category = new ExpansionBookSettingCategory("Territory Settings");
		category.AddSetting("Expansion.Settings.Territory.TerritorySize");
		category.AddSetting("Expansion.Settings.Territory.UseWholeMapForInviteList");
		SettingCategories.Insert(category);
	#endif

	#ifdef EXPANSIONMODNAVIGATION
		category = new ExpansionBookSettingCategory("Map Settings");
		category.AddSetting("Expansion.Settings.Map.NeedGPSItemForKeyBinding");
		category.AddSetting("Expansion.Settings.Map.NeedMapItemForKeyBinding");
		SettingCategories.Insert(category);
	#endif

	#ifdef EXPANSIONMODGROUPS
		category = new ExpansionBookSettingCategory("Party Settings");
		category.AddSetting("Expansion.Settings.Party.MaxMembersInParty");
		category.AddSetting("Expansion.Settings.Party.UseWholeMapForInviteList");
		SettingCategories.Insert(category);
	#endif
	}

	void DefaultLinks()
	{
		ExpansionBookLink link = new ExpansionBookLink("Homepage", "https://www.google.com/", "Homepage", ARGB(255,35,39,42));
		Links.Insert(link);

		link = new ExpansionBookLink("Feedback", "https://www.google.com/", "Forums", ARGB(255,35,39,42));
		Links.Insert(link);

		link = new ExpansionBookLink("Discord", "https://www.google.com/", "Discord", ARGB(255,114,137,218));
		Links.Insert(link);

		link = new ExpansionBookLink("Patreon", "https://www.patreon.com/dayzexpansion", "Patreon", ARGB(255,249,104,84));
		Links.Insert(link);

		link = new ExpansionBookLink("Steam", "https://steamcommunity.com/sharedfiles/filedetails/?id=2116151222", "Steam", ARGB(255,42,71,94));
		Links.Insert(link);

		link = new ExpansionBookLink("Reddit", "https://www.reddit.com/r/ExpansionProject/", "Reddit", ARGB(255,67,0,1));
		Links.Insert(link);

		link = new ExpansionBookLink("GitHub", "https://github.com/salutesh/DayZ-Expansion-Scripts/wiki", "GitHub", ARGB(255,0,0,0));
		Links.Insert(link);

		link = new ExpansionBookLink("YouTube", "https://www.youtube.com/channel/UCZNgSvIEWfru963tQZOAVJg", "YouTube", ARGB(255,255,0,0));
		Links.Insert(link);

		link = new ExpansionBookLink("Twitter", "https://twitter.com/DayZExpansion", "Twitter", ARGB(255,29,161,242));
		Links.Insert(link);
	}

	void DefaultDescriptions()
	{
		ExpansionBookDescriptionCategory category = new ExpansionBookDescriptionCategory("General Info");
		category.AddDescription("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
		category.AddDescription("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
		category.AddDescription("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");

		Descriptions.Insert(category);

		category = new ExpansionBookDescriptionCategory("Mod Info");
		category.AddDescription("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
		category.AddDescription("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
		category.AddDescription("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");

		Descriptions.Insert(category);
	}

	void DefaultCraftingCategories()
	{
		ExpansionBookCraftingCategory category = new ExpansionBookCraftingCategory("Accessories");
		category.AddResult("Armband_");
		category.AddResult("Armband_White");
		category.AddResult("EyePatch_Improvised");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Backpacks");
		category.AddResult("CourierBag");
		category.AddResult("FurCourierBag");
		category.AddResult("FurImprovisedBag");
		category.AddResult("ImprovisedBag");
		category.AddResult("LeatherSack_Brown");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Base-Building");
		category.AddResult("ExpansionBarbedWireKit");
		category.AddResult("FenceKit");
		category.AddResult("ExpansionFloorKit");
		category.AddResult("ExpansionHelipadKit");
		category.AddResult("ExpansionHescoKit");
		category.AddResult("ExpansionRampKit");
		category.AddResult("ShelterKit");
		category.AddResult("ExpansionStairKit");
		category.AddResult("TerritoryFlagKit");
		category.AddResult("ExpansionWallKit");
		category.AddResult("WatchtowerKit");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Camouflage");
		category.AddResult("Camonet");
		category.AddResult("GhillieAtt_Tan");
		category.AddResult("GhillieAtt_Mossy");
		category.AddResult("GhillieAtt_Woodland");
		category.AddResult("GhillieBushrag_Tan");
		category.AddResult("GhillieBushrag_Mossy");
		category.AddResult("GhillieBushrag_Woodland");
		category.AddResult("GhillieHood_Tan");
		category.AddResult("GhillieHood_Mossy");
		category.AddResult("GhillieHood_Woodland");
		category.AddResult("GhillieSuit_Tan");
		category.AddResult("GhillieSuit_Mossy");
		category.AddResult("GhillieSuit_Woodland");
		category.AddResult("GhillieTop_Tan");
		category.AddResult("GhillieTop_Mossy");
		category.AddResult("GhillieTop_Woodland");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Cooking");
		category.AddResult("Fireplace");
		category.AddResult("Firewood");
		category.AddResult("HandDrillKit");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Fishing");
		category.AddResult("Bait");
		category.AddResult("BoneBait");
		category.AddResult("BoneHook");
		category.AddResult("ImprovisedFishingRod");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Food");
		category.AddResult("CarpFilletMeat");
		category.AddResult("MackerelFilletMeat");
		category.AddResult("ExpansionMilkBottle");
		category.AddResult("Potato");
		category.AddResult("SlicedPumpkin");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Horticulture");
		category.AddResult("PepperSeeds");
		category.AddResult("PumpkinSeeds");
		category.AddResult("TomatoSeeds");
		category.AddResult("ZucchiniSeeds");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Lights");
		category.AddResult("LongTorch");
		category.AddResult("Torch");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Medical Supplies");
		category.AddResult("BloodBagIV");
		category.AddResult("SalineBagIV");
		category.AddResult("Splint");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Melee Weapons");
		category.AddResult("NailedBaseballBat");
		category.AddResult("StoneKnife");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Storage");
		category.AddResult("WoodenCrate");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Supplies");
		category.AddResult("BoarPelt");
		category.AddResult("BurlapSack");
		category.AddResult("BurlapStrip");
		category.AddResult("LongWoodenStick");
		category.AddResult("ExpansionLumber1");
		category.AddResult("ExpansionLumber1_5");
		category.AddResult("ExpansionLumber3");
		category.AddResult("Nails");
		category.AddResult("Netting");
		category.AddResult("Rag");
		category.AddResult("Rope");
		category.AddResult("SharpWoodenStick");
		category.AddResult("SmallStone");
		category.AddResult("TannedLeather");
		category.AddResult("WoodenPlank");
		category.AddResult("WoodenStick");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Weapon Modifications");
		category.AddResult("SawedoffIzh18Shotgun");
		CraftingCategories.Insert(category);

		category = new ExpansionBookCraftingCategory("Weapon Attachments");
		category.AddResult("ImprovisedSuppressor");
		CraftingCategories.Insert(category);
	}

	override string SettingName()
	{
		return "Book Settings";
	}
};

//---------------------------------------------------------------//
// Old leacy classes (just still here for the setting conversion //
//---------------------------------------------------------------//
class ExpansionRulesCategory
{
	autoptr array< ref ExpansionRuleButton > RuleButtons;
	string DisplayName;
};

class ExpansionRuleButton
{
	ref ExpansionRuleSection RuleSection;
	string DetailLabel;
	string DetailContent;
	string Icon;
};

class ExpansionRuleSection
{
	autoptr array< ref ExpansionRule > Rules;
	string DetailLabel;
};

class ExpansionServerInfos
{
	ref array<ref ExpansionServerInfoButtonData> ServerButtons;
	ref array<ref ExpansionServerInfoSection> ServerSections;
	string ServerName;
};

class ExpansionServerInfoButtonData
{
	string IconPath;
	string URL;
	string Tooltip;
	int Color;
};

class ExpansionServerInfoSection
{
	bool HasHeading;
	string HeadText;
	string BodyText;
};

class ExpansionRule
{
	string DetailContent;
};
