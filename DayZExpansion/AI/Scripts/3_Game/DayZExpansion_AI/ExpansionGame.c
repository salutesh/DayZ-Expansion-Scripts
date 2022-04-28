modded class ExpansionGame
{
	protected ref eAICommandManager m_CommandManager;

	private bool m_IsAdmin;
	private bool m_InGroup;

	void ExpansionGame()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ExpansionGame");
#endif

		// anything dependent on settings during init must be initialized first.
		eAISettings.Init();

		SetAdmin(false);
		GetRPCManager().AddRPC("eAI", "RPC_SetAdmin", this, SingeplayerExecutionType.Client);
	}

	eAICommandManager GetCommandManager()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetCommandManager");
#endif

		return m_CommandManager;
	}

	/**
	 * @note Client only
	 */
	bool IsAdmin()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "IsAdmin");
#endif

		return m_IsAdmin;
	}

	/**
	 * @note Client only
	 */
	void SetAdmin(bool admin)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetAdmin");
#endif

		m_IsAdmin = admin;
	}

	/**
	 * @note Client only
	 */
	bool InGroup()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "InGroup");
#endif

		return m_InGroup;
	}

	/**
	 * @note Client only
	 */
	void SetInGroup(bool group)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetInGroup");
#endif

		m_InGroup = group;
	}

	void RPC_SetAdmin(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "RPC_SetAdmin");
#endif

		Param1<bool> data;
		if (!ctx.Read(data))
			return;

		if (GetGame().IsServer())
			return;

		SetAdmin(data.param1);
	}

	void InvokeOnConnect(DayZPlayer player, PlayerIdentity identity)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "InvokeOnConnect");
#endif

		string guid = identity.GetPlainId();
		int idx = eAISettings.GetAdmins().Find(guid);

		GetRPCManager().SendRPC("eAI", "RPC_SetAdmin", new Param1<bool>(idx != -1), true, identity);
	}
};
