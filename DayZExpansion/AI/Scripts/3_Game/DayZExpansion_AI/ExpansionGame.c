modded class ExpansionGame
{
	protected ref eAICommandManager m_CommandManager;

	private bool m_InGroup;

	void ExpansionGame()
	{
		m_RPCManager.RegisterClient("RPC_SetInGroup");
	}

	eAICommandManager GetCommandManager()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetCommandManager");
#endif

		return m_CommandManager;
	}

	ExpansionRPCManager GetRPCManager()
	{
		return m_RPCManager;
	}

	void RPC_SetInGroup(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		bool group;
		if (ctx.Read(group))
			SetInGroup(group);
	}

	/**
	 * @note Client only
	 */
	bool InGroup()
	{
		return m_InGroup;
	}

	/**
	 * @note Client only
	 */
	void SetInGroup(bool group)
	{
		m_InGroup = group;
	}

	void eAI_Spectate(DayZPlayer player, PlayerIdentity sender);
};
