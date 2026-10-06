#ifdef JM_COT
//! @note using Playerbase::Expansion_GetReputation on client is not an option because it'll only work for players in network bubble,
//! so we send the value from server
modded class JMPlayerInstance
{
	private int m_ExpansionReputation;

	override void Update()
	{
		if (g_Game.IsServer() && (g_Game.GetTime() - m_DataLastUpdated) >= 100)
		{
			if (!g_Game.IsMultiplayer())
			{
				PlayerBase.CastTo(PlayerObject, g_Game.GetPlayer());
			}

			if (PlayerObject)
			{
				m_ExpansionReputation = PlayerObject.Expansion_GetReputation();
			}
		}

		super.Update();
	}

#ifdef COT_ONRECEIVE_PROPER
	override void OnSendStatus(ParamsWriteContext ctx)
	{
		super.OnSendStatus(ctx);

		ctx.Write(m_ExpansionReputation);
	}

	override bool OnReceiveStatus(ParamsReadContext ctx)
	{
		if (!super.OnReceiveStatus(ctx))
			return false;
			
		if (!ctx.Read(m_ExpansionReputation))
			return false;

		return true;
	}
#else
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
#endif

	int GetExpansionReputation()
	{
		return m_ExpansionReputation;
	}
}
#endif
