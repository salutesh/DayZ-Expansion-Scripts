/**
 * DayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZGame
{
	//! Workaround for respawn timer closing prematurely (see accompanying changes to OnPlayerLoaded in 4_World/DayZExpansion_VanillaFixes/PlayerBase.c)
	LoginTimeBase GetLoginTimeScreen()
	{
		return m_LoginTimeScreen;
	}

	#ifndef SERVER
	//! Fix for vehicle HUD being visible after connect due to vanilla sometimes not properly handling closing of login timer UI
	override void CancelLoginTimeCountdown()
	{
		super.CancelLoginTimeCountdown();

		auto menu = GetUIManager().GetMenu();
		if (!menu)
			return;

		auto loginTime = LoginTimeBase.Cast(menu);
		if (!loginTime)
			return;

		EXLogPrint("Closing " + loginTime);

		if (loginTime.IsStatic())
		{
			loginTime.Hide();
			delete loginTime;
		}
		else
		{
			loginTime.Close();
		}

		Expansion_UnlockControls();
	}
	#endif

	private bool Expansion_UseMouse()
	{
		#ifdef PLATFORM_CONSOLE
		return GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer();
		#else
		return true;
		#endif
	}

	private bool Expansion_UseKeyboard()
	{
		#ifdef PLATFORM_CONSOLE
		return GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer();
		#else
		return true;
		#endif
	}

	private bool Expansion_UseGamepad()
	{
		return true;
	}

	private void Expansion_UnlockControls()
	{
		if (Expansion_UseMouse())
		{
			GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
		}

		auto menu = GetUIManager().GetMenu();
		auto scriptViewMenu = GetExpansionGame().GetExpansionUIManager().GetMenu();

		if ((menu && menu.UseMouse()) || scriptViewMenu)
		{
			GetUIManager().ShowUICursor(true);
		}
		else
		{
			GetUIManager().ShowUICursor(false);
		}

		if (Expansion_UseKeyboard())
		{
			GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
		}
		
		if (Expansion_UseGamepad())
		{
			GetInput().ChangeGameFocus(-1, INPUT_DEVICE_GAMEPAD);
		}
	}
}
