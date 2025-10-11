modded class Edible_Base
{
	override void ChangeFoodStage(FoodStageType new_food_stage_type)
	{
		super.ChangeFoodStage(new_food_stage_type);

		if (new_food_stage_type == FoodStageType.ROTTEN)
		{
			eAIBase ai;
			if (Class.CastTo(ai, GetHierarchyRoot()))
				ai.eAI_OnFoodExpired(this);
		}
	}
}
