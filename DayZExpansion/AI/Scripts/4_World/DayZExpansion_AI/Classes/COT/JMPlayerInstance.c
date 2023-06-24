#ifdef JM_COT
modded class JMPlayerInstance
{
	private int m_ExpansionFactionTypeID;

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
				if ( PlayerObject.GetGroup() )
					m_ExpansionFactionTypeID = PlayerObject.GetGroup().GetFaction().GetTypeID();
				else
					m_ExpansionFactionTypeID = 0;
			}
		}

		super.Update();
	}

	override void OnSendHealth(ParamsWriteContext ctx)
	{
		super.OnSendHealth(ctx);

		ctx.Write(m_ExpansionFactionTypeID);
	}

	override void OnRecieveHealth(ParamsReadContext ctx)
	{
		super.OnRecieveHealth(ctx);

		ctx.Read(m_ExpansionFactionTypeID);
	}

	int GetExpansionFactionTypeID()
	{
		return m_ExpansionFactionTypeID;
	}
}
#endif
