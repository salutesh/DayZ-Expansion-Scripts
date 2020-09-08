/**
 * DayZPlayerImplementThrowing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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