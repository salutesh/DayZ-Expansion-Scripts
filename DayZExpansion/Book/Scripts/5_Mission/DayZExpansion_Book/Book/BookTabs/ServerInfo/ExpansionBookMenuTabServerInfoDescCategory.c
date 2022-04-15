/**
 * ExpansionBookMenuTabRulesCategoryEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabServerInfoDescCategory: ExpansionScriptView
{
	ref ExpansionBookMenuTabServerInfoDescCategoryController m_EntryController;
	ref ExpansionBookDescriptionCategory m_Category;
	ref ExpansionBookMenuTabServerInfo m_ServerInfoTab;
	
	private ButtonWidget category_entry_button;
	private TextWidget category_entry_label;
	private ImageWidget category_entry_icon;
	private WrapSpacerWidget categories_spacer;
	
	void ExpansionBookMenuTabServerInfoDescCategory(ExpansionBookMenuTabServerInfo tab, ExpansionBookDescriptionCategory category)
	{
		m_ServerInfoTab = tab;
		m_Category = category;
		
		if (!m_EntryController)
			m_EntryController = ExpansionBookMenuTabServerInfoDescCategoryController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/serverinfo/expansion_book_tab_description_category.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabServerInfoDescCategoryController;
	}
	
	void SetView()
	{
		m_EntryController.CategoryName = m_Category.CategoryName;
		m_EntryController.NotifyPropertyChanged("CategoryName");
		
		foreach (ExpansionBookDescription description: m_Category.Descriptions)
		{
			ExpansionBookMenuTabServerInfoDescElement descElement = new ExpansionBookMenuTabServerInfoDescElement(description);
			m_EntryController.Descriptions.Insert(descElement);
		}
	}
	
	void OnEntryButtonClick()
	{
		categories_spacer.Show(!categories_spacer.IsVisible());
		
		/*for ( int i = 0; i < m_EntryController.Descriptions.Count(); i++ )
		{
			m_EntryController.Descriptions[i].GetTextWidget().Update();
		}*/
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case category_entry_button:
			category_entry_icon.SetColor(ARGB(255,220,220,220));
			category_entry_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case category_entry_button:
			category_entry_icon.SetColor(ARGB(255,0,0,0));
			category_entry_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabServerInfoDescCategoryController: ExpansionViewController
{
	string CategoryName;
	ref ObservableCollection<ref ExpansionBookMenuTabServerInfoDescElement> Descriptions = new ObservableCollection<ref ExpansionBookMenuTabServerInfoDescElement>(this);
};