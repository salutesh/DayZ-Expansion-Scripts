class eAIFSM : ExpansionFSM
{
	protected eAIBase m_Unit;

	void Init(eAIBase unit)
	{
		m_Unit = unit;
	}

	eAIBase GetUnit()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetUnit");
		#endif

		return m_Unit;
	}
};
