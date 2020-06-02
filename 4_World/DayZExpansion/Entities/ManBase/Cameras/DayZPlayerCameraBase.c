

modded class DayZPlayerCameraBase
{
    private DayZPlayerCameraResult m_LastResult;

    override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCameraBase::OnUpdate - Start");
		#endif
		
        super.OnUpdate( pDt, pOutResult );
        m_LastResult = pOutResult;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCameraBase::OnUpdate - End");
		#endif
    }

	DayZPlayerCameraResult ExpansionDayZPlayerCameraResult()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCameraBase::ExpansionDayZPlayerCameraResult - Return: " + m_LastResult.ToString() );
		#endif
        return m_LastResult;
    }
}