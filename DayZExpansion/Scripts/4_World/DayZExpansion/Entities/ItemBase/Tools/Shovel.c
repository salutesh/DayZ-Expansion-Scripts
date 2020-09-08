modded class Shovel
{
	override void SetActions()
	{
		super.SetActions();
		#ifdef DAYZ_1_09
		AddAction(ActionDestroyTerritory);
		#endif
	}
}