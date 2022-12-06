/**
 * DayZPlayerImplementThrowing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplementThrowing
{	
	override void HandleThrowing(HumanInputController pHic, HumanCommandWeapons pHcw, EntityAI pEntityInHands, float pDt)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "HandleThrowing");
#endif

		if ( m_bThrowingModeEnabled )
		{
			PlayerBase player = PlayerBase.Cast(m_Player);
			if ( player && player.Expansion_IsInSafeZone() )
			{
				m_bThrowingModeEnabled = false;
				ResetState();
			
				pHcw.SetActionProgressParams( 0, 0 );
				pHcw.SetThrowingMode( false );
				
				return;
			}
		}

		super.HandleThrowing( pHic, pHcw, pEntityInHands, pDt );
	}
};
