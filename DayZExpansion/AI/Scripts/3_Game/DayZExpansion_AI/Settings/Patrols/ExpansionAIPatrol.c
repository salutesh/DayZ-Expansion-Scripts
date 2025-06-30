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
	bool UseRandomWaypointAsStartPoint;
	autoptr TVectorArray Waypoints;     // a list of positions to create a path to follow

	void ExpansionAIPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "West", string loa = "HumanLoadout", bool canbelooted = true, int unlimitedreload = 0, float chance = 1.0, float mindistradius = -1, float maxdistradius = -1, float respawntime = -2, float wprnd = 0, TVectorArray way = null)
	{
		RespawnTime = respawntime;
		if (wprnd < 0.0)
			MinSpreadRadius = 1;
		MaxSpreadRadius = Math.AbsFloat(wprnd);
		UseRandomWaypointAsStartPoint = true;
		Waypoints = way;
	}

	override TVectorArray GetWaypoints(vector position = vector.Zero, int beh = eAIWaypointBehavior.HALT)
	{
		TVectorArray waypoints();

		vector surfacePosition;
		foreach (vector waypoint: Waypoints)
		{
			if (MaxSpreadRadius > 0)
				waypoint = ExpansionMath.GetRandomPointInRing(waypoint, MinSpreadRadius, MaxSpreadRadius);

			waypoints.Insert(GetPlacementPosition(waypoint));
		}

		if (WaypointInterpolation)
		{
			int curveType;

			if (!ExpansionStatic.StringToEnumEx(ECurveType, WaypointInterpolation, curveType))
			{
				EXError.Error(null, string.Format("WaypointInterpolation \"%1\" is not one of the valid values, \"CatmullRom\", \"NaturalCubic\" or \"UniformCubic\"", WaypointInterpolation), {});
			}
			else
			{
				bool smooth;

				if (MaxSpreadRadius > 0)
					smooth = true;

				return ExpansionMath.PathInterpolated(waypoints, curveType, smooth);
			}
		}

		return waypoints;
	}
};

//! Legacy
class ExpansionAIPatrol_V19: ExpansionAIPatrol
{
	string LoadoutFile;
};
