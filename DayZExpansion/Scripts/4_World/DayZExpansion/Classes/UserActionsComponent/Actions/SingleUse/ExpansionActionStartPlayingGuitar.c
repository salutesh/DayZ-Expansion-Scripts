class ExpansionActionStartPlayingGuitar: ActionSingleUseBase
{
	void ExpansionActionStartPlayingGuitar()
	{
		m_StanceMask = /* DayZPlayerConstants.STANCEMASK_CROUCH | */ DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCIPresent;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Start Playing Guitar";
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Expansion_Guitar guitar;
		if ( !Class.CastTo( guitar, item ) )
			return false;

		if ( player.GetCommand_Vehicle() )
			return false;

		if ( player.GetCommand_ExpansionGuitar() )
			return false;

		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.StartCommand_ExpansionGuitar( Expansion_Guitar.Cast( action_data.m_MainItem ) );
	}
}