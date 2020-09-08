modded class KitchenKnife
{
	override void SetActions()
	{
		super.SetActions();
		#ifdef DAYZ_1_09
		AddAction(ActionDestroyFlag);
		#endif
	}
}