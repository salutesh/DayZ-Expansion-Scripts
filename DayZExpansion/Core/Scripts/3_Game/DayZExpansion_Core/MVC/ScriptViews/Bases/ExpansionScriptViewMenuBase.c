/**
 * ExpansionScriptViewMenuBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionScriptViewMenuBase: ExpansionScriptViewBase
{
	void LockControls(bool lockMovement = true);
	void UnlockControls();
	void LockInputs(bool state, bool lockMovement = true);
	void UnlockInputs();
	void ShowHud(bool state);
	void ShowUICursor(bool state);
	
	override bool CanShow()
	{
		return true;
	}

	bool UseMouse()
	{
		#ifdef PLATFORM_CONSOLE
		return GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer();
		#else
		return true;
		#endif
	}
};