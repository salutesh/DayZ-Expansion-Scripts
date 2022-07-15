/**
 * ExpansionAIObjectPatrol.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAIObjectPatrol: ExpansionAIDynamicSpawnBase
{
	string ClassName;           // Any valid BuildingBase type

	void ExpansionAIObjectPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "WEST", string loa = "", bool canbelooted = true, bool unlimitedreload = false, float chance = 1.0, float mindistradius = -2, float maxdistradius = -2, string classname = "Wreck_UH1Y")
	{
		ClassName = classname;
		DefaultSpread();
	}

	//! TODO: Improve the spread code, this is very unoptimised
	void DefaultSpread()
	{
		if (Behaviour == "HALT")
		{
			if (ClassName == "ContaminatedArea_Static" || ClassName == "ContaminatedArea_Dynamic")
			{
				MinSpreadRadius = 0;
				MaxSpreadRadius = 50;
			}
			else
			{
				MinSpreadRadius = 5;
				MaxSpreadRadius = 10;
			}
		}
		else
		{
			if (ClassName == "ContaminatedArea_Static" || ClassName == "ContaminatedArea_Dynamic")
			{
				MinSpreadRadius = 0;
				MaxSpreadRadius = 150;
			}
			else
			{
				MinSpreadRadius = 5;
				MaxSpreadRadius = 20;
			}
		}
	}

	TVectorArray GetWaypoints(vector position, int beh = eAIWaypointBehavior.HALT )
	{
		TVectorArray waypoints = new TVectorArray;
		vector waypoint;
		int amountofwaypoints;

		if (beh == eAIWaypointBehavior.HALT)
		{
			amountofwaypoints = 1;
		}
		else
		{
			if (ClassName == "ContaminatedArea_Static" || ClassName == "ContaminatedArea_Dynamic")
				amountofwaypoints = Math.RandomIntInclusive(4, 8);
			else
				amountofwaypoints = Math.RandomIntInclusive(2, 6);
		}

		for (int i = 0; i < amountofwaypoints; i++)
		{
			waypoint = ExpansionMath.GetRandomPointInRing(position, MinSpreadRadius, MaxSpreadRadius);
			waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

			waypoints.Insert(waypoint);
		}

		if (WaypointInterpolation)
			return PathInterpolated(waypoints, typename.StringToEnum(ECurveType, WaypointInterpolation));

		return waypoints;
	}
};

//! Legacy
class ExpansionAICrashPatrol: ExpansionAIObjectPatrol
{
	string EventName;
};