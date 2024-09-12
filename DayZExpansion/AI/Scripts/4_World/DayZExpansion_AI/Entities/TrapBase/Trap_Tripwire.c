modded class TripwireTrap
{
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		ItemBase itemBase;
		if (Class.CastTo(itemBase, item) && itemBase.Expansion_IsExplosive())
			GetTargetInformation().UpdateItemInfo();
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		ItemBase itemBase;
		if (Class.CastTo(itemBase, item) && itemBase.Expansion_IsExplosive())
			GetTargetInformation().UpdateItemInfo();
	}
}