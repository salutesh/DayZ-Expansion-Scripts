modded class ActionUnmountBarbedWire
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory();

		if ( player.IsInSafeZone() )
			return false;

		return super.ActionCondition(player, target, item);
	}
}