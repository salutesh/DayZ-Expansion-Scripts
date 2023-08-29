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
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN, this);

		//! Workaround for vanilla timing bug. It's basically chance if OnPlayerLoaded gets called before or after OnRespawnEvent is received on client,
		//! which has the effect that if a server is using respawnTime > 0 and OnRespawnEvent is received before OnPlayerLoaded is called,
		//! then the respawn timer screen may be closed before the countdown finishes due to the call to GetGame().GetUIManager().CloseAll() in OnPlayerLoaded.

		Hud hud;
		LoginTimeBase loginTimeScreen = GetDayZGame().GetLoginTimeScreen();
		if (loginTimeScreen && loginTimeScreen.IsRespawn() && loginTimeScreen.IsVisible())
		{
			hud = m_Hud;
			m_Hud = null;
		}

		super.OnPlayerLoaded();

		if (hud)
		{
			m_Hud = hud;

			//! The following is almost equivalent to the relevant part in vanilla OnPlayerLoaded,
			//! EXCEPT we DON'T call GetGame().GetUIManager().CloseAll()
			if (IsControlledPlayer())
			{
				m_Hud.UpdateBloodName();
				PPERequesterBank.GetRequester(PPERequester_DeathDarkening).Stop();
				PPERequesterBank.GetRequester(PPERequester_ShockHitReaction).Stop();
				PPERequesterBank.GetRequester(PPERequester_UnconEffects).Stop();
				//GetGame().GetUIManager().CloseAll();
				GetGame().GetMission().SetPlayerRespawning(false);
				GetGame().GetMission().OnPlayerRespawned(this);
				
				m_Hud.ShowHudUI( true );
				m_Hud.ShowQuickbarUI(true);
				#ifdef PLATFORM_CONSOLE
				m_Hud.ShowQuickBar(GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer()); //temporary solution
				#else
				m_Hud.ShowQuickBar(g_Game.GetProfileOption(EDayZProfilesOptions.QUICKBAR));
				#endif
			}
		}
	}
};
