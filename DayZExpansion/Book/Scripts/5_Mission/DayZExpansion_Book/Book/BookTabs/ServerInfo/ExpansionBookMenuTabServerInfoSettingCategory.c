/**
 * ExpansionBookMenuTabServerInfoSettingCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabServerInfoSettingCategory: ExpansionScriptView
{
	ref ExpansionBookMenuTabServerInfo m_ServerInfoTab;
	ref ExpansionBookMenuTabServerInfoSettingCategoryController m_SettingCategoryController;
	
	private ImageWidget setting_entry_icon;
	private ButtonWidget setting_entry_button;
	private TextWidget setting_entry_label;
	
	string m_Setting;
	
	WrapSpacerWidget settings_content;
	
	void ExpansionBookMenuTabServerInfoSettingCategory(ExpansionBookMenuTabServerInfo tab, string setting)
	{
		m_ServerInfoTab = tab;
		m_Setting = setting;
		
		if (!m_SettingCategoryController)
			m_SettingCategoryController = ExpansionBookMenuTabServerInfoSettingCategoryController.Cast(GetController());
		
		SetView();
	}
	
	void SetView()
	{
		m_SettingCategoryController.SettingName = m_Setting;
		m_SettingCategoryController.NotifyPropertyChanged("SettingName");
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/serverinfo/expansion_book_tab_serverinfo_setting_category.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabServerInfoSettingCategoryController;
	}
	
	void OnEntryButtonClick()
	{
		if (!settings_content.IsVisible())
		{
			settings_content.Show(true);
		}
		else
		{
			settings_content.Show(false);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case setting_entry_button:
			setting_entry_icon.SetColor(ARGB(255,220,220,220));
			setting_entry_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case setting_entry_button:
			setting_entry_icon.SetColor(ARGB(255,0,0,0));
			setting_entry_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	void AddSetting(ExpansionBookMenuTabServerInfoSetting setting)
	{
		m_SettingCategoryController.Settings.Insert(setting);
	}
};

class ExpansionBookMenuTabServerInfoSettingCategoryController: ExpansionViewController
{
	string SettingName;
	string SettingBackground;
	
	ref ObservableCollection<ref ExpansionBookMenuTabServerInfoSetting> Settings = new ObservableCollection<ref ExpansionBookMenuTabServerInfoSetting>(this);
};