/**
 * ExpansionBookMenuTabServerInfo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabServerInfo: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabServerInfoController m_ServerInfoTabController;
	ref ExpansionTooltipServerSettingEntry m_Tooltip;
		
	void ExpansionBookMenuTabServerInfo(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_ServerInfoTabController)
			m_ServerInfoTabController = ExpansionBookMenuTabServerInfoController.Cast(GetController());
		
		SetView();
	}
		
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/serverinfo/expansion_book_tab_serverinfo.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabServerInfoController;
	}
	
	override string GetTabIconName()
	{
		return "Book 2";
	}
	
	override string GetTabName()
	{
		return "#STR_EXPANSION_BOOK_TAB_SERVER_INFORMATION";
	}
	
	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}
	
	override bool CanShow()
	{
		return GetExpansionSettings().GetBook().EnableServerInfoTab;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		AddLinks();
		AddDescriptions();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		RemoveLinks();
		RemoveDescriptions();
	}
	
	override bool IsParentTab()
	{
		return true;
	}
	
	void SetView()
	{
		if (!GetExpansionClientSettings().StreamerMode) 
			SetServerInfo();
		
		if (GetExpansionSettings().GetBook().DisplayServerSettingsInServerInfoTab)
			AddSettings();
	}
	
	void SetServerInfo()
	{
		//! Get and set basic server information
		MenuData menu_data = GetGame().GetMenuData();
		GetServersResultRow info = OnlineServices.GetCurrentServerInfo();
		if (info)
		{
			m_ServerInfoTabController.ServerName = info.m_Name;
			m_ServerInfoTabController.NotifyPropertyChanged("ServerName");
			
			m_ServerInfoTabController.ServerAdress = info.m_HostIp + ":" + info.m_HostPort;
			m_ServerInfoTabController.NotifyPropertyChanged("ServerAdress");
		}
		else if (menu_data && menu_data.GetLastPlayedCharacter() != GameConstants.DEFAULT_CHARACTER_MENU_ID)
		{
			int char_id = menu_data.GetLastPlayedCharacter();
			string name;
			string ip;
			int port;
			
			menu_data.GetLastServerName(char_id, name);
			menu_data.GetLastServerAddress(char_id, ip);
			port = menu_data.GetLastServerPort(char_id);
			
			m_ServerInfoTabController.ServerName = name;
			m_ServerInfoTabController.NotifyPropertyChanged("ServerName");
			
			m_ServerInfoTabController.ServerAdress = ip + ":" + port.ToString();
			m_ServerInfoTabController.NotifyPropertyChanged("ServerAdress");
		}
	}
	
	void AddSettings()
	{
		for (int i = 0; i < GetExpansionSettings().GetBook().SettingCategories.Count(); i++)
		{
			ExpansionBookSettingCategory category = GetExpansionSettings().GetBook().SettingCategories[i];
			ExpansionBookMenuTabServerInfoSettingCategory settingCategory = new ExpansionBookMenuTabServerInfoSettingCategory(this, category.CategoryName);
			foreach (ExpansionBookSetting setting: category.Settings)
			{
				ExpansionBookMenuTabServerInfoSetting categorySetting = new ExpansionBookMenuTabServerInfoSetting(setting.SettingTitle, setting.SettingText, setting.SettingValue);
				settingCategory.AddSetting(categorySetting);
			}
			
			m_ServerInfoTabController.SettingsCategories.Insert(settingCategory);
		}
	}
	
	void AddLinks()
	{
		ObservableCollection<ref ExpansionScriptView> bookmarksSideSpacerLeft = m_BookMenu.GetBookmarksSideSpacerLeft();
		
		Print(bookmarksSideSpacerLeft);
		if (bookmarksSideSpacerLeft)
		{
			foreach (ExpansionBookLink link : GetExpansionSettings().GetBook().Links)
			{
				ExpansionBookMenuTabSideBookmarkLeft linkButton = ExpansionBookMenuTabSideBookmarkLeft(link.Name, link.URL, link.IconName, link.IconColor);
				bookmarksSideSpacerLeft.Insert(linkButton);
			}
		}
	}
	
	void RemoveLinks()
	{
		ObservableCollection<ref ExpansionScriptView> bookmarksSideSpacerLeft = m_BookMenu.GetBookmarksSideSpacerLeft();
		if (bookmarksSideSpacerLeft)
		{
			if (bookmarksSideSpacerLeft.Count() > 0)
				bookmarksSideSpacerLeft.Clear();
		}
	}
	
	void AddDescriptions()
	{
		Print(GetExpansionSettings().GetBook().Descriptions.Count());
		
		foreach (ExpansionBookDescriptionCategory category : GetExpansionSettings().GetBook().Descriptions)
		{
			ExpansionBookMenuTabServerInfoDescCategory descCategory = new ExpansionBookMenuTabServerInfoDescCategory(this, category);
			m_ServerInfoTabController.Descriptions.Insert(descCategory);
		}
	}
	
	void RemoveDescriptions()
	{
		if (m_ServerInfoTabController && m_ServerInfoTabController.Descriptions.Count() > 0)
			m_ServerInfoTabController.Descriptions.Clear();
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void OnBackButtonClick()
	{
		super.OnBackButtonClick();
	}
	
	override bool IgnoreBackButtonBase()
	{
		return false;
	}
};

class ExpansionBookMenuTabServerInfoController: ExpansionViewController 
{
	string ServerName;
	string ServerAdress;
	string ServerSocial;
	string ServerDescription;
	ref ObservableCollection<ref ExpansionBookMenuTabServerInfoSettingCategory> SettingsCategories = new ObservableCollection<ref ExpansionBookMenuTabServerInfoSettingCategory>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabServerInfoDescCategory> Descriptions = new ObservableCollection<ref ExpansionBookMenuTabServerInfoDescCategory>(this);
};