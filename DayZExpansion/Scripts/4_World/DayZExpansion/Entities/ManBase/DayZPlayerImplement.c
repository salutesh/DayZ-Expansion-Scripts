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

static bool g_ExpansionShowDeadScreen = false;

modded class DayZPlayerImplement
{
	// ------------------------------------------------------------
	// DayZPlayerImplement ShowDeadScreen
	// ------------------------------------------------------------
	override void ShowDeadScreen(bool show, float duration)
	{
		super.ShowDeadScreen( show, duration );

		#ifndef NO_GUI
		if (!GetExpansionSettings().GetGeneral(false).IsLoaded())
			return;

		if (duration > 0)
		{
			if (GetExpansionSettings().GetGeneral().UseDeathScreen)
				g_ExpansionShowDeadScreen = true;
		}
		else
		{
			if (GetExpansionSettings().GetGeneral().UseDeathScreen)
				g_ExpansionShowDeadScreen = false;
		}
		#endif
	}

	override bool IsShootingFromCamera()
	{
		if (GetExpansionSettings().GetGeneral(false).IsLoaded() && GetExpansionSettings().GetGeneral().DisableMagicCrosshair)
			return false;
		else
			return true;
	}
}
