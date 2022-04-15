/**
 * ExpansionDismantleFlagMode.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CraftRag
{
	override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		if (!super.CanDo(ingredients, player))
			return false;

		if (ingredients[1].IsInherited(Flag_Base) && ingredients[1].GetHierarchyParent())
		{
			TerritoryFlag flag;
			if (!Class.CastTo(flag, ingredients[1].GetHierarchyParent()) || !flag.HasExpansionTerritoryInformation())
				return true;
		}

		if (player.IsInTerritory() && !player.IsInsideOwnTerritory())
			return GetExpansionSettings().GetBaseBuilding().DismantleFlagMode == ExpansionDismantleFlagMode.AnyoneWithTools;

		return true;
	}
}
