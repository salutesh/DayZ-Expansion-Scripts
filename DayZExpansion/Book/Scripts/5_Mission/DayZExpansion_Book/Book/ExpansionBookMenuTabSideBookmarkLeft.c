/**
 * ExpansionBookMenuTabBookmark.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabSideBookmarkLeft: ExpansionScriptView
{
	ButtonWidget bookmark_button;
	ImageWidget bookmark_icon;
	
	string m_Name;
	string m_URL;
	string m_IconName;
	int m_IconColor;
	
	ref ExpansionBookMenuTabSideBookmarkLeftController m_BookmarkController;
	
	void ExpansionBookMenuTabSideBookmarkLeft(string name, string url, string iconName, int iconColor)
	{
		if (!m_BookmarkController)
			m_BookmarkController = ExpansionBookMenuTabSideBookmarkLeftController.Cast(GetController());
		
		m_Name = name;
		m_URL = url;
		m_IconName = iconName;
		m_IconColor = iconColor;
		
		SetIcon(iconName, iconColor);
		SetBackground();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/expansion_book_tab_bookmark_side_left.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabSideBookmarkLeftController;
	}
	
	void SetIcon(string icon_name, int color)
	{
		m_BookmarkController.BookmarkIcon = ExpansionIcons.GetPath(icon_name);
		m_BookmarkController.NotifyPropertyChanged("BookmarkIcon");
		
		bookmark_icon.SetColor(color);
	}
	
	void SetBackground()
	{
		m_BookmarkController.Background = GetRandomElementBackground();
		m_BookmarkController.NotifyPropertyChanged("Background");
	}
	
	string GetRandomElementBackground()
	{
		TStringArray backgrounds = new TStringArray;
		
		backgrounds.Insert("set:book_assets image:bookmark_element_1");
		backgrounds.Insert("set:book_assets image:bookmark_element_2");
		backgrounds.Insert("set:book_assets image:bookmark_element_3");
		
		return backgrounds.GetRandomElement();
	}
	
	void OnBookmarkButtonClick()
	{
		GetGame().OpenURL(m_URL);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case bookmark_button:
			bookmark_icon.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case bookmark_button:
			bookmark_icon.SetColor(m_IconColor);
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabSideBookmarkLeftController: ExpansionViewController
{
	string BookmarkIcon;
	string Background;
};