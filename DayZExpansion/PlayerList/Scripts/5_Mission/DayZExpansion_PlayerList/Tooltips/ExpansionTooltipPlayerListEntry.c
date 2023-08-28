/**
 * ExpansionTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTooltipSectionPlayerListEntryController: ExpansionViewController
{
	string SectionIcon;
	string SectionLabel;
	string SectionText;
}

class ExpansionTooltipSectionPlayerListEntry: ExpansionScriptView
{
	private string m_Icon;
	private StringLocaliser m_Label;
	private StringLocaliser m_Text;
	
	void ExpansionTooltipSectionPlayerListEntry(string icon, StringLocaliser label, StringLocaliser text)
	{
		m_Icon = icon;
		m_Label = label;
		m_Text = text;
		
		ExpansionTooltipSectionPlayerListEntryController controller = ExpansionTooltipSectionPlayerListEntryController.Cast(GetController());
		if (!controller)
			return;
		
		controller.SectionIcon = ExpansionIcons.GetPath(m_Icon);
		controller.NotifyPropertyChanged("SectionIcon");
		
		controller.SectionLabel = m_Label.Format();
		controller.NotifyPropertyChanged("SectionLabel");
		
		controller.SectionText = m_Text.Format();
		controller.NotifyPropertyChanged("SectionText");		
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/PlayerList/GUI/layouts/expansion_tooltip_section.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionTooltipSectionPlayerListEntryController;
	}
}

class ExpansionTooltipPlayerListEntry: ExpansionScriptView
{
	string m_PlayerID;
	string m_PlayerPlainID;
	string m_Title;
	bool m_StatsRequested;
	
	void ExpansionTooltipPlayerListEntry(string title, string playerID, string playerPlainID)
	{
		m_Title = title;
		m_PlayerID = playerID;
		if (m_PlayerID != GetGame().GetPlayer().GetIdentity().GetId())
			m_PlayerPlainID = playerPlainID;
		
		int x, y;
		GetGame().GetMousePos(x, y);
		GetLayoutRoot().SetPos(x, y, true);
		
		ExpansionTooltipPlayerListEntryController controller = ExpansionTooltipPlayerListEntryController.Cast(GetController());
		if (!controller)
			return;
		
		controller.TooltipTitle = m_Title;
		controller.NotifyPropertyChanged("TooltipTitle");
		
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (!monitorModule)
			return;
		
		monitorModule.m_StatsInvoker.Insert(OnTooltipDataRecived);
		OnShow();
	#endif
	}
	
	override void OnShow()
	{
		super.OnShow();

	#ifdef EXPANSIONMONITORMODULE
		if (m_StatsRequested)
			return;

		m_StatsRequested = true;

		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (!monitorModule)
			return;
		
		monitorModule.RequestPlayerStats(m_PlayerPlainID, true, false);
	#endif
	}

	override void OnHide()
	{
		super.OnHide();

		m_StatsRequested = false;
	}

	void ~ExpansionTooltipPlayerListEntry()
	{
	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (!monitorModule)
			return;
		
		monitorModule.m_StatsInvoker.Remove(OnTooltipDataRecived);
	#endif
	}
	
	void OnTooltipDataRecived( ExpansionSyncedPlayerStats player_stats)
	{
		if ((player_stats.m_PlainID && m_PlayerPlainID != player_stats.m_PlainID) || !player_stats.m_HasRegisteredStats) return;
		
		SetStats(player_stats);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/PlayerList/GUI/layouts/expansion_tooltip_base.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionTooltipPlayerListEntryController;
	}
	
	void SetStats( ExpansionSyncedPlayerStats player_stats)
	{
		ExpansionTooltipPlayerListEntryController controller = ExpansionTooltipPlayerListEntryController.Cast(GetController());
		if (!controller)
			return;

		if (controller.Sections && controller.Sections.Count() >0)
			controller.Sections.Clear();
		
		ExpansionTooltipSectionPlayerListEntry entry;
		StringLocaliser text;
		StringLocaliser lable;
		
		lable = new StringLocaliser("STR_EXPANSION_BOOK_STATUS_CHARACTER_STAT_PLAYER_DISTANCE");
		text = new StringLocaliser(ExpansionStatic.GetDistanceString(player_stats.m_Distance));
		entry = new ExpansionTooltipSectionPlayerListEntry("Tent Small", lable, text);
		controller.Sections.Insert(entry);
		
		lable = new StringLocaliser("STR_EXPANSION_BOOK_STATUS_CHARACTER_STAT_PLAYER_PLAYTIME");
		text = new StringLocaliser(ExpansionStatic.GetTimeString(player_stats.m_Playtime));
		entry = new ExpansionTooltipSectionPlayerListEntry("Fireplace", lable, text);
		controller.Sections.Insert(entry);
		
	#ifdef EXPANSIONMONITORMODULE
		lable = new StringLocaliser("STR_EXPANSION_BOOK_STATUS_CHARACTER_STAT_PLAYER_KILLS");
		text = new StringLocaliser(ExpansionStatic.GetValueString(player_stats.m_PlayersKilled) + " Killed");
		entry = new ExpansionTooltipSectionPlayerListEntry("Persona", lable, text);
		controller.Sections.Insert(entry);
		
		lable = new StringLocaliser("STR_EXPANSION_BOOK_STATUS_CHARACTER_STAT_ZOMBIE_KILLS");
		text = new StringLocaliser(ExpansionStatic.GetValueString(player_stats.m_InfectedKilled) + " Killed");
		entry = new ExpansionTooltipSectionPlayerListEntry("Infected 2", lable, text);
		controller.Sections.Insert(entry);
		
		lable = new StringLocaliser("STR_EXPANSION_BOOK_STATUS_CHARACTER_STAT_ANIMAL_KILLS");
		text = new StringLocaliser(ExpansionStatic.GetValueString(player_stats.m_AnimalsKilled) + " Killed");
		entry = new ExpansionTooltipSectionPlayerListEntry("Animal Skull", lable, text);
		controller.Sections.Insert(entry);
	#endif
		
		lable = new StringLocaliser("STR_EXPANSION_BOOK_STATUS_CHARACTER_STAT_PLAYER_SHOT");
		text = new StringLocaliser(ExpansionStatic.GetDistanceString(player_stats.m_LongestShot));
		entry = new ExpansionTooltipSectionPlayerListEntry("Gun", lable, text);
		controller.Sections.Insert(entry);
	}
}

class ExpansionTooltipPlayerListEntryController: ExpansionViewController
{
	string TooltipTitle;
	string TooltipText;
	ref ObservableCollection<ref ExpansionTooltipSectionPlayerListEntry> Sections = new ObservableCollection<ref ExpansionTooltipSectionPlayerListEntry>(this);
}