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

class NA_WorkbenchMenu: ExpansionScriptViewMenu
{
	protected ref NA_WorkbenchMenuController m_WorkbenchMenuController;
	protected Expansion_3DPrinter m_3DPrinter;
	protected ref array<ref NA_WorkbenchRecipe> m_WorkbenchRecipes;
	protected ref map<string, ref ExpansionInventoryItemType> m_InventoryItems;
	protected ref NA_WorkbenchRecipe m_SelectedRecipe;
	protected bool m_CanCraft;
	protected bool m_IsCrafting;
	
	protected ImageWidget fuel_pointer;
	protected Widget print_button_panel;

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
		
		if (!m_WorkbenchRecipes)
			m_WorkbenchRecipes = new array<ref NA_WorkbenchRecipe>;
		
		//! Anomaly Bolt - Ice
		NA_WorkbenchRecipe recipe = new NA_WorkbenchRecipe();
		recipe.SetDisplayName("Anomaly Bold - Ice");
		
		//! Ingredients
		NA_WorkbenchRecipeIngredient ingredient = new NA_WorkbenchRecipeIngredient();
		ingredient.Set("Ammo_HuntingBolt", 1);
		recipe.AddIngredient(ingredient);
		
		ingredient = new NA_WorkbenchRecipeIngredient();
		ingredient.Set("Rope", 1);
		recipe.AddIngredient(ingredient);
		
		ingredient = new NA_WorkbenchRecipeIngredient();
		ingredient.Set("Expansion_AnomalyCore_Ice", 1);
		recipe.AddIngredient(ingredient);
		
		//! Results
		NA_WorkbenchRecipeResult result = new NA_WorkbenchRecipeResult();
		result.Set("Expansion_Ammo_BoltAnomaly_Ice", 1);
		recipe.AddResult(result);
		
		m_WorkbenchRecipes.Insert(recipe);
		
		//! Anomaly Bolt - Warper
		recipe = new NA_WorkbenchRecipe();
		recipe.SetDisplayName("Anomaly Bold - Warper");
		
		//! Ingredients
		ingredient = new NA_WorkbenchRecipeIngredient();
		ingredient.Set("Ammo_HuntingBolt", 1);
		recipe.AddIngredient(ingredient);
		
		ingredient = new NA_WorkbenchRecipeIngredient();
		ingredient.Set("Rope", 1);
		recipe.AddIngredient(ingredient);
		
		ingredient = new NA_WorkbenchRecipeIngredient();
		ingredient.Set("Expansion_AnomalyCore_Warper", 1);
		recipe.AddIngredient(ingredient);
		
		//! Results
		result = new NA_WorkbenchRecipeResult();
		result.Set("Expansion_Ammo_BoltAnomaly_Warper", 1);
		recipe.AddResult(result);
		
		m_WorkbenchRecipes.Insert(recipe);
	}
	
	void SetWorkbenchItems(Expansion_3DPrinter printer)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ExDebugPrint("::SetWorkbenchItems - 3D Printer: " + printer.ToString());

		//SetFuel(repair_generator.GetFuelPercentage());
		m_3DPrinter = printer;
		
		if (m_3DPrinter)
			UpdatePrinterInventory();
		
		if (m_3DPrinter.GetActivation())
			m_IsCrafting = true;
		
		foreach (NA_WorkbenchRecipe recipe: m_WorkbenchRecipes)
		{
			NA_WorkbenchMenuRecipe recipeItem = new NA_WorkbenchMenuRecipe(recipe, this);
			m_WorkbenchMenuController.Recipes.Insert(recipeItem);
		}
	}
	
	protected void UpdatePrinterInventory()
	{
		m_WorkbenchMenuController.ContainerItems.Clear();

		if (!m_InventoryItems)
			m_InventoryItems = new map<string, ref ExpansionInventoryItemType>;
		else
			m_InventoryItems.Clear();
		
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(m_3DPrinter.GetInventory().CountInventory());
		m_3DPrinter.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item: items)
		{
			ExDebugPrint("::SetWorkbenchItems - Add Repair Center Container item: " + item.ToString());
			if (item == m_3DPrinter)
				continue;
			
			ItemBase itemIB;
			if (!Class.CastTo(itemIB, item))
				continue;
			
			string typeName = itemIB.GetType();
			
			ExpansionInventoryItemType invType;
			if (m_InventoryItems.Find(typeName, invType))
			{
				if (invType.Items.Find(itemIB) == -1)
					invType.Items.Insert(itemIB);
			}
			else
			{
				invType = new ExpansionInventoryItemType();
				if (invType.Items.Find(itemIB) == -1)
				{
					invType.Items.Insert(itemIB);
					m_InventoryItems.Insert(typeName, invType);
				}
			}
			
			NA_WorkbenchMenuItem containerItem = new NA_WorkbenchMenuItem(itemIB);
			m_WorkbenchMenuController.ContainerItems.Insert(containerItem);
		}
	}
	
	void OnRecipeSelected(NA_WorkbenchRecipe recipe)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_CanCraft = false;
		int overallNeeded = 0;
		int overallCount = 0;
		
		m_SelectedRecipe = recipe;

		array<ref NA_WorkbenchRecipeIngredient> ingredients = m_SelectedRecipe.GetIngredients();
		foreach (NA_WorkbenchRecipeIngredient ingredient: ingredients)
		{
			string typeName = ingredient.GetType();
			int needed = ingredient.GetQuantity();
			
			overallNeeded += needed;
			
			ExpansionInventoryItemType invType;
			if (!m_InventoryItems.Find(typeName, invType))
				continue;
			
			if (invType.GetStackAmount() <= needed)
				overallCount += invType.GetStackAmount();
		}
		
		if (overallCount == overallNeeded)
			m_CanCraft = true;
		
		if (m_CanCraft)
		{
			m_WorkbenchMenuController.TestResult = "Test Result: CAN PRINT";
			print_button_panel.Show(true);
		}
		else
		{
			m_WorkbenchMenuController.TestResult = "Test Result: CAN'T PRINT";
			print_button_panel.Show(false);
		}
		
		m_WorkbenchMenuController.NotifyPropertyChanged("TestResult");
	}
	
	void OnPrintClick()
	{
		if (!m_CanCraft || !m_3DPrinter || !m_SelectedRecipe)
			return;

		ExpansionNamalskModule.GetModuleInstance().SendWorkbenchResult(m_3DPrinter, m_SelectedRecipe);
		m_IsCrafting = true;

		CloseMenu();
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
		/*if (!m_RepairGenerator)
			return;

		SetFuel(m_RepairGenerator.GetFuelPercentage());*/
	}
	
	override void OnShow()
	{
		super.OnShow();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		if (!m_IsCrafting)
			ExpansionNamalskModule.GetModuleInstance().SetWorkbenchState(m_3DPrinter, true);
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
	string TestResult;
};

