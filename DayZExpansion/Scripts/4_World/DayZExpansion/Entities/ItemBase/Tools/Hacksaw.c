modded class Hacksaw
{
	override void SetActions()
	{
		super.SetActions();
		#ifdef DAYZ_1_09
		AddAction(ActionDestroyFlag);
		AddAction(ActionDestroyTerritory);
		#endif
	}
}