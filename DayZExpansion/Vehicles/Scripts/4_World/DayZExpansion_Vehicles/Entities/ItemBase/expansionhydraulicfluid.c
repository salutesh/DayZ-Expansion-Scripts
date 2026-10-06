class ExpansionHydraulicFluid: ItemBase
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ExpansionActionFillHydraulicFluid);
	}
}
