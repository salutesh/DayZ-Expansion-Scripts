class eAIPatrol : Managed
{
	private static autoptr array<ref eAIPatrol> m_AllPatrols = new array<ref eAIPatrol>();
	static const float UPDATE_RATE_IN_SECONDS = 5.0;
	
	private ref Timer m_Timer;
	private bool m_IsBeingDestroyed;

	/**
	 * @brief Destroys a patrol
	 * 
	 * @param patrol the patrol to destroy
	 */
	static void DeletePatrol(eAIPatrol patrol)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIPatrol", "DeletePatrol").Add(patrol);
		#endif

		int index = m_AllPatrols.Find(patrol);
		m_AllPatrols.Remove(index);
	}

	/**
	 * @brief Privated constructor to prevent calling/storing in ref. The instance is managed through 'Create(X)Patrol' and 'DeletePatrol'
	 */
	private void eAIPatrol()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIPatrol");
		#endif

		m_AllPatrols.Insert(this);
	}

	/**
	 * @brief Privated destructor to prevent calling/storing in ref. The instance is managed through 'Create(X)Patrol' and 'DeletePatrol'
	 */
	private void ~eAIPatrol()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "~eAIPatrol");
		#endif

		if (!GetGame())
			return;

		int idx = m_AllPatrols.Find(this);
		if (idx != -1) m_AllPatrols.RemoveOrdered(idx);

		Stop();
	}
	
	static void DebugAll()
	{
		Print("DebugAll");
		Print(m_AllPatrols.Count());
		foreach (auto patrol : m_AllPatrols)
		{
			patrol.Debug();
		}
	}
	
	static void DeleteAll()
	{
		m_AllPatrols.Clear();
	}
	
	void Debug()
	{
		Print(Type());
		Print(m_Timer);
	}
	/**
	 * @brief Destroys this patrol on the next frame
	 */
	void Delete()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Delete");
		#endif

		m_IsBeingDestroyed = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DeletePatrol, this);
	}

	/**
	 * @brief Returns true if the patrol is being destroyed
	 */
	bool IsBeingDestroyed()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "IsBeingDestroyed");
		#endif

		return m_IsBeingDestroyed;
	}

	/**
	 * @brief Waits around a second and then calls OnUpdate at a frequency specified in UPDATE_RATE_IN_SECONDS in a new timer
	 */
	void Start()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Start");
		#endif

		//DelayedStart();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DelayedStart, Math.RandomInt(1, 1000), false);
	}

	private void DelayedStart()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "DelayedStart");
		#endif

		OnUpdate();

		if (!m_Timer) m_Timer = new Timer(CALL_CATEGORY_GAMEPLAY);
		m_Timer.Run(UPDATE_RATE_IN_SECONDS, this, "OnUpdate", null, true);
	}

	/**
	 * @brief Stops the timer and OnUpdate from being called.
	 */
	void Stop()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Stop");
		#endif

		if (m_Timer && m_Timer.IsRunning()) m_Timer.Stop();
	}

	/**
	 * @brief Abstract function. 
	 */
	void OnUpdate()
	{

	}
};
