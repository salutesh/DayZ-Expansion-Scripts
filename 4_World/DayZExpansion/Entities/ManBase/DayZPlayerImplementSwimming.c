modded class DayZPlayerImplementSwimming
{
	private bool m_WasCalledCurrentCommand;
	private bool m_LastReturn;

	void ResetCall()
	{
		m_WasCalledCurrentCommand = false;
	}

	override bool HandleSwimming( int pCurrentCommandID, HumanCommandMove pCMove, HumanMovementState pState )
	{
		if ( m_WasCalledCurrentCommand )
			return m_LastReturn;

		m_LastReturn = super.HandleSwimming( pCurrentCommandID, pCMove, pState );
		m_WasCalledCurrentCommand = true;
		return m_LastReturn;
	}
}