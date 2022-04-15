/**
 * ExpansionBookMenuTabCrafting.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabCrafting: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabCraftingController m_CraftingTabController;
	
	EntityAI m_Object;
	private ScrollWidget results_list_scroller1;
	private ScrollWidget results_list_scroller2;
	private Widget book_menu_content_left;
	
	ref ExpansionBookCraftingItem m_IngredientOne;
	ref ExpansionBookMenuTabCraftingIngredient m_IngredientElementOne;
	
	ref ExpansionBookCraftingItem m_IngredientTwo;
	ref ExpansionBookMenuTabCraftingIngredient m_IngredientElementTwo;
	
	Widget results_button_panel;
	ButtonWidget prepare_button;
	TextWidget prepare_button_label;
	
	ref ExpansionBookCraftingRecipe m_SelectedRecipe;
	
	void ExpansionBookMenuTabCrafting(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_CraftingTabController)
			m_CraftingTabController = ExpansionBookMenuTabCraftingController.Cast(GetController());
	}
		
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/crafting/expansion_book_tab_crafting.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabCraftingController;
	}
	
	override string GetTabIconName()
	{
		return "Scrap Metal";
	}
	
	override string GetTabName()
	{
		return "#STR_EXPANSION_BOOK_TAB_CRAFTING_RECIPES";
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

		SetView();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
		
		book_menu_content_left.Show(false);
	}
	
	override bool IsParentTab()
	{
		return true;
	}
	
	void SetView()
	{
		LoadCraftingCategories();
	}
	
	void LoadCraftingCategories()
	{
		if (!m_CraftingTabController)
			return;
		
		ClearCraftingCategories();
		
		m_CraftingTabController.LoadRecipes();
		
		TStringArray catNames = m_CraftingTabController.CraftingCategoriesRecipes.GetKeyArray();
		catNames.Sort();
		
		foreach (string catName : catNames)
		{
			ExpansionBookCraftingCategoryRecipes cat = m_CraftingTabController.CraftingCategoriesRecipes.Get(catName);

			if (!cat.Recipes || cat.Recipes.Count() == 0)  //! Skip category if empty
				continue;

			cat.SortRecipes(false);

			ExpansionBookMenuTabCraftingCategory catEntry = new ExpansionBookMenuTabCraftingCategory(this, cat);
			m_CraftingTabController.CraftingCategoriesTabs.Insert(catEntry);

			CF_Log.Debug("Crafting category: " + cat.CategoryName);
			foreach (ExpansionBookCraftingRecipe recipe : cat.Recipes)
			{
				//! Here we should add the recipe to the view.
				CF_Log.Debug("  Recipe name: " + recipe.GetDisplayName());
				CF_Log.Debug("    Recipe main result: " + recipe.Results[recipe.m_TempMainIndex].Format());
				CF_Log.Debug("    Recipe result(s): " + recipe.FormatItems(recipe.Results));
				CF_Log.Debug("    Ingredient(s) 1: " + recipe.FormatItems(recipe.Ingredients[0]));
				CF_Log.Debug("    Ingredient(s) 2: " + recipe.FormatItems(recipe.Ingredients[1]));
				
				ExpansionBookMenuTabCraftingResult entry = new ExpansionBookMenuTabCraftingResult(this, recipe);
				catEntry.GetCraftingCategoryController().Results.Insert(entry);

				if (recipe.CategoriesCount > 1 && recipe.Results.Count() > 1)
				{
					//! If the same recipe appears under a different category as well, show different main item.
					//! Also affects recipe display name, example Prepare Pumpkin / Cut Out Pumkin Seeds
					if (recipe.m_TempMainIndex < recipe.Results.Count() - 1)
						recipe.m_TempMainIndex++;
					else
						recipe.m_TempMainIndex = 0;
				}
			}
			
			//catEntry.GetCraftingCategoryController().NotifyPropertyChanged("Results");
		}
		
		//m_CraftingTabController.NotifyPropertyChanged("CraftingCategoriesTabs");
	}
	
	void ClearCraftingCategories()
	{
		for (int i = 0; i < m_CraftingTabController.CraftingCategoriesTabs.Count(); i++)
		{
			ExpansionBookMenuTabCraftingCategory entry = m_CraftingTabController.CraftingCategoriesTabs[i];
			entry.GetCraftingCategoryController().Results.Clear();
			entry.Hide();
			entry = NULL;
		}
		
		m_CraftingTabController.CraftingCategoriesTabs.Clear();
	}
	
	void SetResultsView(ExpansionBookCraftingItem item, ExpansionBookCraftingRecipe recipe)
	{
		if (item.Amount > 1)
			m_CraftingTabController.ItemName = item.Amount.ToString() + "x " + item.DisplayName;
		else
			m_CraftingTabController.ItemName = item.DisplayName;
		m_CraftingTabController.NotifyPropertyChanged("ItemName");
	
		m_SelectedRecipe = recipe;
			
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
		
		for (int i = 0; i < item.ClassNames.Count(); i++)
		{
			m_Object = EntityAI.Cast(GetGame().CreateObjectEx(item.ClassNames[i], vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
			if (m_Object)
				break;
		}
				
		m_CraftingTabController.ItemPreview = m_Object;
		m_CraftingTabController.NotifyPropertyChanged("ItemPreview");
		
		book_menu_content_left.Show(true);
		
		if (m_IngredientElementOne)
		{
			if (m_IngredientElementOne.IsSelected())
				m_IngredientElementOne.SetUnselected();
			
			m_IngredientElementOne = NULL;
		}
		
		if (m_IngredientOne)
			m_IngredientOne = NULL;
		
		if (m_IngredientElementTwo)
		{
			if (m_IngredientElementTwo.IsSelected())
				m_IngredientElementTwo.SetUnselected();
			
			m_IngredientElementTwo = NULL;
		}
		
		if (m_IngredientTwo)
			m_IngredientTwo = NULL;
	}
	
	void SetFirstIngredient(ExpansionBookCraftingItem item, ExpansionBookMenuTabCraftingIngredient ingredientElement)
	{
		if (!m_IngredientElementOne)
		{
			m_IngredientOne = item;
			m_IngredientElementOne = ingredientElement;
			m_IngredientElementOne.SetSelected();
		} 
		else if (m_IngredientElementOne && m_IngredientElementOne != ingredientElement)
		{
			if (m_IngredientElementOne.IsSelected())
				m_IngredientElementOne.SetUnselected();
		
			m_IngredientOne = item;
			m_IngredientElementOne = ingredientElement;
			m_IngredientElementOne.SetSelected();
		}
		else if (m_IngredientElementOne && m_IngredientElementOne == ingredientElement)
		{
			m_IngredientElementOne.SetUnselected();
			m_IngredientOne = NULL;
			m_IngredientElementOne = NULL;
		}
		
		
		PlayDrawSound();
	}
	
	void SetSecondIngredient(ExpansionBookCraftingItem item, ExpansionBookMenuTabCraftingIngredient ingredientElement)
	{
		if (!m_IngredientElementTwo)
		{
			m_IngredientTwo = item;
			m_IngredientElementTwo = ingredientElement;
			m_IngredientElementTwo.SetSelected();
		} 
		else if (m_IngredientElementTwo && m_IngredientElementTwo != ingredientElement)
		{
			if (m_IngredientElementTwo.IsSelected())
				m_IngredientElementTwo.SetUnselected();
			
			m_IngredientTwo = item;
			m_IngredientElementTwo = ingredientElement;
			m_IngredientElementTwo.SetSelected();
		}
		else if (m_IngredientElementTwo && m_IngredientElementTwo == ingredientElement)
		{
			m_IngredientElementTwo.SetUnselected();
			m_IngredientTwo = NULL;
			m_IngredientElementTwo = NULL;
		}
		
		PlayDrawSound();
	}
	
	override void OnBackButtonClick()
	{
		super.OnBackButtonClick();
	}
	
	override bool IgnoreBackButtonBase()
	{
		return false;
	}
	
	ref ExpansionBookMenuTabCraftingController GetCraftingTabController()
	{
		return m_CraftingTabController;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case prepare_button:
			prepare_button_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case prepare_button:
			prepare_button_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	void PlayDrawSound()
	{
		EffectSound m_Sound = SEffectManager.PlaySound("Expansion_Draws_SoundSet", GetGame().GetPlayer().GetPosition());
		m_Sound.SetSoundAutodestroy(true);
	}
};

class ExpansionBookMenuTabCraftingController: ExpansionViewController 
{
	ref ObservableCollection<ref ExpansionBookMenuTabCraftingCategory> CraftingCategoriesTabs = new ObservableCollection<ref ExpansionBookMenuTabCraftingCategory>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabCraftingIngredient> Ingredients1 = new ObservableCollection<ref ExpansionBookMenuTabCraftingIngredient>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabCraftingIngredient> Ingredients2 = new ObservableCollection<ref ExpansionBookMenuTabCraftingIngredient>(this);
	
	Object ItemPreview;
	string ItemName;

	static ref map<string, ref ExpansionBookCraftingCategoryRecipes> CraftingCategoriesRecipes = new map<string, ref ExpansionBookCraftingCategoryRecipes>;
	
	static void LoadRecipes()
	{
		if (CraftingCategoriesRecipes.Count() > 0)
			return;

		ExpansionBookSettings settings = GetExpansionSettings().GetBook();

		PluginRecipesManager recipes = PluginRecipesManager.Cast(GetPlugin(PluginRecipesManager));

		map<int, ref ExpansionBookCraftingRecipe> seenRecipes = new map<int, ref ExpansionBookCraftingRecipe>;

		for (int recipeIdx = 0; recipeIdx < recipes.m_RecipeList.Count(); recipeIdx++)
		{
			RecipeBase recipeBase = recipes.m_RecipeList[recipeIdx];
			if (!recipeBase)
				continue;

			array<string> ingredients1 = recipeBase.m_Ingredients[0];
			array<string> ingredients2 = recipeBase.m_Ingredients[1];
			if (!ingredients1 || ingredients1.Count() == 0 || !ingredients2 || ingredients2.Count() == 0)
				continue;

			//! For each recipe, check if any category contains one of the recipe results
			bool resultFound = false;
			ExpansionBookCraftingCategoryRecipes remainingRecipes;
			foreach (ExpansionBookCraftingCategory cat : settings.CraftingCategories)
			{
				ExpansionBookCraftingCategoryRecipes catRecipes;
				if (!CraftingCategoriesRecipes.Find(cat.CategoryName, catRecipes))
				{
					catRecipes = new ExpansionBookCraftingCategoryRecipes(cat.CategoryName);
					CraftingCategoriesRecipes.Insert(cat.CategoryName, catRecipes);
				}

				for (int i = 0; i < recipeBase.m_NumberOfResults; i++)
				{
					string result = recipeBase.m_ItemsToCreate[i];
					result.ToLower();
					if (cat.Results.Find(result) > -1)
					{
						resultFound = true;
						if (AddRecipe(seenRecipes, recipeIdx, catRecipes, recipeBase))
							break;
					}
				}

				if (cat.Results.Count() == 0 && !remainingRecipes)
					remainingRecipes = catRecipes;
			}

			//! If recipe result was not found in any category and a category with empty results exists, add recipe to that category
			if (!resultFound && remainingRecipes)
				AddRecipe(seenRecipes, recipeIdx, remainingRecipes, recipeBase);
		}
	}

	protected static bool AddRecipe(map<int, ref ExpansionBookCraftingRecipe> seenRecipes, int recipeIdx, ExpansionBookCraftingCategoryRecipes catRecipes, RecipeBase recipeBase)
	{
		ExpansionBookCraftingRecipe recipe;
		if (seenRecipes.Find(recipeIdx, recipe))
		{
			catRecipes.AddRecipe(recipe);  //! Re-use existing recipe
			return true;
		}
		else
		{
			recipe = catRecipes.AddRecipe(recipeBase);
			if (recipe)
			{
				seenRecipes.Insert(recipeIdx, recipe);
				return true;
			}
		}
		return false;
	}
};