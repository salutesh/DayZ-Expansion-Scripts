class ExpansionInputCarHorn : ContinuousDefaultActionInput
{	
	void ExpansionInputCarHorn( PlayerBase player )
	{
		SetInput( "UAExpansionCarHorn" );
		
		m_InputType = ActionInputType.AIT_CONTINUOUS;
		m_DetectFromTarget = true;
	}
};