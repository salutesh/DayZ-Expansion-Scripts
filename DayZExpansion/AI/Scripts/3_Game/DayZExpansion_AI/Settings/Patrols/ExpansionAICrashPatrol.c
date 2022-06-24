/**
 * ExpansionAICrashPatrol.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAICrashPatrol: ExpansionAIPatrolBase
{
	string EventName;           // Any valid BuildingBase type

	void ExpansionAICrashPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "PATROL", string fac = "WEST", string eventtype = "Wreck_UH1Y", string loa = "", float mindistradius = -2, float maxdistradius = -2, bool canbelooted = true, bool unlimitedreload = false, float chance = 1.0)
	{
		EventName = eventtype;
		LoadoutFile = loa;
		MinDistRadius = mindistradius;
		MaxDistRadius = maxdistradius;
		DefaultSpread();
		CanBeLooted = canbelooted;
		UnlimitedReload = unlimitedreload;
		Chance = chance;
	}

	//! TODO: Improve the spread code, this is very unoptimised
	void DefaultSpread()
	{
		if (Behaviour == "HALT")
		{
			if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
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
			if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
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

	vector GetStartPosition(vector position)
	{
		position = ExpansionMath.GetRandomPointInRing(position, MinSpreadRadius, MaxSpreadRadius);
		position = ExpansionStatic.GetSurfacePosition(position);

		return position;
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
			if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
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

		return waypoints;
	}
};
