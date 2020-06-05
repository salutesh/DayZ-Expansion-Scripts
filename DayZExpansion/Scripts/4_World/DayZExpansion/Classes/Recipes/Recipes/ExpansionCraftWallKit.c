/**
 * ExpansionCraftWallKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCraftWallKit
 * @brief		
 **/
class ExpansionCraftWallKit extends RecipeBase
{	
	override void Init()
	{
		m_Name = "#STR_USRACT_CRAFT_WALL_KIT";
		
		m_IsInstaRecipe = false;
		m_AnimationLength = 1.5;
		m_Specialty = -0.02;
		
		//! Conditions
		m_MinDamageIngredient[0] = -1;
		m_MaxDamageIngredient[0] = 3;
		
		m_MinQuantityIngredient[0] = 1;
		m_MaxQuantityIngredient[0] = -1;
		
		m_MinDamageIngredient[1] = -1;
		m_MaxDamageIngredient[1] = 3;
		
		m_MinQuantityIngredient[1] = 1;
		m_MaxQuantityIngredient[1] = -1;
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Ingredient 2
		InsertIngredient(0, "WoodenStick"); //! You can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;
		m_IngredientSetHealth[0] = -1; 
		m_IngredientAddQuantity[0] = -1;
		m_IngredientDestroy[0] = true;
		m_IngredientUseSoftSkills[0] = false;
		
		//! Ingredient 2
		InsertIngredient(1, "Rope"); //! You can insert multiple ingredients this way
		
		m_IngredientAddHealth[1] = 0;
		m_IngredientSetHealth[1] = -1; 
		m_IngredientAddQuantity[1] = -1;
		m_IngredientDestroy[1] = true;
		m_IngredientUseSoftSkills[1] = false;
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Result
		AddResult("ExpansionWallKit"); //! Add results here

		m_ResultSetFullQuantity[0] = false;
		m_ResultSetQuantity[0] = 1;
		m_ResultSetHealth[0] = -1;
		m_ResultInheritsHealth[0] = -2;
		m_ResultInheritsColor[0] = -1;
		m_ResultToInventory[0] = -2;
		m_ResultUseSoftSkills[0] = false;
		m_ResultReplacesIngredient[0] = -1;
	}

    override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		bool canCraft = GetExpansionSettings().GetBaseBuilding().CanCraftExpansionBasebuilding;

		if ( canCraft )
		{
			return super.CanDo( ingredients, player );
		}
		else 
		{
			return false;
		}

		return super.CanDo( ingredients, player );
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight) //! Gets called upon recipe's completion
	{
		Debug.Log("Recipe Do method called", "recipes" );
	}
}