/**
 * ExpansionQuestAIGroup.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestAIGroup: ExpansionAISpawnBase
{
    ref TVectorArray Waypoints; // a list of positions to create a path to follow

    void ExpansionQuestAIGroup(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "PATROL", string fac = "WEST", string loa = "", bool canbelooted = true, bool unlimitedreload = false, TVectorArray way = null)
    {
        Waypoints = way;
    }
};
#endif