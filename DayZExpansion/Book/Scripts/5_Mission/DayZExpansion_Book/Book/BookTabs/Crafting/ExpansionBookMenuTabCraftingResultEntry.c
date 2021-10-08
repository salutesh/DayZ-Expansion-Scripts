/**
 * ExpansionBookMenuTabCraftingResultEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabCraftingResultEntry: ExpansionScriptView
{
	ref ExpansionBookMenuTabCraftingResult m_ResultElement;
	ref ExpansionBookMenuTabCraftingResultEntryController m_ResultEntryController;
	ref ExpansionBookCraftingItem m_Item;
	ref ExpansionBookCraftingRecipe m_Recipe;
	
	ButtonWidget result_entry_button;
	TextWidget result_entry_label;
	
	void ExpansionBookMenuTabCraftingResultEntry(ExpansionBookMenuTabCraftingResult result, ExpansionBookCraftingItem item, ExpansionBookCraftingRecipe recipe)
	{		
		m_ResultElement = result;
		m_Item = item;
		m_Recipe = recipe;
		
		if (!m_ResultEntryController)
			m_ResultEntryController = ExpansionBookMenuTabCraftingResultEntryController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/crafting/expansion_book_tab_crafting_result_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabCraftingResultEntryController;
	}
	
	void SetView()
	{
		if (m_Item.Amount > 0)
		{
			m_ResultEntryController.ItemName = m_Item.Amount.ToString() + "x " + m_Item.DisplayName;
		}
		else
		{
			m_ResultEntryController.ItemName = m_Item.DisplayName;
		}
		m_ResultEntryController.NotifyPropertyChanged("ItemName");
	}
	
	void OnResultEntryButtonClick()
	{
		m_ResultElement.m_CraftingTab.GetCraftingTabController().Ingredients1.Clear();
		for (int i = 0; i < m_ResultElement.m_Recipe.Ingredients[0].Count(); i++)
		{
			ExpansionBookCraftingItem ingredients1 = m_ResultElement.m_Recipe.Ingredients[0][i];
			ExpansionBookMenuTabCraftingIngredient ingred1;
			if (i == 0)
			{
				ingred1 = new ExpansionBookMenuTabCraftingIngredient(m_ResultElement.m_CraftingTab, ingredients1, true);
			}
			else
			{
				ingred1 = new ExpansionBookMenuTabCraftingIngredient(m_ResultElement.m_CraftingTab, ingredients1);
			}
			m_ResultElement.m_CraftingTab.GetCraftingTabController().Ingredients1.Insert(ingred1);
		}
		m_ResultElement.m_CraftingTab.GetCraftingTabController().NotifyPropertyChanged("Ingredients1");
		
		m_ResultElement.m_CraftingTab.GetCraftingTabController().Ingredients2.Clear();
		for (int j = 0; j < m_ResultElement.m_Recipe.Ingredients[1].Count(); j++)
		{
			ExpansionBookCraftingItem ingredients2 = m_ResultElement.m_Recipe.Ingredients[1][j];
			ExpansionBookMenuTabCraftingIngredient ingred2 = new ExpansionBookMenuTabCraftingIngredient(m_ResultElement.m_CraftingTab, ingredients2);
			m_ResultElement.m_CraftingTab.GetCraftingTabController().Ingredients2.Insert(ingred2);
		}
		m_ResultElement.m_CraftingTab.GetCraftingTabController().NotifyPropertyChanged("Ingredients2");
		
		m_ResultElement.m_CraftingTab.SetResultsView(m_Item, m_Recipe);
		
		m_ResultElement.m_CraftingTab.PlayDrawSound();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case result_entry_button:
			result_entry_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case result_entry_button:
			result_entry_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}

class ExpansionBookMenuTabCraftingResultEntryController: ExpansionViewController
{
	string ItemName;
};