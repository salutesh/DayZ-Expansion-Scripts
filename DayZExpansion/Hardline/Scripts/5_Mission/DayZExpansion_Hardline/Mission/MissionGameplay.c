/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionGameplay
{
	ref ExpansionHardlineHUD m_ExpansionHardlineHUD;

	// ------------------------------------------------------------
	// OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();
		
		//! Expansion Hardline Hud
		InitExpansionHardlineHud();
	}

	// ------------------------------------------------------------
	// InitExpansionHardlineHud
	// ------------------------------------------------------------
	void InitExpansionHardlineHud()
	{
		if (!m_ExpansionHardlineHUD)
		{
			m_ExpansionHardlineHUD = new ExpansionHardlineHUD(m_Hud);
		}
	}

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		InventoryMenu inventoryMenu;
		if (viewMenu || (menu && !Class.CastTo(inventoryMenu, menu)) || m_Hud.IsHideHudPlayer())
		{
			m_ExpansionHardlineHUD.ShowHud(false);
		}
		else
		{
			m_ExpansionHardlineHUD.ShowHud(true);
		}
	}
};