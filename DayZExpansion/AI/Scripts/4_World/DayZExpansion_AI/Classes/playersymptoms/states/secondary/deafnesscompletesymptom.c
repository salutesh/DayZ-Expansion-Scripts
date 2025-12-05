modded class DeafnessCompleteSymptom
{
	//! Disable effects for AI in offline/SP
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
