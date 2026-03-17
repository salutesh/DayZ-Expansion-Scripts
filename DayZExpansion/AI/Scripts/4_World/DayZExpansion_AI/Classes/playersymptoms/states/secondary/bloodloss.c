modded class BloodLoss
{
	//! Disable effects for AI in offline/SP
	override void OnUpdateClient(PlayerBase player, float deltatime)
	{
		if (!player.Expansion_IsAI())
			super.OnUpdateClient(player, deltatime);
	}
}
