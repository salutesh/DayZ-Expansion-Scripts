modded class ExpansionGame
{
	protected ref eAICommandManager m_CommandManager;

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
	bool InGroup()
	{
		return false;
	}
};
