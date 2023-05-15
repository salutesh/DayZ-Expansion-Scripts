/**
 * RecipeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Disable any recipes involving attached flags if in enemy territory
modded class RecipeBase
{
	override bool CheckRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		if (!super.CheckRecipe(item1, item2, player))
			return false;

		ItemBase ingredients[2];
		ingredients[0] = item1;
		ingredients[1] = item2;

		for (int i = 0; i < 2; i++)
		{
			ItemBase ingredient = ingredients[i];
			if (ingredient.IsInherited(Flag_Base))
			{
				TerritoryFlag flag;
				if (Class.CastTo(flag, ingredient.GetHierarchyParent()) && flag.HasExpansionTerritoryInformation())
					return Expansion_CanDismantle(player);
			}
		}

		return true;
	}

	bool Expansion_CanDismantle(PlayerBase player)
	{
		if (player.IsInTerritory() && !player.IsInsideOwnTerritory())
			return GetExpansionSettings().GetBaseBuilding().DismantleFlagMode > ExpansionDismantleFlagMode.TerritoryMembersWithHands;

		return true;
	}
}
