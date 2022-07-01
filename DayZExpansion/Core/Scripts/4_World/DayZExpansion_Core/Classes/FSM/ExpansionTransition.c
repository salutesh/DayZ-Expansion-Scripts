class ExpansionTransition
{
	static const int FAIL = 0;
	static const int SUCCESS = 1;

	protected string m_ClassName;

	void ExpansionTransition(ExpansionFSM _fsm)
	{
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

	ExpansionState GetSource()
	{
		return null;
	}

	ExpansionState GetDestination()
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
