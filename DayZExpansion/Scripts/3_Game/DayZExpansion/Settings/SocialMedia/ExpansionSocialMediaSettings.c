/**
 * ExpansionSocialMediaSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNewsFeedLinkSetting
{
	protected string m_Label;
	protected string m_Icon;
	protected string m_URL;
	
 	void ExpansionNewsFeedLinkSetting(string label, string icon, string url)
	{
		m_Label = label;
		m_Icon = icon;
		m_URL = url;
	}
	
	string GetLabel()
	{
		return m_Label;
	}
	
	string GetIcon()
	{
		return m_Icon;
	}
	
	string GetURL()
	{
		return m_URL;
	}
};

class ExpansionNewsFeedTextSetting
{
	protected string m_Title;
	protected string m_Text;
	
 	void ExpansionNewsFeedTextSetting(string title, string text)
	{
		m_Title = title;
		m_Text = text;
	}
	
	string GetTitle()
	{
		return m_Title;
	}
	
	string GetText()
	{
		return m_Text;
	}
};

/**@class		ExpansionSocialMediaSettingsBase
 * @brief		Social Media settings base class
 **/
class ExpansionSocialMediaSettingsBase: ExpansionSettingBase
{
	ref array<ref ExpansionNewsFeedTextSetting> NewsFeedTexts;
	ref array<ref ExpansionNewsFeedLinkSetting> NewsFeedLinks;
	
	void ExpansionSocialMediaSettingsBase()
	{
		NewsFeedTexts = new array<ref ExpansionNewsFeedTextSetting>;
		NewsFeedLinks = new array<ref ExpansionNewsFeedLinkSetting>;
	}
};

/**@class		ExpansionSocialMediaSettings
 * @brief		Social Media settings class
 **/
class ExpansionSocialMediaSettings: ExpansionSocialMediaSettingsBase
{
	static const int VERSION = 2;

	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve(ParamsReadContext ctx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		ExpansionSocialMediaSettings setting;
		if (!ctx.Read(setting))
		{
			Error("ExpansionSocialMediaSettings::OnRecieve setting");
			return false;
		}

		CopyInternal(setting);

		m_IsLoaded = true;

		ExpansionSettings.SI_SocialMedia.Invoke();

		return true;
	}
	
	override void OnSend(ParamsWriteContext ctx)
	{
		ExpansionSocialMediaSettings thisSetting = this;

		ctx.Write(thisSetting);
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if (!IsMissionHost())
		{
			return 0;
		}
		
		auto rpc = ExpansionScriptRPC.Create();
		OnSend(rpc);
		rpc.Send(null, ExpansionSettingsRPC.SocialMedia, true, identity);
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy(ExpansionSettingBase setting)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionSocialMediaSettings settings = ExpansionSocialMediaSettings.Cast(setting);
		if (!settings)
			return false;

		CopyInternal(settings);
		
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal(ExpansionSocialMediaSettings s)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		ExpansionSocialMediaSettingsBase sb = s;
		CopyInternal(sb);
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(ExpansionSocialMediaSettingsBase s)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		NewsFeedTexts = s.NewsFeedTexts;
		NewsFeedLinks = s.NewsFeedLinks;
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		//! Never unload social media settings cause they are used in main menu
	}
	
	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool socialMediaSettingsExist = FileExist(EXPANSION_SOCIALMEDIA_SETTINGS);

		if (socialMediaSettingsExist)
		{
			EXPrint("[ExpansionSocialMediaSettings] Load existing setting file:" + EXPANSION_SOCIALMEDIA_SETTINGS);
			
			ExpansionSocialMediaSettings settingsDefault = new ExpansionSocialMediaSettings;
			settingsDefault.Defaults();

			ExpansionSocialMediaSettingsBase settingsBase;

			JsonFileLoader<ExpansionSocialMediaSettingsBase>.JsonLoadFile(EXPANSION_SOCIALMEDIA_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionSocialMediaSettings] Load - Converting v0 \"" + EXPANSION_SOCIALMEDIA_SETTINGS + "\" to v" + VERSION);
				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);
				
				if (settingsBase.m_Version < 1)
				{
					DefaultNewsFeedTexts();
					DefaultNewsFeedLinks();
				}

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
			EXPrint("[ExpansionSocialMediaSettings] No existing setting file:" + EXPANSION_SOCIALMEDIA_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return socialMediaSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionSocialMediaSettings] Saving settings");

		JsonFileLoader<ExpansionSocialMediaSettings>.JsonSaveFile(EXPANSION_SOCIALMEDIA_SETTINGS, this);

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update(ExpansionSettingBase setting)
	{
		super.Update(setting);

		ExpansionSettings.SI_SocialMedia.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		DefaultNewsFeedTexts();
		DefaultNewsFeedLinks();
	}
	
	protected void DefaultNewsFeedTexts()
	{
		NewsFeedTexts.Insert(new ExpansionNewsFeedTextSetting("CHANGE ME", "THIS IS A PLACEHOLDER TEXT"));
	}
	
	protected void DefaultNewsFeedLinks()
	{
		NewsFeedLinks.Insert(new ExpansionNewsFeedLinkSetting("Discord", ExpansionIcons.GetPath("Discord"), "https://www.google.com/"));
		NewsFeedLinks.Insert(new ExpansionNewsFeedLinkSetting("Twitter", ExpansionIcons.GetPath("Twitter"), "https://www.google.com/"));
	}
	
	override string SettingName()
	{
		return "Social Media Settings";
	}
};