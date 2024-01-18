/**
 * InGameMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#ifndef CF_DebugUI
modded class InGameMenu
{
	protected static float DEAD_SCREEN_FADEIN_TIME = 3.0;
	protected static float DEAD_SCREEN_IMAGE_FADEIN_TIME = 1.5;
	protected float m_DeadScreenFadeInIncrement;
	protected float m_DeadScreenFadeInLevel;
	protected float m_DeadScreenImageFadeInIncrement;
	protected float m_DeadScreenImageFadeInLevel;
	protected float m_Distance;
	protected float m_Playtime;
	protected float m_PlayersKilled;
	protected float m_InfectedKilled;
	protected float m_AnimalsKilled;
	protected float m_LongestShot;
	protected float m_TimerSlice;

	protected ImageWidget m_Logo;
	protected WrapSpacerWidget m_Licensing;
	protected TextWidget m_DeadSourceVal;
	protected TextWidget m_LongestShotVal;
	protected TextWidget m_DistanceVal;
	protected TextWidget m_AnimalsKilledVal;
	protected TextWidget m_InfectedKilledVal;
	protected TextWidget m_PlayersKilledVal;
	protected TextWidget m_TimeSurvivedVal;
	protected ButtonWidget m_DeadScreenStatsHideButton;
	protected TextWidget m_DeadSceenStatsPanelTitle;
	protected Widget m_DeadSceenStatsPanel;
	protected TextWidget m_DeadSceenStatsButtonLable;
	protected ButtonWidget m_DeadSceenStatsButton;
	protected Widget m_DeadSceenStatsButtonPanel;
	protected ImageWidget m_DeadScreenImage;
	protected Widget m_DeadScreenRoot;
	protected Widget m_DeadScreen;

	protected ref ExpansionNewsFeed m_NewsFeed;

	void InGameMenu()
	{
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitor;
		if (CF_Modules<ExpansionMonitorModule>.Get(monitor))
			monitor.m_StatsInvoker.Insert(OnDataReceived);
	#endif

		ExpansionSettings.SI_General.Insert(Expansion_OnGeneralSettingsUpdated);
	}

	void ~InGameMenu()
	{
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitor;
		if (CF_Modules<ExpansionMonitorModule>.Get(monitor))
			monitor.m_StatsInvoker.Remove(OnDataReceived);
	#endif

		ExpansionSettings.SI_General.Remove(Expansion_OnGeneralSettingsUpdated);
	}

	override Widget Init()
	{
		layoutRoot = super.Init();

		m_Logo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		//! Load default logo
		m_Logo.LoadImageFile( 0, "set:expansion_iconset image:logo_expansion_white" );

		//! Licensing Panel
		m_Licensing = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("Licensing"));
		m_Licensing.Show(true);

		Widget version = m_Licensing.FindAnyWidget("version");
		version.Show(true);
		Widget copyright = m_Licensing.FindAnyWidget("copyright");
		copyright.Show(true);

		//! Death-Screen
		m_DeadScreenRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_dead_screen.layout", layoutRoot);
		m_DeadScreenRoot.SetAlpha(0);
		m_DeadScreenRoot.Show(false);

		m_DeadScreen = Widget.Cast(m_DeadScreenRoot.FindAnyWidget("dead_screen"));
		m_DeadScreen.SetAlpha(0);
		m_DeadScreenImage = ImageWidget.Cast(m_DeadScreenRoot.FindAnyWidget( "dead_screen_image"));
		m_DeadScreenImage.SetAlpha(0);
		m_DeadScreenImage.LoadImageFile(0, "DayZExpansion/GUI/textures/dead_screens/dead_screen.edds");

		m_DeadSceenStatsButtonPanel = Widget.Cast(m_DeadScreenRoot.FindAnyWidget("ButtonPanel"));
		m_DeadSceenStatsButton = ButtonWidget.Cast(m_DeadScreenRoot.FindAnyWidget("bStats"));
		m_DeadSceenStatsButtonLable = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("bStatsLable"));

		m_DeadSceenStatsPanel	 = Widget.Cast(m_DeadScreenRoot.FindAnyWidget("PlayerStatisticsPanel"));
		m_DeadSceenStatsPanelTitle = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("Caption"));
		m_DeadSourceVal = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("DeadSourceValue"));
		m_LongestShotVal = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("LongRangeShotValue"));
		m_DistanceVal = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("DistanceTraveledValue"));
		m_AnimalsKilledVal	 = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("AnimalsKilledValue"));
		m_InfectedKilledVal = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("InfectedKilledValue"));
		m_PlayersKilledVal = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("PlayersKilledValue"));
		m_TimeSurvivedVal = TextWidget.Cast(m_DeadScreenRoot.FindAnyWidget("TimeSurvivedValue"));
		m_DeadScreenStatsHideButton = ButtonWidget.Cast( m_DeadScreenRoot.FindAnyWidget("bHide"));

		m_DeadScreenFadeInIncrement	= 1 / DEAD_SCREEN_FADEIN_TIME;
		m_DeadScreenImageFadeInIncrement = 1 / DEAD_SCREEN_IMAGE_FADEIN_TIME;

		//! Newsfeed
		m_NewsFeed = new ExpansionNewsFeed();
		layoutRoot.AddChild(m_NewsFeed.GetLayoutRoot(), true);

		MultilineTextWidget modWarning = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("ModdedWarning"));
		float width, height;
		modWarning.GetSize(width, height);
		float x, y;
		modWarning.GetPos(x, y);

		float newY = y + height + 56.0;
		m_NewsFeed.GetLayoutRoot().SetPos(x, newY, true);

		if (GetExpansionSettings().GetGeneral(false).IsLoaded())
			Expansion_OnGeneralSettingsUpdated();

		return layoutRoot;
	}

	void Expansion_OnGeneralSettingsUpdated()
	{
		//! Apply custom logo
		if (GetExpansionSettings().GetGeneral().InGameMenuLogoPath != string.Empty)
		{
			m_Logo.LoadImageFile(0, GetExpansionSettings().GetGeneral().InGameMenuLogoPath);
			m_Logo.SetFlags(m_Logo.GetFlags() | WidgetFlags.STRETCH);
			m_Logo.Show(true);
		}

		if (!GetExpansionSettings().GetGeneral().UseDeathScreenStatistics || !GetValuesFromMonitor())
			m_DeadSceenStatsButtonPanel.Show(false);

		if (!GetExpansionSettings().GetGeneral().UseNewsFeedInGameMenu)
			m_NewsFeed.GetLayoutRoot().Show(false);
	}

	void OnDataReceived( ExpansionSyncedPlayerStats player_stats)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER_MONITOR, this, player_stats.m_PlainID);
	#endif

		if (GetGame().GetPlayer().GetIdentity().Expansion_GetPlainId() != player_stats.m_PlainID || !player_stats.m_HasRegisteredStats)
			return;

		if (GetExpansionSettings().GetGeneral().UseDeathScreenStatistics && GetValuesFromMonitor())
			m_DeadSceenStatsButtonPanel.Show(true);
	}

	bool GetValuesFromMonitor()
	{
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitor;
		if (!CF_Modules<ExpansionMonitorModule>.Get(monitor))
			return false;

		ExpansionSyncedPlayerStats player_stats = monitor.GetClientStats();
		if (!player_stats || !player_stats.m_HasRegisteredStats)
			return false;

		m_Distance = player_stats.m_Distance;
		m_Playtime = player_stats.m_Playtime;
		m_PlayersKilled = player_stats.m_PlayersKilled;
		m_InfectedKilled = player_stats.m_InfectedKilled;
		m_AnimalsKilled = player_stats.m_AnimalsKilled;
		m_LongestShot = player_stats.m_LongestShot;
		return true;
	#endif

		return false;
	}

	protected void UpdatePlayerStatValues()
	{
		if (GetGame().GetPlayer() && GetGame().GetPlayer().GetIdentity())
		{
			string name = GetGame().GetPlayer().GetIdentity().GetName();
			StringLocaliser player_name = new StringLocaliser("STR_EXPANSION_DEADSCREEN_STATS_TITLE", name);
			m_DeadSceenStatsPanelTitle.SetText(player_name.Format());
			m_LongestShotVal.SetText(ExpansionStatic.GetDistanceString(m_LongestShot));
			m_DistanceVal.SetText(ExpansionStatic.GetDistanceString(m_Distance));
			m_AnimalsKilledVal.SetText(ExpansionStatic.GetValueString(m_AnimalsKilled));
			m_InfectedKilledVal.SetText(ExpansionStatic.GetValueString(m_InfectedKilled));
			m_PlayersKilledVal.SetText(ExpansionStatic.GetValueString(m_PlayersKilled));
			m_TimeSurvivedVal.SetText(ExpansionStatic.GetTimeString(m_Playtime));
		}
	}

	protected void DeadScreenShow(float timeslice)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.UI, this, "DeadScreenShow").Add(timeslice);
	#endif

		if (m_DeadScreenImageFadeInLevel == 1)
			return;

		if (GetExpansionSettings().GetGeneral().UseNewsFeedInGameMenu)
			m_NewsFeed.Hide();

		m_DeadScreenRoot.Show(true);
		float new_deadscreen_val = m_DeadScreenFadeInLevel + m_DeadScreenFadeInIncrement * timeslice;
		if (new_deadscreen_val < 1)
		{
			m_DeadScreenFadeInLevel = new_deadscreen_val;
		}
		else
		{
			m_DeadScreenFadeInLevel = 1;
		}

		if (m_DeadScreenFadeInLevel > 0.5)
		{
			float new_logo_val = m_DeadScreenImageFadeInLevel + m_DeadScreenImageFadeInIncrement * timeslice;
			if (new_deadscreen_val < 1)
			{
				m_DeadScreenImageFadeInLevel = new_logo_val;
			}
			else
			{
				m_DeadScreenImageFadeInLevel = 1;
			}
		}

		m_DeadScreenRoot.SetAlpha(m_DeadScreenFadeInLevel);
		m_DeadScreen.SetAlpha(m_DeadScreenFadeInLevel);
		m_DeadScreenImage.SetAlpha(m_DeadScreenImageFadeInLevel);

		Widget version = m_Licensing.FindAnyWidget("version");
		Widget copyright = m_Licensing.FindAnyWidget("copyright");
		if (m_DeadScreenFadeInLevel < 1)
		{
			m_Logo.SetAlpha(-m_DeadScreenFadeInLevel);
			m_Licensing.SetAlpha(-m_DeadScreenFadeInLevel);
			version.SetAlpha(-m_DeadScreenFadeInLevel);
			copyright.SetAlpha(-m_DeadScreenFadeInLevel);
		}
		else if (m_DeadScreenFadeInLevel >= 1)
		{
			m_Logo.Show(false);
			m_Licensing.Show(false);
			version.Show(false);
			copyright.Show(false);
		}
	}

	protected void ShowPlayerStats()
	{
		m_DeadSceenStatsPanel.Show(true);
		m_DeadSceenStatsButtonPanel.Show(false);
	}

	protected void HidePlayerStats()
	{
		m_DeadSceenStatsPanel.Show(false);
		m_DeadSceenStatsButtonPanel.Show(true);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w == m_DeadSceenStatsButton)
		{
			UpdatePlayerStatValues();
			ShowPlayerStats();
		}
		else if (w == m_DeadScreenStatsHideButton)
		{
			HidePlayerStats();
		}

		return false;
	}

	override void Update(float timeslice)
	{
		if (g_ExpansionShowDeadScreen)
		{
			m_TimerSlice += timeslice;
			if (m_TimerSlice >= 0.01)
			{
				DeadScreenShow(timeslice);
				m_TimerSlice = 0;
			}
		}

		super.Update(timeslice);
	}
};
#endif