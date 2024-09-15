modded class ContaminatedArea_Base
{
	ref array<eAIBase> m_eAI_AI = {};

	override void OnPlayerEnterServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerEnterServer(player, trigger);

		player.Expansion_OnContaminatedAreaEnterServer(this, trigger);

		eAIBase ai;
		if (Class.CastTo(ai, player))
			m_eAI_AI.Insert(ai);
	}

	override void OnPlayerExitServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerExitServer(player, trigger);

		player.Expansion_OnContaminatedAreaExitServer(this, trigger);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		if (g_Game.IsServer())
		{
			foreach (eAIBase ai: m_eAI_AI)
			{
				if (ai)
					ai.eAI_ForgetContaminatedArea(this);
			}
		}
	}
}
