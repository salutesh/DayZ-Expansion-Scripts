modded class DayZGame
{
	private ref eAIManagerBase m_eAI_Manager;

	void eAICreateManager()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAICreateManager");
#endif

		// fancy way to spawn the manager from a submodule
		Class.CastTo(m_eAI_Manager, "eAIManager".ToType().Spawn());
	}

	eAIManagerBase eAIManagerGet()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIManagerGet");
#endif

		return m_eAI_Manager;
	}

	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);

		if (m_eAI_Manager)
			m_eAI_Manager.OnUpdate(doSim, timeslice);
	}
};
