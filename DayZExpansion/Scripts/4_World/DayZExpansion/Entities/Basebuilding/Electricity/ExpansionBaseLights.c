class ExpansionBaseLightBase extends ItemBase
{
	override bool ExpansionCanRecievePower()
	{
		return true;
	}

	override void OnWorkStart()
	{
		GetGame().CreateObject("ExplosionTest", this.GetPosition());
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
