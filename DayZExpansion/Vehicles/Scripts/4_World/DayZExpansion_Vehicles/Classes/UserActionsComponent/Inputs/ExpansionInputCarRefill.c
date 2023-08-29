class ExpansionInputCarRefill : ContinuousDefaultActionInput
{	
	void ExpansionInputCarRefill( PlayerBase player )
	{
		SetInput( "UAExpansionCarRefill" );
		
		m_InputType = ActionInputType.AIT_CONTINUOUS;
		m_DetectFromItem = false;
	}
};
