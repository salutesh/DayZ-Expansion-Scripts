modded class AreaExposureMdfr
{
	override void OnActivate(PlayerBase player)
	{
		if (player.IsAI())
		{
			//! Skip the teleport check out of contaminated area for AI by temporarily setting area presence persistence to true.
			//! Vanilla super sets it to false again.
			player.SetPersistentFlag(PersistentFlag.AREA_PRESENCE, true);
		}

		super.OnActivate(player);
	}
}
