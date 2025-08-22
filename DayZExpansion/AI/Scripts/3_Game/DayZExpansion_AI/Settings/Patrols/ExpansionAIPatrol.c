/**
 * ExpansionAIPatrol.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAIPatrol: ExpansionAIDynamicSpawnBase
{
	void ExpansionAIPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "West", string loa = "HumanLoadout", bool canbelooted = true, int unlimitedreload = 0, float chance = 1.0, float mindistradius = -1, float maxdistradius = -1, float respawntime = -2, float wprnd = 0, TVectorArray way = null)
	{
		RespawnTime = respawntime;
		if (wprnd < 0.0)
			MinSpreadRadius = 1;
		MaxSpreadRadius = Math.AbsFloat(wprnd);
		UseRandomWaypointAsStartPoint = true;
		Waypoints = way;
	}
};

//! Legacy
class ExpansionAIPatrol_V4: ExpansionAIPatrol
{
	string LoadoutFile;
};
