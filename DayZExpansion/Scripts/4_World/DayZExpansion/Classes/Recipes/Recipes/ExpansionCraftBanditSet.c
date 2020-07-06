/**
 * ExpansionCraftBanditSet.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCraftBanditSet
 * @brief		
 **/
class ExpansionCraftBanditSet extends RecipeBase
{	
	override void Init()
	{
		m_Name = "#STR_USRACT_UNPACK_BANDITSET";
		
		m_IsInstaRecipe = false;
		m_AnimationLength = 1;
		m_Specialty = -0.01;
		
		//! Conditions
		m_MinDamageIngredient[0] = -1;
		m_MaxDamageIngredient[0] = 3;
		
		m_MinQuantityIngredient[0] = -1;
		m_MaxQuantityIngredient[0] = -1;
		
		m_MinDamageIngredient[1] = -1;
		m_MaxDamageIngredient[1] = 3;
		
		m_MinQuantityIngredient[1] = -1;
		m_MaxQuantityIngredient[1] = -1;
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Ingredient 2
		InsertIngredient(0, "ExpansionBanditSet"); //! You can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;
		m_IngredientSetHealth[0] = -1; 
		m_IngredientAddQuantity[0] = 0;
		m_IngredientDestroy[0] = true;
		m_IngredientUseSoftSkills[0] = false;
		
		//! Ingredient 2
		InsertIngredient(1,"Sickle"); //! You can insert multiple ingredients this way
		InsertIngredient(1,"Hacksaw");
		InsertIngredient(1,"KitchenKnife");
		InsertIngredient(1,"SteakKnife");
		InsertIngredient(1,"HayHook");
		InsertIngredient(1,"StoneKnife");
		InsertIngredient(1,"Cleaver");
		InsertIngredient(1,"CombatKnife");
		InsertIngredient(1,"HuntingKnife");
		InsertIngredient(1,"Machete");
		InsertIngredient(1,"AK_Bayonet");
		InsertIngredient(1,"M9A1_Bayonet");
		InsertIngredient(1,"Mosin_Bayonet");
		InsertIngredient(1,"SKS_Bayonet");
		InsertIngredient(1,"Spear");
		InsertIngredient(1,"CanOpener");
		InsertIngredient(1,"Shovel");
		InsertIngredient(1,"FieldShovel");
		InsertIngredient(1,"Crowbar");
		InsertIngredient(1,"Hammer");
		InsertIngredient(1,"MeatTenderizer");
		InsertIngredient(1,"Wrench");
		InsertIngredient(1,"LugWrench");
		InsertIngredient(1,"Screwdriver");
		InsertIngredient(1,"FarmingHoe");
		InsertIngredient(1,"SledgeHammer");
		InsertIngredient(1,"Broom");
		InsertIngredient(1,"Chainsaw");
		InsertIngredient(1,"BaseballBat");
		InsertIngredient(1,"NailedBaseballBat");
		InsertIngredient(1,"PipeWrench");
		InsertIngredient(1,"TelescopicBaton");
		InsertIngredient(1,"NewHockeyStick");
		InsertIngredient(1,"OldHockeyStick");
		InsertIngredient(1,"Pitchfork");
		InsertIngredient(1,"Pickaxe");
		InsertIngredient(1,"Sword");
		InsertIngredient(1,"WoodAxe");
		InsertIngredient(1,"FirefighterAxe");
		InsertIngredient(1,"Hatchet");
		
		m_IngredientAddHealth[1] = -5;
		m_IngredientSetHealth[1] = -1; 
		m_IngredientAddQuantity[1] = 0;
		m_IngredientDestroy[1] = false;
		m_IngredientUseSoftSkills[1] = true;
		
		//----------------------------------------------------------------------------------------------------------------------
		
		//! Result
		AddResult("");

		m_ResultSetFullQuantity[0] = false;
		m_ResultSetQuantity[0] = -1;
		m_ResultSetHealth[0] = -1;
		m_ResultInheritsHealth[0] = 0;
		m_ResultInheritsColor[0] = -1;
		m_ResultToInventory[0] = -1;
		m_ResultUseSoftSkills[0] = true;
		m_ResultReplacesIngredient[0] = -1;
	}

	override bool CanDo(ItemBase ingredients[], PlayerBase player) //! Final check for recipe's validity
	{
		return true;
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight) //! Gets called upon recipe's completion
	{
		Debug.Log("Recipe Do method called", "recipes");

		ItemBase item1 = ItemBase.Cast( player.GetInventory().CreateInInventory( "USMCPants_Desert" ) );
		if ( !item1 )
			item1 = ExpansionCarKey.Cast( GetGame().CreateObject( "USMCPants_Desert", player.GetPosition() ) );

		ItemBase item2 = ItemBase.Cast( player.GetInventory().CreateInInventory( "USMCJacket_Desert" ) );
		if ( !item2 )
			item2 = ExpansionCarKey.Cast( GetGame().CreateObject( "USMCJacket_Desert", player.GetPosition() ) );

		ItemBase item3 = ItemBase.Cast( player.GetInventory().CreateInInventory( "PlateCarrierVest" ) );
		if ( !item3 )
			item3 = ExpansionCarKey.Cast( GetGame().CreateObject( "PlateCarrierVest", player.GetPosition() ) );

		ItemBase item4 = ItemBase.Cast( player.GetInventory().CreateInInventory( "TacticalGloves_Beige" ) );
		if ( !item4 )
			item4 = ExpansionCarKey.Cast( GetGame().CreateObject( "TacticalGloves_Beige", player.GetPosition() ) );

		ItemBase item5 = ItemBase.Cast( player.GetInventory().CreateInInventory( "CoyoteBag_Brown" ) );
		if ( !item5 )
			item5 = ExpansionCarKey.Cast( GetGame().CreateObject( "CoyoteBag_Brown", player.GetPosition() ) );

		ItemBase item6 = ItemBase.Cast( player.GetInventory().CreateInInventory( "HikingBoots_Black" ) );
		if ( !item6 )
			item6 = ExpansionCarKey.Cast( GetGame().CreateObject( "HikingBoots_Black", player.GetPosition() ) );

		ItemBase item7 = ItemBase.Cast( player.GetInventory().CreateInInventory( "BalaclavaMask_Beige" ) );
		if ( !item7 )
			item7 = ExpansionCarKey.Cast( GetGame().CreateObject( "BalaclavaMask_Beige", player.GetPosition() ) );
	}
}