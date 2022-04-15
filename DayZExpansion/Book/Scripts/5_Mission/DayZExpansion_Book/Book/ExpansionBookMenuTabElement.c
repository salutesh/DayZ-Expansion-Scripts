/**
 * ExpansionBookMenuTabElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabElement: ExpansionScriptView
{
	ref ExpansionBookMenuTabBase m_Tab;
	ref ExpansionBookMenuTabElementController m_ElementController;
	
	ImageWidget book_element_icon;
	TextWidget book_element_label;
	ButtonWidget book_element_button;
	
	void ExpansionBookMenuTabElement(ExpansionBookMenuTabBase book_tab)
	{
		m_Tab = book_tab;
		
		if (!m_ElementController)
			m_ElementController = ExpansionBookMenuTabElementController.Cast(GetController());
		
		SetIcon(m_Tab.GetTabIconName());
		SetName(m_Tab.GetTabName());
		
		m_ElementController.Background = "set:book_assets image:menu_element_3";
		m_ElementController.NotifyPropertyChanged("Background");
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Book/GUI/layouts/expansion_book_tab_element.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabElementController;
	}
	
	void SetIcon(string icon_name)
	{
		m_ElementController.ElementIcon = ExpansionIcons.GetPath(icon_name);
		m_ElementController.NotifyPropertyChanged("ElementIcon");
		
		book_element_icon.SetColor(m_Tab.GetTabColor());
	}
	
	void SetName(string name)
	{
		m_ElementController.ElementName = name;
		m_ElementController.NotifyPropertyChanged("ElementName");
		
		book_element_label.SetColor(m_Tab.GetTabColor());
	}
		
	void OnElementButtonClick()
	{
		m_Tab.Show();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case book_element_button:
			book_element_icon.SetColor(ARGB(255,220,220,220));
			book_element_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case book_element_button:
			book_element_icon.SetColor(m_Tab.GetTabColor());
			book_element_label.SetColor(m_Tab.GetTabColor());
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool CanShow()
	{
		return m_Tab.CanShow();
	}
};

class ExpansionBookMenuTabElementController: ExpansionViewController
{
	string ElementIcon;
	string ElementName;
	string Background;
};