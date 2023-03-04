/**
 * ExpansionQuesItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestItemBase: ItemBase
{
	void ExpansionQuestItemBase()
	{
		m_CanBeSkinned = false;

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			SetAllowDamage(false);
	}
};

class ExpansionQuestItemPaper: ExpansionQuestItemBase {};
class ExpansionQuestItemPackage: ExpansionQuestItemBase {};