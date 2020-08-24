class ExpansionActionStopPlayingGuitar: ActionSingleUseBase
{
	void ExpansionActionStopPlayingGuitar()
	{
		m_StanceMask = /* DayZPlayerConstants.STANCEMASK_CROUCH | */ DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		#ifdef DAYZ_1_9
		m_ConditionItem = new CCIPresent;
		#else
		m_ConditionItem = new CCINone;
		#endif
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Stop Playing Guitar";
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player.GetCommand_ExpansionGuitar() )
			return false;

		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.GetCommand_ExpansionGuitar().Stop();
	}
}