/**
 * Expansion_3DPrinter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_SURVIVAL
class NA_WorkbenchRecipeIngredient
{
	protected string m_TypeName;
	protected int m_Quantity;
	
	void Set(string name, int quantity)
	{
		m_TypeName = name;
		m_Quantity = quantity;
	}
	
	string GetType()
	{
		return m_TypeName;
	}
	
	int GetQuantity()
	{
		return m_Quantity;
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_TypeName);
		ctx.Write(m_Quantity);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_TypeName))
			return false;
		
		if (!ctx.Read(m_Quantity))
			return false;
		
		return true;
	}
};

class NA_WorkbenchRecipeResult
{
	protected string m_TypeName;
	protected int m_Quantity;
	
	void Set(string name, int quantity)
	{
		m_TypeName = name;
		m_Quantity = quantity;
	}
	
	string GetType()
	{
		return m_TypeName;
	}
	
	int GetQuantity()
	{
		return m_Quantity;
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_TypeName);
		ctx.Write(m_Quantity);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_TypeName))
			return false;
		
		if (!ctx.Read(m_Quantity))
			return false;
		
		return true;
	}
};

class NA_WorkbenchRecipe
{
	protected string m_DisplayName;
	protected ref array<ref NA_WorkbenchRecipeIngredient> m_RecipeIngredients = new array<ref NA_WorkbenchRecipeIngredient>;
	protected ref array<ref NA_WorkbenchRecipeResult> m_RecipeResults = new array<ref NA_WorkbenchRecipeResult>;
	
	void SetDisplayName(string displayName)
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
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_DisplayName);
		ctx.Write(m_RecipeIngredients.Count());
	
		foreach (NA_WorkbenchRecipeIngredient ingredient: m_RecipeIngredients)
		{
			ingredient.OnSend(ctx);
		}
	
		ctx.Write(m_RecipeResults.Count());
		foreach (NA_WorkbenchRecipeResult result: m_RecipeResults)
		{
			result.OnSend(ctx);
		}
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		int i;
		
		if (!ctx.Read(m_DisplayName))
			return false;
		
		int ingredientCount;
		if (!ctx.Read(ingredientCount))
			return false;
		
		for (i = 0; i < ingredientCount; i++)
		{
			NA_WorkbenchRecipeIngredient ingredient = new NA_WorkbenchRecipeIngredient();
			if (!ingredient.OnRecieve(ctx))
				return false;
			
			m_RecipeIngredients.Insert(ingredient);
		}
		
		int resultCount;
		if (!ctx.Read(resultCount))
			return false;
		
		for (i = 0; i < resultCount; i++)
		{
			NA_WorkbenchRecipeResult result = new NA_WorkbenchRecipeResult();
			if (!result.OnRecieve(ctx))
				return false;
			
			m_RecipeResults.Insert(result);
		}
		
		return true;
	}
};

class Expansion_3DPrinter: ItemBase
{
	static int PRINTING_LENGTH = 20;
	protected bool m_Activated = false;
	protected float m_PrinterAnimPhase = PRINTING_LENGTH;
	protected bool m_CanViewCargo;
		
	protected ref NA_WorkbenchRecipe m_Recipe;
	
	protected ref map<string, ref ExpansionInventoryItemType> m_InventoryItems;

	void Expansion_3DPrinter()
	{
		RegisterNetSyncVariableBool("m_Activated");
		RegisterNetSyncVariableBool("m_CanViewCargo");
		
		SetCanViewCargo(true);
	}

	void ~Expansion_3DPrinter()
	{
	}

	//! Activate/Deactivate
	void ActivatePrinter()
	{
		SetAnimationPhase("PrinterHead", m_PrinterAnimPhase);
		m_PrinterAnimPhase += PRINTING_LENGTH;

		m_Activated = true;
		SetSynchDirty();
	}

	bool GetActivation()
	{
		return m_Activated;
	}

	void DeactivatePrinter()
	{
		m_Activated = false;
		SetSynchDirty();
	}

	void SetCanViewCargo(bool state)
	{
		m_CanViewCargo = state;
		SetSynchDirty();
	}
	
	bool CanViewCargo()
	{
		return m_CanViewCargo;
	}
	
	//! Printing process
	void CompletePrinting()
	{
		if (!m_Recipe)
			return;
		
		array<ref NA_WorkbenchRecipeIngredient> recipeIngredients = m_Recipe.GetIngredients();		
		foreach (NA_WorkbenchRecipeIngredient ingredient: recipeIngredients)
		{
			string typeName = ingredient.GetType();
			int needed = ingredient.GetQuantity();
			
			ExpansionInventoryItemType invType;
			if (!m_InventoryItems.Find(typeName, invType))
				continue;
			
			if (invType.GetStackAmount() <= needed)
			{
				foreach (ItemBase item : invType.Items)
				{
					GetGame().ObjectDelete(item);
				}
			}
		}
		
		array<ref NA_WorkbenchRecipeResult> recipeResults = m_Recipe.GetResults();
		foreach (NA_WorkbenchRecipeResult result: recipeResults)
		{
			string resultTypeName = result.GetType();
			int resultQuantity = result.GetQuantity();

			ItemBase resultIB = ItemBase.Cast(GetGame().CreateObjectEx(resultTypeName, GetGame().ObjectModelToWorld(this, GetMemoryPointPos("spawn_suit")), ECE_NONE));
			resultIB.SetDirection(GetDirection());
			
			if (resultIB.Expansion_IsStackable())
			{
				resultIB.Expansion_SetStackAmount(resultQuantity);
			}
			else
			{
				for (int i = 0; i < resultQuantity - 1; i++)
				{
					resultIB = ItemBase.Cast(GetGame().CreateObjectEx(resultTypeName, GetGame().ObjectModelToWorld(this, GetMemoryPointPos("spawn_suit")), ECE_NONE));
					resultIB.SetDirection(GetDirection());
				}
			}
		}
		
		//! Shutdown printer
		DeactivatePrinter();
		SetCanViewCargo(true);
	}
	
	void SetRecipe(NA_WorkbenchRecipe recipe)
	{
		m_Recipe = recipe;
		
		UpdatePrinterInventory();
	}
	
	protected void UpdatePrinterInventory()
	{
		if (!m_InventoryItems)
			m_InventoryItems = new map<string, ref ExpansionInventoryItemType>;
		else
			m_InventoryItems.Clear();
		
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(GetInventory().CountInventory());
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item: items)
		{
			if (item == this)
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
		}
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUseRepairCenter);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanDisplayCargo()
	{
		return CanViewCargo();
	}
	
	override bool IsInventoryVisible()
	{
		return CanViewCargo();
	}
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return GetGame().IsDedicatedServer() || CanViewCargo();
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		return GetGame().IsDedicatedServer() || CanViewCargo();
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		return GetGame().IsDedicatedServer() || CanViewCargo();
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		return GetGame().IsDedicatedServer() || CanViewCargo();
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		if (!super.CanSwapItemInCargo(child_entity, new_entity))
			return false;

		return GetGame().IsDedicatedServer() || CanViewCargo();
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool DisableVicinityIcon()
    {
        return true;
    }
};
#endif
