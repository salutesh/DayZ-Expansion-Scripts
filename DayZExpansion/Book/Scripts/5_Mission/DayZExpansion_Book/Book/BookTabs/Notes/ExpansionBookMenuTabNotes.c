/**
 * ExpansionBookMenuTabNotes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabNotes: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabNotesController m_NotesTabController;
	
	void ExpansionBookMenuTabNotes(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_NotesTabController)
			m_NotesTabController = ExpansionBookMenuTabNotesController.Cast(GetController());
		
		SetView();
	}
		
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/notes/expansion_book_tab_notes.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabNotesController;
	}
	
	override string GetTabIconName()
	{
		return "Book 2";
	}
	
	override string GetTabName()
	{
		return "Personal Notes";
	}
	
	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}
	
	override bool CanShow()
	{
		return true;
	}
	
	override void OnShow()
	{
		super.OnShow();
	}
	
	override void OnHide()
	{
		super.OnHide();
	}
	
	override bool IsParentTab()
	{
		return true;
	}
	
	void SetView()
	{
		
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

class ExpansionBookMenuTabNotesController: ExpansionViewController 
{

};