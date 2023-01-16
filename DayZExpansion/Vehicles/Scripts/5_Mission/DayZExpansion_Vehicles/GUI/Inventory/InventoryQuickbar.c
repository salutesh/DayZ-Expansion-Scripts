/**
 * InventoryQuickbar.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class InventoryQuickbar
{
	override int GetQuickbarItemColor(InventoryGrid grid, InventoryItem item)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		if (!player.GetInventory().IsInventoryLocked())
			return super.GetQuickbarItemColor(grid, item);

		EntityAI itemInHand = player.GetHumanInventory().GetEntityInHands();

		if (itemInHand == item)
			return InventoryGrid.ITEM_COLOR_QUICKBAR_H_BAD;

		return InventoryGrid.ITEM_COLOR_QUICKBAR_I_BAD;
	}
}
