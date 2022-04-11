modded class PluginAdminLog
{
	override void PlayerHitBy(TotalDamageResult damageResult, int damageType, PlayerBase player, EntityAI source, int component, string dmgZone, string ammo)
	{
		if (player.IsAI()) return;
		
		super.PlayerHitBy(damageResult, damageType, player, source, component, dmgZone, ammo);
	}

	override void PlayerKilled(PlayerBase player, Object source)
	{
		if (player.IsAI()) return;

		super.PlayerKilled(player, source);
	}
};
