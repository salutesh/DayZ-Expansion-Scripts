#ifdef JM_COT
//! @note using Playerbase::Expansion_GetReputation on client is not an option because it'll only work for players in network bubble,
//! so we send the value from server
modded class JMPlayerInstance
{
	private int m_ExpansionReputation;

	override void Update()
	{
		if (GetGame().IsServer() && (GetGame().GetTime() - m_DataLastUpdated) >= 100)
		{
			if (!GetGame().IsMultiplayer())
			{
				PlayerBase.CastTo(PlayerObject, GetGame().GetPlayer());
			}

			if (PlayerObject)
			{
				m_ExpansionReputation = PlayerObject.Expansion_GetReputation();
			}
		}

		super.Update();
	}

	override void OnSendHealth(ParamsWriteContext ctx)
	{
		super.OnSendHealth(ctx);

		ctx.Write(m_ExpansionReputation);
	}

	override void OnRecieveHealth(ParamsReadContext ctx)
	{
		super.OnRecieveHealth(ctx);

		ctx.Read(m_ExpansionReputation);
	}

	int GetExpansionReputation()
	{
		return m_ExpansionReputation;
	}
}
#endif
