/**
 * ExpansionMarketModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMARKET
modded class ExpansionMarketModule
{	
	// ------------------------------------------------------------
	// Expansion Bool CanSellItem
	// ------------------------------------------------------------
	override bool CanSellItem(EntityAI item, bool checkIfRuined = false)
	{
		ItemBase itemBase;
		if (Class.CastTo(itemBase, item))
		{
			if (!itemBase.IsQuestItem())
				return false;
		}

		return super.CanSellItem(item, checkIfRuined);
	}
};
#endif