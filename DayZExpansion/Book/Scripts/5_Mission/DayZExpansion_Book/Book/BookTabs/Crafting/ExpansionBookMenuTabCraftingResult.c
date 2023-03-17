/**
 * ExpansionBookMenuTabCraftingResult.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


//! Target of this class is to display all the result items from the current recipe we are displaying
class ExpansionBookMenuTabCraftingResult: ExpansionScriptView
{	
	ref ExpansionBookMenuTabCrafting m_CraftingTab;
	ref ExpansionBookCraftingRecipe m_Recipe;
	ref ExpansionBookMenuTabCraftingResultController m_ResultController;
	ref ExpansionBookCraftingItem m_MainResult;
	bool m_RecipeChecked;
	
	ButtonWidget result_entry_button;
	TextWidget result_entry_label;
	ButtonWidget result_collapse_button;
	GridSpacerWidget results_grid;
	ImageWidget result_expand_icon;
	ImageWidget result_colapse_icon;
	
	void ExpansionBookMenuTabCraftingResult(ExpansionBookMenuTabCrafting tab, ExpansionBookCraftingRecipe recipe)
	{
		m_CraftingTab = tab;
		m_Recipe = recipe;
		
		m_MainResult = m_Recipe.Results[m_Recipe.m_TempMainIndex];
		
		if (!m_ResultController)
			m_ResultController = ExpansionBookMenuTabCraftingResultController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/crafting/expansion_book_tab_crafting_result.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabCraftingResultController;
	}
	
	void SetView()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BOOK, this, "SetView");
#endif

		result_collapse_button.Show(true);

		m_ResultController.ResultName = m_Recipe.GetDisplayName();
		
		m_ResultController.NotifyPropertyChanged("ResultName");
		
		//! Add all results to the results spacer under the recipe name
		for (int i = 0; i < m_Recipe.Results.Count(); i++)
		{
			ExpansionBookCraftingItem currentItem = m_Recipe.Results[i];
			ExpansionBookMenuTabCraftingResultEntry result_entry = new ExpansionBookMenuTabCraftingResultEntry(this, currentItem, m_Recipe);
			m_ResultController.Results.Insert(result_entry);
		}
		
		//m_ResultController.NotifyPropertyChanged("Results");
	}
	
	void OnResultButtonClick()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BOOK, this, "OnResultButtonClick");
#endif

		CheckRecipe();

		m_CraftingTab.GetCraftingTabController().Ingredients1.Clear();
		for (int i = 0; i < m_Recipe.Ingredients[0].Count(); i++)
		{
			ExpansionBookCraftingItem ingredients1 = m_Recipe.Ingredients[0][i];
			ExpansionBookMenuTabCraftingIngredient ingred1;
			if (i == 0)
			{
				ingred1 = new ExpansionBookMenuTabCraftingIngredient(m_CraftingTab, ingredients1, true);
			}
			else
			{
				ingred1 = new ExpansionBookMenuTabCraftingIngredient(m_CraftingTab, ingredients1);
			}
			m_CraftingTab.GetCraftingTabController().Ingredients1.Insert(ingred1);
		}
		m_CraftingTab.GetCraftingTabController().NotifyPropertyChanged("Ingredients1");
		
		m_CraftingTab.GetCraftingTabController().Ingredients2.Clear();
		for (int j = 0; j < m_Recipe.Ingredients[1].Count(); j++)
		{
			ExpansionBookCraftingItem ingredients2 = m_Recipe.Ingredients[1][j];
			ExpansionBookMenuTabCraftingIngredient ingred2 = new ExpansionBookMenuTabCraftingIngredient(m_CraftingTab, ingredients2);
			m_CraftingTab.GetCraftingTabController().Ingredients2.Insert(ingred2);
		}
		m_CraftingTab.GetCraftingTabController().NotifyPropertyChanged("Ingredients2");
		
		m_CraftingTab.SetResultsView(m_MainResult, m_Recipe);
		
		m_CraftingTab.PlayDrawSound();
	}
	
	void OnCollapseButtonClick()
	{
		if (!results_grid.IsVisible())
		{
			result_expand_icon.Show(false);
			result_colapse_icon.Show(true);
			results_grid.Show(true);
		}
		else
		{
			result_expand_icon.Show(true);
			result_colapse_icon.Show(false);
			results_grid.Show(false);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case result_collapse_button:
			result_expand_icon.SetColor(ARGB(255,220,220,220));
			result_colapse_icon.SetColor(ARGB(255,220,220,220));
			break;
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
		case result_collapse_button:
			result_expand_icon.SetColor(ARGB(255,0,0,0));
			result_colapse_icon.SetColor(ARGB(255,0,0,0));
			break;
		case result_entry_button:
			result_entry_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}

	void CheckRecipe()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BOOK, this, "CheckRecipe");
#endif

		if (m_RecipeChecked)
			return;

		CF_Log.Debug(ToString() + "::CheckRecipe");

		m_RecipeChecked = true;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());

		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		//! Initializes CanUseInRecipe() result for each ingredient. After calling this,
		//! you can use ExpansionBookCraftingItem.CanUseInRecipe() to check whether the player
		//! can use the item in the recipe.
		m_Recipe.CanDo(player, items);
	}
}

class ExpansionBookMenuTabCraftingResultController: ExpansionViewController
{
	string ResultName;
	
	ref ObservableCollection<ref ExpansionBookMenuTabCraftingResultEntry> Results = new ObservableCollection<ref ExpansionBookMenuTabCraftingResultEntry>(this);
};
