/**
 * ExpansionCraftMilkBottle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCraftMilkBottle
 * @brief		
 **/
class ExpansionCraftMilkBottle extends RecipeBase
{	
	override void Init()
	{
		m_Name = "#STR_USRACT_CRAFT_MILK";
		
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
		
		//! Ingredient 1
		InsertIngredient(0, "PowderedMilk"); //! You can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;
		m_IngredientSetHealth[0] = -1; 
		m_IngredientAddQuantity[0] = -1;
		m_IngredientDestroy[0] = true;
		m_IngredientUseSoftSkills[0] = false;
		
		//! Ingredient 2
		InsertIngredient(1, "WaterBottle"); //! You can insert multiple ingredients this way
		
		m_IngredientAddHealth[1] = 0;
		m_IngredientSetHealth[1] = -1; 
		m_IngredientAddQuantity[1] = -1;
		m_IngredientDestroy[1] = true;
		m_IngredientUseSoftSkills[1] = false;
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Result
		AddResult("ExpansionMilkBottle"); //! Add results here

		m_ResultSetFullQuantity[0] = false;
		m_ResultSetQuantity[0] = -1;
		m_ResultSetHealth[0] = -1;
		m_ResultInheritsHealth[0] = 0;
		m_ResultInheritsColor[0] = -1;
		m_ResultToInventory[0] = -2;
		m_ResultUseSoftSkills[0] = false;
		m_ResultReplacesIngredient[0] = -1;  //! -1 means do nothing, >= 0 means transfer properties of ingredient with that index
		//! NOTE: Using m_ResultReplacesIngredient[0] = 1 here would make the resulting milk bottle contain water
		//! if the original bottle ingredient was also a milk bottle. Use Do() instead to set quantity of milk.
	}

	override bool CanDo(ItemBase ingredients[], PlayerBase player) //! Final check for recipe's validity
	{
		return true;
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight) //! Gets called upon recipe's completion
	{
		results[0].SetQuantity( ingredients[1].GetQuantity() );
	}
}