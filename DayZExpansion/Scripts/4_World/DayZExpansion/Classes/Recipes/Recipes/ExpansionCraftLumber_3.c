/**
 * ExpansionCraftLumber_3.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCraftLumber_3
 * @brief		
 **/
class ExpansionCraftLumber_3 extends RecipeBase
{	
	override void Init()
	{
		m_Name = "#STR_USRACT_CRAFT_LUMBER_3";
		
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
		
		m_MinQuantityIngredient[1] = 2;
		m_MaxQuantityIngredient[1] = 2;
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Ingredient 2
		InsertIngredient( 0, "WoodenLog" ); //! You can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = -1;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		m_IngredientUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this ingredient
		
		//! Ingredient 2
		InsertIngredient( 1, "Hacksaw" );
		InsertIngredient( 1, "HandSaw" );
		InsertIngredient( 1, "Chainsaw" );
		
		m_IngredientAddHealth[1] = -5;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; 
		m_IngredientAddQuantity[1] = 0;// 0 = do nothing
		m_IngredientDestroy[1] = false;
		m_IngredientUseSoftSkills[1] = true;// set 'true' to allow modification of the values by softskills on this ingredient
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Result
		AddResult( "ExpansionLumber3" ); //! Add results here

		m_ResultSetFullQuantity[0] = false;
		m_ResultSetQuantity[0] = 2;
		m_ResultSetHealth[0] = -1;
		m_ResultInheritsHealth[0] = -1;
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
		Debug.Log( "Recipe Do method called", "recipes" );
	}
}