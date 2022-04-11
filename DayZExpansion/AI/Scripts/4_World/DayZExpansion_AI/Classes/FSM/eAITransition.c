class eAITransition
{
	static const int FAIL = 0;
	static const int SUCCESS = 1; 

	protected string m_ClassName;
	
	/* STATE VARIABLES */
	protected eAIBase unit;

	void eAITransition(eAIFSM _fsm, eAIBase _unit)
	{
		unit = _unit;
	}

	#ifdef CF_DEBUG
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		int i;

		instance.Add("Source", GetSource());
		instance.Add("Destination", GetDestination());
		instance.Add("Event", GetEvent());
		instance.Add("Guard", Guard());

		return false;
	}
	#endif

	eAIState GetSource()
	{
		return null;
	}

	eAIState GetDestination()
	{
		return null;
	}

	string GetEvent()
	{
		return "";
	}

	int Guard()
	{
		return SUCCESS;
	}
};
