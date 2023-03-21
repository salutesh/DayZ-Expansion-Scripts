/**
 * ExpansionBookMenuTabCraftingIngredient.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabCraftingIngredient: ExpansionScriptView
{
	ref ExpansionBookMenuTabCrafting m_CraftingTab;
	ref ExpansionBookMenuTabCraftingIngredientController m_IngredientController;
	ref ExpansionBookCraftingItem m_Item;
	bool m_FirstIngredient;
	EntityAI m_Object;
	
	ImageWidget ingredient_entry_icon;
	Widget ingredient_frame;
	
	bool m_Selected = false;
	
	void ExpansionBookMenuTabCraftingIngredient(ExpansionBookMenuTabCrafting tab, ExpansionBookCraftingItem item, bool first = false)
	{
		m_CraftingTab = tab;
		m_Item = item;
		m_FirstIngredient = first;
		
		if (!m_IngredientController)
			m_IngredientController = ExpansionBookMenuTabCraftingIngredientController.Cast(GetController());
		
		SetView();
	}
	
	void ~ExpansionBookMenuTabCraftingIngredient()
	{
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/crafting/expansion_book_tab_crafting_ingredient.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabCraftingIngredientController;
	}
		
	void SetView()
	{
		if (m_Item.Amount > 1)
		{
			m_IngredientController.ItemName = m_Item.Amount.ToString() + "x " + m_Item.DisplayName;
		}
		else
		{
			m_IngredientController.ItemName = m_Item.DisplayName;
		}
		m_IngredientController.NotifyPropertyChanged("ItemName");
		
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
		
		for (int i = 0; i < m_Item.ClassNames.Count(); i++)
		{
			m_Object = EntityAI.Cast(GetGame().CreateObjectEx(m_Item.ClassNames[i], vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
			if (m_Object)
				break;
		}
				
		m_IngredientController.ItemPreview = m_Object;
		m_IngredientController.NotifyPropertyChanged("ItemPreview");
	}
	
	void OnIngredientButtonClick()
	{
		if (m_FirstIngredient)
		{
			m_CraftingTab.SetFirstIngredient(m_Item, this);
		}
		else
		{
			m_CraftingTab.SetSecondIngredient(m_Item, this);
		}
	}
	
	void SetSelected()
	{
		if (ingredient_frame && !ingredient_frame.IsVisible())
			ingredient_frame.Show(true);
		
		m_Selected = true;
	}
	
	void SetUnselected()
	{
		if (ingredient_frame && ingredient_frame.IsVisible())
			ingredient_frame.Show(false);
		
		m_Selected = false;
	}
	
	bool IsSelected()
	{
		return m_Selected;
	}
};
class ExpansionBookMenuTabCraftingIngredientController: ExpansionViewController
{
	string ItemName;
	Object ItemPreview;
};