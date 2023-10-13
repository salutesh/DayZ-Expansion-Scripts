/**
 * ExpansionMarketMenuCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Remove sorting by display name
#ifdef EXPANSIONMODMARKET
modded class ExpansionMarketMenuCategory
{
    override void UpdateMarketItems()
    {
        m_TempDisplayNames = m_TempItems.GetKeyArray();

        int idx;
        int show;
        foreach (string tempDisplayName: m_TempDisplayNames)
        {
            foreach (ExpansionMarketItem tempItem: m_TempItems[tempDisplayName])
            {
                tempItem.m_Idx = idx++;
                if (m_MarketItems.Find(tempItem) == -1)
                    m_MarketItems.Insert(tempItem);
                if (tempItem.m_ShowInMenu && m_Category.m_Finalized)
                    show++;
            }
        }

        if (!IsUpdateTimerRunning() || !m_UpdateItemCount)
            UpdateItemCount(show);
    }
};
#endif