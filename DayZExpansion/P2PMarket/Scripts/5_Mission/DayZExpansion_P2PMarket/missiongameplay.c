/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionGameplay
{
	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);

		if (input.LocalPress("UAExpansionConfirm", false) && viewMenu)
		{
			ExpansionP2PMarketMenu p2pMarketMenu;
			if (Class.CastTo(p2pMarketMenu, viewMenu))
			{
				EditBoxWidget ew = EditBoxWidget.Cast(GetFocus());
				if (ew && ew == p2pMarketMenu.GetSearchFilterBox())
				{
					p2pMarketMenu.OnSearchFilterChange();
				}
			}
		}
	}
}
