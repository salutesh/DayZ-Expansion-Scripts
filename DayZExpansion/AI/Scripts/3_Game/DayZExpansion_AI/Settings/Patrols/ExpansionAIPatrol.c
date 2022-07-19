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
	float RespawnTime;	                // Time in seconds before the dead group will respawn. If set to -1, they won't respawn, if set to -2, will use the general setting instead
	autoptr TVectorArray Waypoints;     // a list of positions to create a path to follow

	void ExpansionAIPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "WEST", string loa = "HumanLoadout", bool canbelooted = true, bool unlimitedreload = false, float chance = 1.0, float mindistradius = -2, float maxdistradius = -2, float respawntime = -2, float wprnd = 0, TVectorArray way = null)
	{
		RespawnTime = respawntime;
		if (wprnd < 0.0)
			MinSpreadRadius = 1;
		MaxSpreadRadius = Math.AbsFloat(wprnd);
		Waypoints = way;
	}

	TVectorArray GetWaypoints()
	{
		TVectorArray waypoints();

		vector surfacePosition;
		foreach (vector waypoint: Waypoints)
		{
			if (MaxSpreadRadius > 0)
				waypoint = ExpansionMath.GetRandomPointInRing(waypoint, MinSpreadRadius, MaxSpreadRadius);
			surfacePosition = ExpansionStatic.GetSurfacePosition(waypoint);
			if (surfacePosition[1] > waypoint[1])
			{
				waypoint[1] = surfacePosition[1];
			}

			waypoints.Insert(waypoint);
		}

		if (WaypointInterpolation)
			return ExpansionMath.PathInterpolated(waypoints, typename.StringToEnum(ECurveType, WaypointInterpolation), MaxSpreadRadius > 0);

		return waypoints;
	}
};
