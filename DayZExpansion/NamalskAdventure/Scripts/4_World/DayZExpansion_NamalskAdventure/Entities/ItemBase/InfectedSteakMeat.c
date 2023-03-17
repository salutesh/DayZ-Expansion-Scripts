/**
 * InfectedSteakMeat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class InfectedSteakMeat extends HumanSteakMeat
{
	void InfectedSteakMeat()
	{
		InsertAgent(eAgents.FOOD_POISON, 1);
	}
	
	override void DeferredInit()
	{
		super.DeferredInit();

		Edible_Base food_item = this;
		if (food_item && food_item.HasFoodStage())
		{
			FoodStage foodStage = food_item.GetFoodStage();
			foodStage.ChangeFoodStage(FoodStageType.ROTTEN);
		}
	}
};