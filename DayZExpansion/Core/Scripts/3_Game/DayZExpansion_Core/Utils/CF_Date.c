/**
 * CF_Date.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CF_Date
{
    //! @note 0 = sunday
    int GetDayOfWeek()
    {
        int y = m_Year;
        int d = m_Day;
        if (m_Month < 3)
            d += y--;
        else
            d += y - 2;
        return (23 * m_Month / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;
    }
}
