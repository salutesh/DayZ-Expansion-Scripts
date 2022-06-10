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
class ExpansionQuestAIGroup
{
	string Faction;             // WEST, EAST, INSURGENT, CIVILIAN
    string LoadoutFile;         // a json file containing the loadout of this team
    int NumberOfAI;             // How many bots, -x will make it random between 0 and x
    string Behaviour;           // HALT, LOOP, REVERSE
    string Speed;               // WALK, JOG, SPRINT
    vector StartPos;            // where the group is going to spawn
    ref TVectorArray Waypoints; // a list of positions to create a path to follow

    void ExpansionQuestAIGroup(int bod, string spd, string beh, string fac, string loa, vector startpos, TVectorArray way)
    {
        NumberOfAI = bod;
        Speed = spd;
        Behaviour = beh;
        Faction = fac;
        LoadoutFile = loa;
        StartPos = startpos;
        Waypoints = way;
    }
};
#endif