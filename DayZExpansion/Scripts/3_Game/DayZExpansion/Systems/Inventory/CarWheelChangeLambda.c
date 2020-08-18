class CarWheelChangeLambda: ReplaceItemWithNewLambdaBase
{
 	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		hndDebugPrint("[inv] ReplaceItemWithNewLambdaBase Step E) Copying props " + old_item + " --> " + new_item);
		float oldHealth = old_item.GetHealth() / old_item.GetMaxHealth();
		new_item.SetHealth(new_item.GetMaxHealth() * oldHealth);
	}
};