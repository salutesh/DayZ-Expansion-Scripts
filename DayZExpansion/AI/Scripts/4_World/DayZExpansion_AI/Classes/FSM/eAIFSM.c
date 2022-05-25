class eAIFSM
{
	static const int EXIT = 0;
	static const int CONTINUE = 1;

	private autoptr array<ref eAIState> m_States;
	private autoptr array<ref eAITransition> m_Transitions;

	private eAIState m_CurrentState;
	private eAIState m_ParentState;

	protected string m_Name;
	protected string m_DefaultState;
	protected eAIBase m_Unit;

	void eAIFSM(eAIBase unit, eAIState parentState)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "eAIFSM").Add(unit).Add(parentState);
		#endif

		m_Unit = unit;
		m_ParentState = parentState;

		m_States = new array<ref eAIState>();
		m_Transitions = new array<ref eAITransition>();
	}

	#ifdef CF_DEBUG
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		int i;

		instance.Add("Active State", m_CurrentState);
		if (m_CurrentState)
		{
			instance.IncrementTab();
			instance.Add(m_CurrentState);
			instance.DecrementTab();
		}

		instance.Add("Transitions", m_Transitions.Count());
		for (i = 0; i < m_Transitions.Count(); i++)
			instance.Add(m_Transitions[i]);

		return false;
	}
	#endif

	string GetName()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetName");
		#endif

		return m_Name;
	}

	eAIBase GetUnit()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetUnit");
		#endif

		return m_Unit;
	}

	eAIState GetParent()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetParent");
		#endif

		return m_ParentState;
	}

	void AddState(eAIState state)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "AddState").Add(state);
		#endif

		m_States.Insert(state);
	}

	void AddTransition(eAITransition transition)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "AddTransition").Add(transition);
		#endif

		m_Transitions.Insert(transition);
	}

	void SortTransitions()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SortTransitions");
		#endif

		//TODO: if the source transition is null, push to the back of the array
		//TODO: if the destination transition is null, remove from the array
	}
	
	eAIState GetState()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetState");
		#endif

		return m_CurrentState;
	}

	eAIState GetState(string type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetState").Add(type);
		#endif

		for (int i = 0; i < m_States.Count(); i++) if (m_States[i].ClassName() == type) return m_States[i];

		return null;
	}

	eAIState GetState(typename type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetState").Add(type.ToString());
		#endif

		for (int i = 0; i < m_States.Count(); i++) if (m_States[i].Type() == type) return m_States[i];

		return null;
	}
	
	bool StartDefault()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		if (m_DefaultState == "") return Start();

		eAIState src = m_CurrentState;
		eAIState dst = GetState(m_DefaultState);

		if (src)
		{
			EXTrace.Print(EXTrace.AI, m_Unit, "Exiting " + src);
			src.OnExit("", true, dst);
		}
	
		m_CurrentState = dst;
		
		if (m_CurrentState)
		{
			EXTrace.Print(EXTrace.AI, m_Unit, "Starting " + m_CurrentState);
			m_CurrentState.OnEntry("", src);
			return true;
		}
		
		CF_Log.Warn("No valid state found.");
		
		return false;
	}

	bool Start(string e = "")
	{
		auto trace = EXTrace.Start(EXTrace.AI, this, e);

		Param2<eAIState, bool> new_state = FindSuitableTransition(m_CurrentState, "");

		eAIState src = m_CurrentState;
		eAIState dst = new_state.param1;

		if (m_CurrentState && m_CurrentState != dst)
		{
			EXTrace.Print(EXTrace.AI, m_Unit, "Exiting " + m_CurrentState);
			m_CurrentState.OnExit(e, true, dst);
		}

		m_CurrentState = dst;

		if (m_CurrentState && src != m_CurrentState)
		{
			EXTrace.Print(EXTrace.AI, m_Unit, "Starting " + m_CurrentState);
			m_CurrentState.OnEntry(e, src);
			return true;
		}

		CF_Log.Warn("No valid state found.");

		return false;
	}

	bool Abort(string e = "")
	{
		auto trace = EXTrace.Start(EXTrace.AI, this, e);

		if (m_CurrentState)
		{
			EXTrace.Print(EXTrace.AI, m_Unit, "Aborting " + m_CurrentState);
			m_CurrentState.OnExit(e, true, null);
			return true;
		}

		return false;
	}

	/**
	 * @return EXIT Tell the parent FSM that the child FSM is complete
	 * @return CONTINUE Tell the parent FSM that the child FSM is still running
	 */
	int Update(float pDt, int pSimulationPrecision)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Update").Add(pDt).Add(pSimulationPrecision);
		#endif

		if (!m_Unit)
			return EXIT;

		//CF_Log.Debug("m_CurrentState: %1", "" + m_CurrentState);

		if (m_CurrentState && m_CurrentState.OnUpdate(pDt, pSimulationPrecision) == CONTINUE) return CONTINUE;

		Param2<eAIState, bool> new_state = FindSuitableTransition(m_CurrentState, "");
		if (!new_state.param2 || m_CurrentState == new_state.param1)
		{
			if (!m_CurrentState)
			{
				EXTrace.Print(EXTrace.AI, m_Unit, "Current state is NULL");
				return EXIT;
			}

			return CONTINUE;
		}

		eAIState src = m_CurrentState;

		EXTrace.Print(EXTrace.AI, m_Unit, "Exiting " + src);
		if (m_CurrentState) m_CurrentState.OnExit("", false, new_state.param1);

		m_CurrentState = new_state.param1;

		if (m_CurrentState == null)
		{
			return EXIT;
		}

		EXTrace.Print(EXTrace.AI, m_Unit, "Starting " + m_CurrentState);
		m_CurrentState.OnEntry("", src);

		return CONTINUE;
	}
	
	Param2<eAIState, bool> FindSuitableTransition(eAIState s, string e = "")
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "FindSuitableTransition").Add(s).Add(e);
		#endif

		// returns tuple as a valid destination can still be null

		//TODO: store a reference to the transitions inside the state for that state

		foreach (auto t: m_Transitions)
		{
			if ((t.GetSource() == s || t.GetSource() == null) && (e == "" || t.GetEvent() == e))
			{
				switch (t.Guard())
				{
				case eAITransition.SUCCESS:
#ifdef EAI_DEBUG_TRANSITION
					if (s != t.GetDestination())
						EXPrint(m_Unit.ToString() + " transition " + s + " -> " + t.GetDestination());
#endif
					return new Param2<eAIState, bool>(t.GetDestination(), true);
				case eAITransition.FAIL:
					break;
				}
			}
		}

		return new Param2<eAIState, bool>(null, false);
	}

	int ExitGuard(float DeltaTime = 0)
	{
		return EXIT;
	}
};
