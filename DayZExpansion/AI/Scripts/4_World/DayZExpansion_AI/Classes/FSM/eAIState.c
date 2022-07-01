class eAIState : ExpansionState
{
	eAIBase unit;

	void eAIState(ExpansionFSM _fsm)
	{
		Class.CastTo(unit, _fsm.GetOwner());
	}
};
