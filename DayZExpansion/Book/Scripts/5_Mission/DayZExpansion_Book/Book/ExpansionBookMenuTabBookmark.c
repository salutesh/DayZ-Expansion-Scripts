/**
 * ExpansionBookMenuTabBookmark.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabBookmark: ExpansionScriptView
{
	ButtonWidget bookmark_button;
	ImageWidget bookmark_icon;
	
	string m_IconName;
	ref ExpansionBookMenuTabBase m_Tab;
	ref ExpansionBookMenuTabBookmarkController m_BookmarkController;
	
	void ExpansionBookMenuTabBookmark(ExpansionBookMenuTabBase book_tab)
	{
		m_Tab = book_tab;
		
		if (!m_BookmarkController)
			m_BookmarkController = ExpansionBookMenuTabBookmarkController.Cast(GetController());
		
		SetIcon(m_Tab.GetTabIconName());
		
		SetBackground();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/expansion_book_tab_bookmark.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabBookmarkController;
	}
	
	void SetIcon(string icon_name)
	{
		m_BookmarkController.BookmarkIcon = ExpansionIcons.GetPath(icon_name);
		m_BookmarkController.NotifyPropertyChanged("BookmarkIcon");
		
		bookmark_icon.SetColor(m_Tab.GetTabColor());
	}
		
	void SetBackground()
	{
		m_BookmarkController.Background = "set:book_assets image:bookmark_element_3";
		m_BookmarkController.NotifyPropertyChanged("Background");
	}
	
	/*string GetRandomElementBackground()
	{
		TStringArray backgrounds = new TStringArray;
		
		backgrounds.Insert("set:book_assets image:bookmark_element_1");
		backgrounds.Insert("set:book_assets image:bookmark_element_2");
		backgrounds.Insert("set:book_assets image:bookmark_element_3");
		
		return backgrounds.GetRandomElement();
	}*/
	
	void OnBookmarkButtonClick()
	{
		m_Tab.Show();
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
			bookmark_icon.SetColor(m_Tab.GetTabColor());
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool CanShow()
	{
		return m_Tab.CanShow();
	}
};

class ExpansionBookMenuTabBookmarkController: ExpansionViewController
{
	string BookmarkIcon;
	string Background;
};