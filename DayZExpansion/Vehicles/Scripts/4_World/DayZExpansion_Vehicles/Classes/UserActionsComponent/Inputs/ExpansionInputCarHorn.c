class ExpansionInputCarHorn : ContinuousDefaultActionInput
{	
	void ExpansionInputCarHorn( PlayerBase player )
	{
		SetInput( "UAExpansionCarHorn" );
		
		m_InputType = ActionInputType.AIT_CONTINUOUS;
		m_DetectFromTarget = true;
	}
	
	/*override void UpdatePossibleActions( PlayerBase player, ActionTarget target, ItemBase item, int action_condition_mask )
	{
		super.UpdatePossibleActions( player, target, item, action_condition_mask );

		//if ( ForceActionCheck( player ) )
		//{
		//	m_SelectAction = m_ForcedActionData.m_Action;
		//	return;
		//}
	}*/
}