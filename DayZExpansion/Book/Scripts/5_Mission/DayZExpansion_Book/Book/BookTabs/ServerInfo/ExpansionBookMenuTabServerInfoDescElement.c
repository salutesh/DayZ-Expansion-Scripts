/**
 * ExpansionBookMenuTabServerInfoDescElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabServerInfoDescElement: ExpansionScriptView
{
	ref ExpansionBookMenuTabServerInfoDescElementController m_EntryController;
	ref ExpansionBookDescription m_Desc;
	
	RichTextWidget text;
	
	void ExpansionBookMenuTabServerInfoDescElement(ExpansionBookDescription desc)
	{
		m_Desc = desc;
		
		if (!m_EntryController)
			m_EntryController = ExpansionBookMenuTabServerInfoDescElementController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/serverinfo/expansion_book_tab_description_element.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabServerInfoDescElementController;
	}
	
	void SetView()
	{		
		m_EntryController.Text = m_Desc.DescriptionText;
		m_EntryController.NotifyPropertyChanged("Text");
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	RichTextWidget GetTextWidget()
	{
		return text;
	}
};

class ExpansionBookMenuTabServerInfoDescElementController: ExpansionViewController
{
	string Text;
};