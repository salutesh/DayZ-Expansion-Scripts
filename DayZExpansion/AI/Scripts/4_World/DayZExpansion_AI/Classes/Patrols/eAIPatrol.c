class eAIPatrol : Managed
{
	protected static autoptr array<ref eAIPatrol> s_AllPatrols = new array<ref eAIPatrol>();
	private static int s_NextID;
	static const float UPDATE_RATE_IN_SECONDS = 5.0;
	
	private ref Timer m_Timer;
	private bool m_IsBeingDestroyed;

	protected int m_ID;

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

		int index = s_AllPatrols.Find(patrol);
		s_AllPatrols.Remove(index);
	}

	/**
	 * @brief Privated constructor to prevent calling/storing in ref. The instance is managed through 'Create(X)Patrol' and 'DeletePatrol'
	 */
	private void eAIPatrol()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIPatrol");
		#endif

		s_AllPatrols.Insert(this);
		m_ID = s_NextID++;
	}

	/**
	 * @brief Privated destructor to prevent calling/storing in ref. The instance is managed through 'Create(X)Patrol' and 'DeletePatrol'
	 */
	private void ~eAIPatrol()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "~eAIPatrol");
		#endif

		if (!g_Game)
			return;

		int idx = s_AllPatrols.Find(this);
		if (idx != -1) s_AllPatrols.RemoveOrdered(idx);

		Stop();
	}
	
	static void DebugAll()
	{
		Print("=== eAIPatrol::DebugAll =========================================");

		int allPatrols_Count = s_AllPatrols.Count();
		Print(allPatrols_Count);

		eAIDynamicPatrol.OnDebugAll();

		foreach (auto patrol : s_AllPatrols)
		{
			patrol.Debug();
		}

		Print("=================================================================");
	}

	static void DeleteAll()
	{
		s_AllPatrols.Clear();
	}
	
	void Debug()
	{
		string title = string.Format("--- %1::Debug ", this);
		for (int i = title.Length(); i < 65; i++) title += "-";
		PrintFormat(title);
		Print(m_Timer);
		Print(m_ID);
	}

	void LoadBalancing_Update()
	{
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
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DeletePatrol, this);
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
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DelayedStart, Math.RandomInt(1, 1000), false);
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
