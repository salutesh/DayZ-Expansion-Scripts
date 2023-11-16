/**
 * CutOutPumpkinSeeds.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CutOutPumpkinSeeds
{
	override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		if (!super.CanDo(ingredients, player))
			return false;

		ItemBase pumpkin = ingredients[0];
		if (pumpkin && pumpkin.Expansion_IsQuestItem())
			return false;

		return true;
	}
};
