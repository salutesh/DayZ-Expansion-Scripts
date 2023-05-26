/**
 * NA_WorkbenchMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class NA_WorkbenchRecipeIngredient
{
	protected string m_TypeName;
	protected int m_Quantity;
	
	void NA_WorkbenchRecipeIngredient(string name, int quantity)
	{
		m_TypeName = name;
		m_Quantity = quantity;
	}
};

class NA_WorkbenchRecipeResult
{
	protected string m_TypeName;
	protected int m_Quantity;
	
	void NA_WorkbenchRecipeResult(string name, int quantity)
	{
		m_TypeName = name;
		m_Quantity = quantity;
	}
};

class NA_WorkbenchRecipe
{
	protected string m_DisplayName;
	protected ref array<ref NA_WorkbenchRecipeIngredient> m_RecipeIngredients = new array<ref NA_WorkbenchRecipeIngredient>;
	protected ref array<ref NA_WorkbenchRecipeResult> m_RecipeResults = new array<ref NA_WorkbenchRecipeResult>;

	void NA_WorkbenchRecipe(string displayName)
	{
		m_DisplayName = displayName;
	}
	
	string GetDisplayName()
	{
		return m_DisplayName;
	}
	
	void AddIngredient(NA_WorkbenchRecipeIngredient ingredient)
	{
		m_RecipeIngredients.Insert(ingredient);
	}
	
	array<ref NA_WorkbenchRecipeIngredient> GetIngredients()
	{
		return m_RecipeIngredients;
	}
	
	void AddResult(NA_WorkbenchRecipeResult result)
	{
		m_RecipeResults.Insert(result);
	}
	
	array<ref NA_WorkbenchRecipeResult> GetResults()
	{
		return m_RecipeResults;
	}
};

class NA_WorkbenchMenu: ExpansionScriptViewMenu
{
	protected ref NA_WorkbenchMenuController m_WorkbenchMenuController;
	
	protected Land_Repair_Center m_RepairCenter;
	protected RepairBenchGenerator m_RepairGenerator;
	
	protected ImageWidget fuel_pointer;
	
	protected ref array<ref NA_WorkbenchRecipe> m_WorkbenchRecipes = new array<ref NA_WorkbenchRecipe>;
	
	void NA_WorkbenchMenu()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Class.CastTo(m_WorkbenchMenuController, GetController());
		
		ExpansionNamalskModule.GetModuleInstance().GetWorkbenchMenuSI().Insert(SetWorkbenchItems);
		
		RegisterRecipes();
	}
	
	protected void RegisterRecipes()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		//! Test recipe - Anomaly Bolt
		NA_WorkbenchRecipe recipe = new NA_WorkbenchRecipe("Anomaly Bold - Ice");
		
		//! Ingredients
		recipe.AddIngredient(new NA_WorkbenchRecipeIngredient("Ammo_HuntingBolt", 1));
		recipe.AddIngredient(new NA_WorkbenchRecipeIngredient("Rope", 1));
		recipe.AddIngredient(new NA_WorkbenchRecipeIngredient("Expansion_AnomalyCore_Ice", 1));
		
		//! Results
		recipe.AddResult(new NA_WorkbenchRecipeResult("Expansion_Ammo_BoltAnomaly", 1));
		
		m_WorkbenchRecipes.Insert(recipe);
	}
	
	void SetWorkbenchItems(Land_Repair_Center repair_center, RepairBenchGenerator repair_generator)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ExDebugPrint("::SetWorkbenchItems - Repair Center: " + repair_center.ToString() + " | Active: " + repair_center.GetActiveState().ToString());
		ExDebugPrint("::SetWorkbenchItems - Repair Center Generator: " + repair_generator.ToString() + " | Working: " + repair_generator.GetCompEM().IsWorking().ToString());

		m_RepairCenter = repair_center;
		m_RepairGenerator = repair_generator;
		
		SetFuel(repair_generator.GetFuelPercentage());
		
		m_WorkbenchMenuController.ContainerItems.Clear();
		
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(GetGame().GetPlayer().GetInventory().CountInventory());
		GetGame().GetPlayer().GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item: items)
		{
			ExDebugPrint("::SetWorkbenchItems - Add Repair Center Container item: " + item.ToString());
			if (item == GetGame().GetPlayer())
				continue;
			
			NA_WorkbenchMenuItem playerItem = new NA_WorkbenchMenuItem(item);
			m_WorkbenchMenuController.ContainerItems.Insert(playerItem);
		}
		
		foreach (NA_WorkbenchRecipe recipe: m_WorkbenchRecipes)
		{
			NA_WorkbenchMenuRecipe recipeItem = new NA_WorkbenchMenuRecipe(recipe);
			m_WorkbenchMenuController.Recipes.Insert(recipeItem);
		}
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/WorkbenchMenu/na_workbech_menu.layout";
	}

	override typename GetControllerType()
	{
		return NA_WorkbenchMenuController;
	}
	
	protected void SetFuel(int fuel_percentage)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		fuel_pointer.SetRotation(0, 0, fuel_percentage * 260 - 130, true);
	}
	
	override float GetUpdateTickRate()
	{
		return 3.0;
	}
	
	override void Expansion_Update()
	{
		if (!m_RepairGenerator)
			return;

		SetFuel(m_RepairGenerator.GetFuelPercentage());
	}
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

class NA_WorkbenchMenuController: ExpansionViewController
{
	ref ObservableCollection<ref NA_WorkbenchMenuItem> ContainerItems = new ObservableCollection<ref NA_WorkbenchMenuItem>(this);
	ref ObservableCollection<ref NA_WorkbenchMenuRecipe> Recipes = new ObservableCollection<ref NA_WorkbenchMenuRecipe>(this);
};

class NA_WorkbenchMenuItem: ExpansionScriptView
{
	protected ref NA_WorkbenchMenuItemController m_WorkbenchMenuItemController;
	
	protected EntityAI m_Item;
	
	void NA_WorkbenchMenuItem(EntityAI item)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Class.CastTo(m_WorkbenchMenuItemController, GetController());
		
		m_Item = item;
		
		SetView();
	}
	
	void SetView()
	{
		m_WorkbenchMenuItemController.ItemName = m_Item.GetDisplayName();
		m_WorkbenchMenuItemController.NotifyPropertyChanged("ItemName");
		
		m_WorkbenchMenuItemController.Preview = m_Item;
		m_WorkbenchMenuItemController.NotifyPropertyChanged("Preview");
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/WorkbenchMenu/na_workbech_menu_item.layout";
	}

	override typename GetControllerType()
	{
		return NA_WorkbenchMenuItemController;
	}
};

class NA_WorkbenchMenuItemController: ExpansionViewController
{
	Object Preview;
	string ItemName;
};

class NA_WorkbenchMenuRecipe: ExpansionScriptView
{
	protected ref NA_WorkbenchMenuRecipeController m_WorkbenchMenuRecipeController;
	protected ref NA_WorkbenchRecipe m_Recipe;
	
	void NA_WorkbenchMenuRecipe(NA_WorkbenchRecipe recipe)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Class.CastTo(m_WorkbenchMenuRecipeController, GetController());
		
		m_Recipe = recipe;
		
		SetView();
	}
	
	void SetView()
	{
		NA_WorkbenchRecipeResult result = m_Recipe.GetResults().Get(0);
		if (!result)
			return;
		
		m_WorkbenchMenuRecipeController.RecipeName = m_Recipe.GetDisplayName();
		m_WorkbenchMenuRecipeController.NotifyPropertyChanged("RecipeName");
		
		//m_WorkbenchMenuRecipeController.Preview = m_Item;
		//m_WorkbenchMenuRecipeController.NotifyPropertyChanged("Preview");
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/WorkbenchMenu/na_workbech_menu_recipe.layout";
	}

	override typename GetControllerType()
	{
		return NA_WorkbenchMenuRecipeController;
	}
};

class NA_WorkbenchMenuRecipeController: ExpansionViewController
{
	Object Preview;
	string RecipeName;
};