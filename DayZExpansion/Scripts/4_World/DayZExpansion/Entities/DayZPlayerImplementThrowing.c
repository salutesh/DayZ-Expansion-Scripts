modded class DayZPlayerImplementThrowing
{	
	override void HandleThrowing(HumanInputController pHic, HumanCommandWeapons pHcw, EntityAI pEntityInHands, float pDt)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplementThrowing::HandleThrowing Start");
		#endif
		
		if ( m_bThrowingModeEnabled )
		{
			PlayerBase player = PlayerBase.Cast(m_Player);
			if ( player && player.IsInSafeZone() )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("DayZPlayerImplementThrowing::HandleThrowing End");
				#endif

				m_bThrowingModeEnabled = false;
				ResetState();
			
				pHcw.SetActionProgressParams( 0, 0 );
				pHcw.SetThrowingMode( false );
				
				return;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplementThrowing::HandleThrowing End");
		#endif

		super.HandleThrowing( pHic, pHcw, pEntityInHands, pDt );
	}
};