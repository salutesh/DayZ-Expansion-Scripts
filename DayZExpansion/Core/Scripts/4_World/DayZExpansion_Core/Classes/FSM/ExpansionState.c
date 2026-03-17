class ExpansionState
{
	static const int EXIT = 0;
	static const int CONTINUE = 1;
	static const int RESTART = 2;

	string m_Name;
	string m_ClassName;

	//! only used if there is a sub-fsm
	ref ExpansionFSM m_SubFSM;

	autoptr array<ref ExpansionTransition> m_Transitions = {};

	/* STATE VARIABLES */
	ExpansionState parent;

	void ExpansionState(ExpansionFSM _fsm)
	{
		parent = _fsm.GetParent();
	}

	void AddTransition(ExpansionTransition transition)
	{
		m_Transitions.Insert(transition);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.FSM, this, "AddTransition -> " + transition.GetDestination() + " | count: " + m_Transitions.Count());
	#endif
	}

	#ifdef CF_DEBUG
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		int i;

		instance.Add("FSM", m_SubFSM);
		if (m_SubFSM)
		{
			instance.IncrementTab();
			instance.Add(m_SubFSM);
			instance.DecrementTab();
		}

		return false;
	}
	#endif
	
	string GetName()
	{
		return m_Name;
	}

	ExpansionFSM GetSubFSM()
	{
		return m_SubFSM;
	}

	/* IMPLEMENTED IN XML */
	void OnEntry(string Event, ExpansionState From)
	{

	}

	void OnExit(string Event, bool Aborted, ExpansionState To)
	{

	}

	int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		return CONTINUE;
	}
};
