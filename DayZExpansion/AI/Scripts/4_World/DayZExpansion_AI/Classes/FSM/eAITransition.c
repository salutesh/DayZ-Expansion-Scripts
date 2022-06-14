class eAITransition : ExpansionTransition
{
	eAIBase unit;

	void eAITransition(ExpansionFSM _fsm)
	{
		Class.CastTo(unit, _fsm.GetOwner());
	}
};
