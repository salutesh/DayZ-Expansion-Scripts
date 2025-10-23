class eAIState : ExpansionState
{
	/**
	 * TODO: Explore possibility of common checks being done only once per FSM update loop.
	 * A set of bitmasks would need to be computed which the individual transitions then
	 * check against with a single `if` statement.
	 * E.g.
	 *
	 * FSM::Update
	 * int stateMask;
	 * stateMask |= unit.IsFighting() * eAIState.IS_FIGHTING;
	 * stateMask |= unit.IsRestrained() * eAIState.IS_RESTRAINED;
	 * stateMask |= unit.IsUnjonscious() * eAIState.IS_UNCONSCIOUS;
	 * stateMask |= unit.IsSwimming() * eAIState.IS_SWIMMING;
	 * stateMask |= unit.eAI_IsChangingStance() * eAIState.IS_CHANGING_STANCE;
	 * stateMask |= unit.IsRaised() * eAIState.IS_RAISED;
	 * stateMask |= unit.GetWeaponManager().IsRunning() * eAIState.IS_WPNMGR_RUNNING;
	 * stateMask |= (bool)unit.GetActionManager().GetRunningAction() * eAIState.IS_ACTION_RUNNING;
	 * stateMask |= !(bool)unit.GetCommand_MoveAI() * eAIState.IS_NOT_IN_MOVE_CMD;
	 *
	 * Transition::Guard
	 * int failureMask;
	 * failureMask |= eAIState.IS_FIGHTING;
	 * failureMask |= eAIState.IS_RESTRAINED;
	 * failureMask |= eAIState.IS_UNCONSCIOUS;
	 * failureMask |= eAIState.IS_SWIMMING;
	 * if (stateMask & failureMask) return FAIL;
	 */
	static const int IS_FIGHTING = 1;
	static const int IS_RESTRAINED = 2;
	static const int IS_UNCONSCIOUS = 4;
	static const int IS_SWIMMING = 8;
	static const int IS_CHANGING_STANCE = 16;
	static const int IS_RAISED = 32;
	static const int IS_WPNMGR_RUNNING = 64;
	static const int IS_ACTION_RUNNING = 128;
	static const int IS_NOT_IN_MOVE_CMD = 256;

	eAIBase unit;

	void eAIState(ExpansionFSM _fsm)
	{
		Class.CastTo(unit, _fsm.GetOwner());
	}
};
