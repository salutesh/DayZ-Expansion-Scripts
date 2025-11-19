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
	LoginTimeBase Expansion_GetLoginTimeScreen()
	{
		return m_LoginTimeScreen;
	}

	void Expansion_RestoreRespawnTimeScreen()
	{
		if ((!m_LoginTimeScreen || m_LoginTimeScreen.IsClosing()) && m_LoginTime > 0)
		{
			if (m_LoginTimeScreen)
			{
				UIMenuPanel parentUIMenuPanel = m_LoginTimeScreen.GetParentMenu();
				EXError.Info(this, "Respawn time screen is closing, entering new using parent " + parentUIMenuPanel);
				EnterLoginTime(parentUIMenuPanel);
			}
			else
			{
				UIScriptedMenu parentUIScriptedMenu = GetUIManager().GetMenu();
				EXError.Info(this, "Respawn time screen is null, entering new using parent " + parentUIScriptedMenu);
				EnterLoginTime(parentUIScriptedMenu);
			}
		
			m_LoginTimeScreen.SetRespawn(true);
			m_LoginTimeScreen.SetTime(m_LoginTime);
			m_LoginTimeScreen.Show();
		}
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
		return g_Game.GetInput().IsEnabledMouseAndKeyboardEvenOnServer();
		#else
		return true;
		#endif
	}

	private bool Expansion_UseKeyboard()
	{
		#ifdef PLATFORM_CONSOLE
		return g_Game.GetInput().IsEnabledMouseAndKeyboardEvenOnServer();
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
	#ifdef EXPANSIONUI
		auto scriptViewMenu = GetExpansionGame().GetExpansionUIManager().GetMenu();
	#else
		bool scriptViewMenu;
	#endif

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

	void Expansion_SuppressVanillaInvalidImpactSurfaceSpam(Object directHit, inout string surface, string ammoType)
	{
		if (!ImpactMaterials.GetImpactEffect(surface, ammoType))
		{
			if (directHit)
			{
				string object_type = directHit.GetType();
				
				if ( object_type == "" )
					object_type = "OBJECT_WITHOUT_CONFIG_CLASS";

				if (surface == "")
					EXError.ErrorOnce(this, string.Format("Object '%1' with model file: %2 has undefined 'Hit_...' material! Cannot play impact effect.", object_type, directHit.GetShapeName()), {});
				else
					EXError.WarnOnce(this, string.Format("Object '%1' with model file: %2 has unregistered surface impact material <%3>! Register this surface in ImpactMaterials (Script).", object_type, directHit.GetShapeName(), surface), {});
			}

			if (surface == "")
				surface = "Hit_ErrorNoMaterial";
			else
				surface = "Hit_Undefined";
		}
	}

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
		Expansion_SuppressVanillaInvalidImpactSurfaceSpam(directHit, surface, ammoType);

		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
	}

	override void CloseCombatEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 bool isWater, string ammoType) 
	{
		Expansion_SuppressVanillaInvalidImpactSurfaceSpam(directHit, surface, ammoType);

		super.CloseCombatEffects(source, directHit, componentIndex, surface, pos, surfNormal, isWater, ammoType);
	}
}
