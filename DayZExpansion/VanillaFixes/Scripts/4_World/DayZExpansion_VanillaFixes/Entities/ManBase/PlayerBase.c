/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	override void OnPlayerLoaded()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN, this);
#endif 
		
		//! Workaround for vanilla timing bug. It's basically chance if OnPlayerLoaded gets called before or after OnRespawnEvent is received on client,
		//! which has the effect that if a server is using respawnTime > 0 and OnRespawnEvent is received before OnPlayerLoaded is called,
		//! then the respawn timer screen may be closed before the countdown finishes due to the call to GetGame().GetUIManager().CloseAll() in OnPlayerLoaded.

		bool hasRespawnTimeScreen;

		LoginTimeBase loginTimeScreen = GetDayZGame().Expansion_GetLoginTimeScreen();
	#ifndef DAYZ_1_27
		//! 1.28+
		if (loginTimeScreen && loginTimeScreen.IsRespawn() && loginTimeScreen.IsVisible() && !loginTimeScreen.IsClosing())
	#else
		if (loginTimeScreen && loginTimeScreen.IsRespawn() && loginTimeScreen.IsVisible())
	#endif
		{
			hasRespawnTimeScreen = true;
		}

		super.OnPlayerLoaded();

		if (IsControlledPlayer() && m_Hud && hasRespawnTimeScreen)
		{
			GetDayZGame().Expansion_RestoreRespawnTimeScreen();
		}
	}
};
