modded class HMP3Symptom
{
	//! Disable effects for AI in offline/SP
	override void OnUpdateClient(PlayerBase player, float deltatime)
	{
		if (!player.Expansion_IsAI())
			super.OnUpdateClient(player, deltatime);
	}

	override void OnGetActivatedClient(PlayerBase player)
	{
		if (!player.Expansion_IsAI())
			super.OnGetActivatedClient(player);
	}

	override void OnGetDeactivatedClient(PlayerBase player)
	{
		if (!player.Expansion_IsAI())
			super.OnGetDeactivatedClient(player);
	}
}
