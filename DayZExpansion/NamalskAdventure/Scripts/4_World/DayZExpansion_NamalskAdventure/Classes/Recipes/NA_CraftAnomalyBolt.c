/**
 * NA_CraftAnomalyBolt.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class NA_CraftAnomalyBolt extends RecipeBase
{
	override void Init()
	{
		m_Name = "Craft Anomaly Bolt";
		m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
		m_AnimationLength = 0.5;//animation length in relative time units
		m_Specialty = 0.02;// value > 0 for roughness, value < 0 for precision


		//conditions
		m_MinDamageIngredient[0] = -1;//-1 = disable check
		m_MaxDamageIngredient[0] = 3;//-1 = disable check

		m_MinQuantityIngredient[0] = 1;//-1 = disable check
		m_MaxQuantityIngredient[0] = -1;//-1 = disable check

		m_MinDamageIngredient[1] = -1;//-1 = disable check
		m_MaxDamageIngredient[1] = 3;//-1 = disable check

		m_MinQuantityIngredient[1] = 1;//-1 = disable check
		m_MaxQuantityIngredient[1] = -1;//-1 = disable check
		//----------------------------------------------------------------------------------------------------------------------

		//INGREDIENTS
		//ingredient 1
		InsertIngredient(0,"Ammo_HuntingBolt");//you can insert multiple ingredients this way

		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = 0;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		m_IngredientUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this ingredient

		//ingredient 2
		InsertIngredient(1,"Expansion_AnomalyCore_Ice");//you can insert multiple ingredients this way
		InsertIngredient(1,"Expansion_AnomalyCore_Warper");//you can insert multiple ingredients this way

		m_IngredientAddHealth[1] = 0;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = -1;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		m_IngredientUseSoftSkills[1] = false;// set 'true' to allow modification of the values by softskills on this ingredient
		//----------------------------------------------------------------------------------------------------------------------
	}

	override void Do(ItemBase ingredients[], PlayerBase player, array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		float resultHealth;
		
		Magazine boltMag;
		if (Class.CastTo(boltMag, ingredients[0]))
		{
			resultHealth = boltMag.GetHealth01("", "");
			int ammoCount = boltMag.GetAmmoCount();
			int newAmmoCount = ammoCount - 1;
			if (newAmmoCount == 0)
			{
				GetGame().ObjectDelete(boltMag);
			}
			else
			{
				boltMag.ServerSetAmmoCount(newAmmoCount);
			}
		}
		
		Expansion_AnomalyCore_Base anomalyCore;
		if (Class.CastTo(anomalyCore, ingredients[1]))
		{
			vector pos = player.GetPosition();
			float surfaceY = GetGame().SurfaceY(pos[0], pos[2]);
			Expansion_Ammo_BoltAnomaly_Base anomalyBolt;
			
			Expansion_AnomalyCore_Ice anomalyCoreIce;
			if (Class.CastTo(anomalyCoreIce, anomalyCore))
			{
				anomalyBolt = Expansion_Ammo_BoltAnomaly_Base.Cast(GetGame().CreateObject("Expansion_Ammo_BoltAnomaly_Ice", Vector(pos[0], surfaceY, pos[2])));
			}
			
			Expansion_AnomalyCore_Warper anomalyCoreWarper;
			if (Class.CastTo(anomalyCoreWarper, anomalyCore))
			{
				anomalyBolt = Expansion_Ammo_BoltAnomaly_Base.Cast(GetGame().CreateObject("Expansion_Ammo_BoltAnomaly_Warper", Vector(pos[0], surfaceY, pos[2])));
			}
			
			if (anomalyBolt)
			{
				anomalyBolt.SetHealth01("", "", resultHealth);
			
				Magazine newBoltMag;
				if (Class.CastTo(newBoltMag, anomalyBolt))
				{
					newBoltMag.ServerSetAmmoCount(1);
				}
			}

			GetGame().ObjectDelete(anomalyCore);
		}
	}
};
