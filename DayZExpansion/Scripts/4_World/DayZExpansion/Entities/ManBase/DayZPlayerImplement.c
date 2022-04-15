/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static bool m_ExpansionShowDeadScreen = false;

modded class DayZPlayerImplement
{
	//private ExpansionParachute m_ActiveParachute;
	
	// ------------------------------------------------------------
	// DayZPlayerImplement ShowDeadScreen
	// ------------------------------------------------------------	
	override void ShowDeadScreen(bool show, float duration)
	{
		super.ShowDeadScreen( show, duration );
		
		#ifndef NO_GUI		
		if ( show && IsPlayerSelected() )
		{
			GetGame().GetUIManager().ScreenFadeIn( duration, "#dayz_implement_dead", FadeColors.BLACK, FadeColors.WHITE );
		}
		else
		{
			GetGame().GetUIManager().ScreenFadeOut(0);
		}
		
		if (duration > 0)
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( StopDeathDarkeningEffect, duration*1000, false );
			
			if (GetExpansionSettings().GetGeneral().UseDeathScreen)
				m_ExpansionShowDeadScreen = true;
		}
		else
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( StopDeathDarkeningEffect );
			
			if (GetExpansionSettings().GetGeneral().UseDeathScreen)
				m_ExpansionShowDeadScreen = false;
		}
		#endif
	}
	
	override bool IsShootingFromCamera()
	{
		bool disableMagicCrosshair = GetExpansionSettings().GetGeneral().DisableMagicCrosshair; 
		return !disableMagicCrosshair;
	}
}