class NA_WorkbenchMenuItem: ExpansionScriptView
{
	protected ref NA_WorkbenchMenuItemController m_WorkbenchMenuItemController;
	
	protected ItemBase m_Item;
	
	void NA_WorkbenchMenuItem(ItemBase item)
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
	protected NA_WorkbenchMenu m_Menu;
	protected EntityAI m_PrevObj;
	
	void NA_WorkbenchMenuRecipe(NA_WorkbenchRecipe recipe, NA_WorkbenchMenu menu)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Class.CastTo(m_WorkbenchMenuRecipeController, GetController());
		
		m_Recipe = recipe;
		m_Menu = menu;
		
		SetView();
	}
	
	void SetView()
	{
		NA_WorkbenchRecipeResult result = m_Recipe.GetResults().Get(0);
		if (!result)
			return;
		
		m_WorkbenchMenuRecipeController.RecipeName = m_Recipe.GetDisplayName();
		m_WorkbenchMenuRecipeController.NotifyPropertyChanged("RecipeName");		
		
		UpdatePreviewObject();
	}
	
	void UpdatePreviewObject()
	{
		NA_WorkbenchRecipeResult result = m_Recipe.GetResults().Get(0);
		
		string previewClassName = result.GetType();
		CreatePreviewObject(previewClassName, m_PrevObj);
		
		/*if (m_PrevObj)
		{
			if (m_PrevObj.IsInherited(TentBase))
			{
				TentBase tent;
				Class.CastTo(tent, m_PrevObj);
				tent.Pack(false);
			}
			
			Transport transportEntity;
			if (Class.CastTo(transportEntity, m_PrevObj))
			{
				dBodyActive(m_PrevObj, ActiveState.INACTIVE);
				dBodyDynamic(m_PrevObj, false);
				transportEntity.DisableSimulation(true);
			}

			if (m_PrevObj.HasSelection("antiwater"))
				m_PrevObj.HideSelection("antiwater");

			BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(m_PrevObj);
			if (baseBuilding && baseBuilding.CanUseConstruction())
			{
				bool isSupportedBB;
				if (baseBuilding.GetType() == "Fence" || baseBuilding.GetType() == "Watchtower" || baseBuilding.GetType() == "TerritoryFlag")
					isSupportedBB = true;
				#ifdef EXPANSIONMODBASEBUILDING
				else if (baseBuilding.IsInherited(ExpansionBaseBuilding))
					isSupportedBB = true;
				#endif
				if (isSupportedBB)
				{
					Construction construction = baseBuilding.GetConstruction();
					construction.Init();
					construction.ExpansionBuildFull();
				}
			}
		}*/
		
		m_WorkbenchMenuRecipeController.Preview = m_PrevObj;
		m_WorkbenchMenuRecipeController.NotifyPropertyChanged("Preview");
	}
	
	void CreatePreviewObject(string className, inout EntityAI preview)
	{
		if (preview)
		{
			if (CF_String.EqualsIgnoreCase(preview.GetType(), className))
			{
				//! Same classname, remove all attachments
				for (int i = preview.GetInventory().AttachmentCount() - 1; i >= 0; i--)
				{
					EntityAI attachment = preview.GetInventory().GetAttachmentFromIndex(i);
					if (attachment)
						GetGame().ObjectDelete(attachment);
				}

				return;
			}
			else
			{
				//! Different classname, delete old preview object
				GetGame().ObjectDelete(preview);
			}
		}

		if (!GetGame().IsKindOf(className, "DZ_LightAI"))
		{
			preview = EntityAI.Cast(GetGame().CreateObjectEx(className, vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
		#ifdef EXPANSIONMODHARDLINE
			ItemBase item;
			if (GetExpansionSettings().GetHardline().EnableItemRarity && Class.CastTo(item, preview))
				item.Expansion_SetRarity(GetExpansionSettings().GetHardline().GetItemRarityByType(item.GetType()));
		#endif
		}
	}
	
	void OnSelectRecipe()
	{
		m_Menu.OnRecipeSelected(m_Recipe);
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