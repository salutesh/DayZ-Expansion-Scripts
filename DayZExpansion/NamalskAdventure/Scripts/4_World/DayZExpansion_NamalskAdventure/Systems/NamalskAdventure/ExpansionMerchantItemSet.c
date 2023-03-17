/**
 * ExpansionMerchantItemSet.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_NAMALSK_ADVENTURE
#ifdef NAMALSK_TERRAIN
#ifdef EXPANSIONMODMARKET
class ExpansionMerchantItemSet
{
	protected int m_ID = -1;
	protected ref map<string, int> m_Items = new map<string, int>;

	int GetID()
	{
		return m_ID;
	}

	map<string, int> GetItems()
	{
		return m_Items;
	}
};
class ExpansionMerchantItemSet_1: ExpansionMerchantItemSet
{
	void ExpansionMerchantItemSet_1()
	{
		m_ID = 1;
		m_Items.Insert("er7_gauss_magazine", 2);
		m_Items.Insert("ammo_er7rfw", 5);
	}
};
#endif
#endif
#endif