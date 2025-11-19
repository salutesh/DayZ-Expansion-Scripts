modded class DayZPlayerImplementFallDamage
{
	override void HandleFallDamage(FallDamageData pData)
	{
		if (g_Game.IsServer())
		{
			DayZPlayerImplement player;
			//! @note `FallDamage` is just a placeholder we use, it doesn't actually exist as ammo since DayZ 1.21
			if (Class.CastTo(player, m_Player) && !player.Expansion_CanBeDamaged("FallDamage"))
				return;
		}

		super.HandleFallDamage(pData);
	}
}
