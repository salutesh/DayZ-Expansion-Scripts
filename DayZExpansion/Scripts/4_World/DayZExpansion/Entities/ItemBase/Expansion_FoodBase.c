class Expansion_FoodBase : Edible_Base
{
    override bool CanBeCooked()
	{
		return true;
	}
	
	override bool CanBeCookedOnStick()
	{
		return true;
	}	
	
	override bool IsFruit()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionForceFeed);
		AddAction(ActionEatFruit);
	}
	
	override void EEOnCECreate()
	{
		int rand = Math.RandomInt(0, 10);

		if (rand > 6)
		{
			ChangeFoodStage(FoodStageType.ROTTEN);
			SetHealth(GetMaxHealth() * 0.1);
		}
		else if (rand > 2)
		{
			ChangeFoodStage(FoodStageType.DRIED);
			SetHealth(GetMaxHealth() * 0.4);
		}
	}
}