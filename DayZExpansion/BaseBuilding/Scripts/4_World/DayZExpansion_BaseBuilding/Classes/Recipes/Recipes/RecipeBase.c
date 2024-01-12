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
	protected bool m_ShowAdminPrefix;

	override string GetName()
	{
		if (m_ShowAdminPrefix)
			return "[ADMIN] " + m_Name;

		return m_Name;
	}

	override bool CheckRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		if (!super.CheckRecipe(item1, item2, player))
			return false;

		ItemBase ingredients[2];
		ingredients[0] = item1;
		ingredients[1] = item2;

		m_ShowAdminPrefix = false;

		for (int i = 0; i < 2; i++)
		{
			ItemBase ingredient = ingredients[i];
			if (ingredient.IsInherited(Flag_Base))
			{
				TerritoryFlag flag;
				if (Class.CastTo(flag, ingredient.GetHierarchyParent()) && flag.HasExpansionTerritoryInformation())
				{
					bool isInsideOwnTerritory;
					bool canDismantle = player.Expansion_CanDismantleFlag(isInsideOwnTerritory);
				#ifndef SERVER
					if (isInsideOwnTerritory && GetPermissionsManager().IsAdminToolsToggledOn())
						m_ShowAdminPrefix = true;
				#endif
					return canDismantle;
				}
			}
		}

		return true;
	}
}
