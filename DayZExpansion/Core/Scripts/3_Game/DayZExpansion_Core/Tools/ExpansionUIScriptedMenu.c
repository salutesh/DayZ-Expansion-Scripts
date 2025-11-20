/**
 * ExpansionUIScriptedMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionUIScriptedMenu: UIScriptedMenu
{	
	// ------------------------------------------------------------
	// ExpansionUIScriptedMenu OnShow
	// ------------------------------------------------------------	
	override void OnShow()
	{
		super.OnShow();
		
		ShowHud(false);
	}
	
	// ------------------------------------------------------------
	// ExpansionUIScriptedMenu OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
		
		ShowHud(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionUIScriptedMenu ShowHud
	// ------------------------------------------------------------
	void ShowHud(bool state)
	{
		Mission mission = g_Game.GetMission();
		
		if (!mission)
			return;

		Hud hud = mission.GetHud();
		if (!hud)
			return;
		
		hud.ShowHudUI(state);
		hud.ShowQuickbarUI(state);
	}
}