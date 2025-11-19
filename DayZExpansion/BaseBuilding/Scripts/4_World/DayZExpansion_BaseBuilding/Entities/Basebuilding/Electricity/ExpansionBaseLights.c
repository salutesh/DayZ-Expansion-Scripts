class ExpansionBaseLightBase: ItemBase
{
	override bool ExpansionCanRecievePower()
	{
		return true;
	}

	override void OnWorkStart()
	{
		g_Game.CreateObject("ExplosionTest", this.GetPosition());
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
};

modded class Spotlight
{
	override bool ExpansionCanRecievePower()
	{
		return true;
	}
}
