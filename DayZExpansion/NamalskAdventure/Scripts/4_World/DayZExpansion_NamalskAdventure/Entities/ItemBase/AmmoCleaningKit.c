class AmmoCleaningKit: Inventory_Base 
{
	override void InitItemVariables()
	{
		super.InitItemVariables();
		can_this_be_combined = true;
	}
};