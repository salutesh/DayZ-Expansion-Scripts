/**
 * ExpansionBookMenuTabCraftingCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabCraftingCategory: ExpansionScriptView
{
	ref ExpansionBookMenuTabCraftingCategoryController m_CatgoryController;
	ref ExpansionBookMenuTabCrafting m_CraftingTab;
	ref ExpansionBookCraftingCategoryRecipes m_CraftingCategory;

	private ButtonWidget category_entry_button;
	private TextWidget category_entry_label;
	private ImageWidget category_entry_icon;
	private WrapSpacerWidget categories_spacer;

	void ExpansionBookMenuTabCraftingCategory(ExpansionBookMenuTabCrafting tab, ExpansionBookCraftingCategoryRecipes cat)
	{
		m_CraftingTab = tab;
		m_CraftingCategory = cat;
		
		if (!m_CatgoryController)
			m_CatgoryController = ExpansionBookMenuTabCraftingCategoryController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/crafting/expansion_book_tab_crafting_category.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabCraftingCategoryController;
	}
	
	void SetView()
	{
		m_CatgoryController.CategoryName = m_CraftingCategory.CategoryName;
		m_CatgoryController.NotifyPropertyChanged("CategoryName");
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
	
	ExpansionBookMenuTabCraftingCategoryController GetCraftingCategoryController()
	{
		return m_CatgoryController;
	}
	
	void OnEntryButtonClick()
	{
		if (categories_spacer.IsVisible())
		{
			categories_spacer.Show(false);
		}
		else	
		{
			categories_spacer.Show(true);
		}
		
		m_CraftingTab.PlayDrawSound();
	}
};

class ExpansionBookMenuTabCraftingCategoryController: ExpansionViewController
{
	string CategoryName;
	ref ObservableCollection<ref ExpansionBookMenuTabCraftingResult> Results = new ObservableCollection<ref ExpansionBookMenuTabCraftingResult>(this);
};