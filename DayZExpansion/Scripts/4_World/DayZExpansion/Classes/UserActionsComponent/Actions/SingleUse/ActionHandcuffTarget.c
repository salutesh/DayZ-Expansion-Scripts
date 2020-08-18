modded class ActionHandcuffTarget
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.IsInSafeZone() )
			return false;

		return super.ActionCondition(player, target, item);
	}
};