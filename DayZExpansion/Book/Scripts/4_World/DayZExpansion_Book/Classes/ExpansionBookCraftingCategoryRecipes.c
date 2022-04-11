/**
 * ExpansionBookCraftingCategoryRecipes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookCraftingCategoryRecipes : ExpansionBookCraftingCategory
{
	ref array<ref ExpansionBookCraftingRecipe> Recipes;
	bool m_Sorted;

	ExpansionBookCraftingRecipe AddRecipe(RecipeBase recipeBase)
	{
		ExpansionBookCraftingRecipe recipe = new ExpansionBookCraftingRecipe(recipeBase);

		return AddRecipe(recipe);
	}

	ExpansionBookCraftingRecipe AddRecipe(ExpansionBookCraftingRecipe recipe)
	{
		//! Discard invalid recipes
		if (!recipe.IsValid())
			return NULL;

		if (!Recipes)
			Recipes = new array<ref ExpansionBookCraftingRecipe>;

		//! Check if an existing recipe produces the same result where only one ingredient is different and if so, add to it instead of adding new recipe
		foreach (ExpansionBookCraftingRecipe existingRecipe : Recipes)
		{
			if (ExpansionBookCraftingRecipe.FormatItems(recipe.Results) == existingRecipe.FormatItems(existingRecipe.Results))
			{
				if (ExpansionBookCraftingRecipe.FormatItems(recipe.Ingredients[1]) == existingRecipe.FormatItems(existingRecipe.Ingredients[1]))
				{
					CF_Log.Debug("Adding " + ExpansionBookCraftingRecipe.FormatItems(recipe.Ingredients[0]) + " to " + ExpansionBookCraftingRecipe.FormatItems(existingRecipe.Ingredients[0]));

					foreach (ExpansionBookCraftingItem ingredient : recipe.Ingredients[0])
					{
						existingRecipe.Ingredients[0].Insert(ingredient);
					}
					return existingRecipe;
				}
			}
		}

		Recipes.Insert(recipe);

		recipe.CategoriesCount++;

		return recipe;
	}

	void SortRecipes(bool splitRecipesPerResult = false)
	{
		if (m_Sorted)
			return;

		m_Sorted = true;

		array<ref ExpansionBookCraftingRecipe> resultsRecipes;

		map<string, ref array<ref ExpansionBookCraftingRecipe>> recipes = new map<string, ref array<ref ExpansionBookCraftingRecipe>>;
		foreach (ExpansionBookCraftingRecipe recipe : Recipes)
		{
			recipe.SortIngredients();

			if (splitRecipesPerResult)
			{
				recipe.SortResults();
				foreach (ExpansionBookCraftingItem result : recipe.Results)
				{
					string format = result.Format(true, false);
					AddResultsRecipe(result.Format(true, false), recipe, recipes, resultsRecipes);
				}
			}
			else
			{
				string displayName = recipe.GetDisplayName();
				AddResultsRecipe(displayName, recipe, recipes, resultsRecipes);
			}
		}

		TStringArray recipeResults = recipes.GetKeyArray();
		recipeResults.Sort();

		array<ref ExpansionBookCraftingRecipe> sortedRecipes = new array<ref ExpansionBookCraftingRecipe>;

		foreach (string recipeResult : recipeResults)
		{
			resultsRecipes = recipes.Get(recipeResult);
			foreach (ExpansionBookCraftingRecipe resultRecipe : resultsRecipes)
			{
				sortedRecipes.Insert(resultRecipe);
			}
		}

		Recipes = sortedRecipes;
	}

	protected void AddResultsRecipe(string key, ExpansionBookCraftingRecipe recipe, inout map<string, ref array<ref ExpansionBookCraftingRecipe>> recipes, inout array<ref ExpansionBookCraftingRecipe> resultsRecipes)
	{
		if (!recipes.Find(key, resultsRecipes))
		{
			resultsRecipes = new array<ref ExpansionBookCraftingRecipe>;
			recipes.Insert(key, resultsRecipes);
		}
		resultsRecipes.Insert(recipe);
	}
}

class ExpansionBookCraftingRecipe
{
	ref RecipeBase Recipe;
	ref array<ref ExpansionBookCraftingItem> Results;
	ref array<ref array<ref ExpansionBookCraftingItem>> Ingredients;
	int CategoriesCount;
	int m_TempMainIndex;

	void ExpansionBookCraftingRecipe(RecipeBase recipeBase)
	{
		Recipe = recipeBase;

		Results = recipeBase.ExpansionGetResults();
		Ingredients = recipeBase.ExpansionGetIngredients();

		if (!IsValid())
			return;
	}

	bool IsValid()
	{
		//! Discard invalid recipes with no results
		if (Results.Count() == 0)
			return false;

		foreach (array<ref ExpansionBookCraftingItem> items : Ingredients )
		{
			//! Discard invalid recipes with no ingredients
			if (items.Count() == 0)
				return false;
		}

		return true;
	}

	string GetDisplayName()
	{
		string recipeNameLower = Recipe.m_Name;
		recipeNameLower.ToLower();

		string displayName = Recipe.m_Name;

		switch (recipeNameLower)
		{
			case "#fill":
				displayName += " " + Results[m_TempMainIndex].DisplayName;
				break;
			case "#str_breakdown0":
			case "#str_gutandprepare0":
			case "#str_prepare0":
			case "#str_split0":
			case "#sharpen":
				if (Results.Count() > 1 && GetGame().IsKindOf(Results[m_TempMainIndex].ClassNames[0], "SeedBase"))
				{
					recipeNameLower = "#str_cutoutseeds0";
					displayName = "#str_cutoutseeds0";
				}
				else
				{
					displayName += " " + Ingredients[0][0].DisplayName;
					break;
				}
			case "#str_cutoutseeds0":
				if (Results[m_TempMainIndex].Amount > 1)
					displayName += " (" + Results[m_TempMainIndex].Amount + "x)";
				displayName = Ingredients[0][0].DisplayName + ": " + displayName;
				break;
			case "#str_craftgutsrope0":
			case "#str_craftragrope0":
				displayName = Ingredients[0][0].DisplayName + ": " + displayName;
				break;
			default:
				if (recipeNameLower.IndexOf("#str_craft") == 0 || recipeNameLower.IndexOf("#craft") == 0 || recipeNameLower == "#str_sawwoodenlog0")
				{
					//! TODO: Localize "Craft"
					displayName = "Craft " + Results[m_TempMainIndex].DisplayName;
					if (Results[m_TempMainIndex].Amount > 1)
						displayName += " (" + Results[m_TempMainIndex].Amount + "x)";
				}
				break;
		}

		return Widget.TranslateString(displayName);
	}

	static string FormatItems(array<ref ExpansionBookCraftingItem> items, bool translate = true, bool includeAmount = true)
	{
		TStringArray names = new TStringArray;

		foreach (ExpansionBookCraftingItem item : items)
		{
			names.Insert(item.Format(translate, includeAmount));
		}

		return ExpansionString.JoinStrings(names);
	}

	bool CanDo(PlayerBase player, array<EntityAI> items)
	{
		foreach (array<ref ExpansionBookCraftingItem> ingredients : Ingredients)
		{
			foreach (ExpansionBookCraftingItem ingredient : ingredients)
			{
				if (!ingredient.CanUseInRecipe(player, items, this))
					return false;
			}
		}

		return true;
	}

	void SortResults()
	{
		map<string, ref ExpansionBookCraftingItem> results = new map<string, ref ExpansionBookCraftingItem>;

		foreach (ExpansionBookCraftingItem result : Results)
		{
			string format = result.Format(true, false);
			results.Insert(format, result);
		}

		TStringArray resultsKeys = results.GetKeyArray();
		resultsKeys.Sort();

		array<ref ExpansionBookCraftingItem> sortedResults = new array<ref ExpansionBookCraftingItem>;

		foreach (string resultKey : resultsKeys)
		{
			sortedResults.Insert(results.Get(resultKey));
		}

		Results = sortedResults;
	}

	void SortIngredients()
	{
		for (int i = 0; i < Ingredients.Count(); i++)
		{
			map<string, ref ExpansionBookCraftingItem> ingredients = new map<string, ref ExpansionBookCraftingItem>;

			foreach (ExpansionBookCraftingItem ingredient : Ingredients[i])
			{
				string format = ingredient.Format(true, false);
				ingredients.Insert(format, ingredient);
			}

			TStringArray ingredientsKeys = ingredients.GetKeyArray();
			ingredientsKeys.Sort();

			array<ref ExpansionBookCraftingItem> sortedIngredients = new array<ref ExpansionBookCraftingItem>;

			foreach (string ingredientsKey : ingredientsKeys)
			{
				sortedIngredients.Insert(ingredients.Get(ingredientsKey));
			}

			Ingredients[i] = sortedIngredients;
		}
	}
}

class ExpansionBookCraftingItem
{
	string DisplayName;
	int Amount;
	autoptr TStringArray ClassNames = new TStringArray;

	bool m_CanUseInRecipe;
	ItemBase m_PlayerItem;

	void ExpansionBookCraftingItem(string displayName, int amount, TStringArray classNames = NULL)
	{
		DisplayName = displayName;
		Amount = amount;
		if (classNames)
			ClassNames = classNames;
	}

	string Format(bool translate = true, bool includeAmount = true)
	{
		if (translate)
		{
			if (includeAmount && Amount > 1)
				return Amount.ToString() + "x " + DisplayName;
			else
				return DisplayName;
		}
		else
		{
			return ExpansionString.JoinStrings(ClassNames);
		}
	}

	bool CanUseInRecipe()
	{
		return m_CanUseInRecipe;
	}

	//! Check whether this item is present in items and can be used in recipe
	bool CanUseInRecipe(PlayerBase player, array<EntityAI> items, ExpansionBookCraftingRecipe recipe)
	{
		m_PlayerItem = NULL;
		m_CanUseInRecipe = false;

		foreach (EntityAI item : items)
		{
			foreach (string className : ClassNames)
			{
				if (className != item.GetType() && !GetGame().IsKindOf(item.GetType(), className))
					continue;

				ItemBase itemBase = ItemBase.Cast(item);

				//! As CanDo and CheckConditions expect an array of two ingredients, but we are only interested in whether
				//! the current item could be used in this recipe, we insert it twice
				recipe.Recipe.m_IngredientsSorted[0] = itemBase;
				recipe.Recipe.m_IngredientsSorted[1] = itemBase;

				if (recipe.Recipe.CanDo(recipe.Recipe.m_IngredientsSorted, player) && recipe.Recipe.CheckConditions(recipe.Recipe.m_IngredientsSorted))
				{
					m_PlayerItem = itemBase;
					m_CanUseInRecipe = true;
					return true;
				}
			}
		}

		return false;
	}
}
