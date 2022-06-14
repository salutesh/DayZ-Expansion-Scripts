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
	string EventName;           // Wreck_UH1Y, Wreck_Mi8, Wreck_PoliceCar, ContaminatedArea_Static, ContaminatedArea_Dynamic

	void ExpansionAICrashPatrol(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "PATROL", string fac = "WEST", string eventtype = "Wreck_UH1Y", string loa = "", float mindistradius = -2, float maxdistradius = -2, bool canbelooted = true, bool unlimitedreload = false, float chance = 1.0)
	{
		EventName = eventtype;
		LoadoutFile = loa;
		MinDistRadius = mindistradius;
		MaxDistRadius = maxdistradius;
		CanBeLooted = canbelooted;
		UnlimitedReload = unlimitedreload;
		Chance = chance;
	}

	//! TODO: Improve the spread code, this is very unoptimised
	vector GetStartPosition(vector position)
	{
		float minspread = 5;
		float maxspread = 10;
		if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
		{
			minspread = 0;
			maxspread = 50;
		}

		position = ExpansionMath.GetRandomPointInRing(position, minspread, maxspread);
		position = ExpansionStatic.GetSurfacePosition(position);

		return position;
	}

	//! TODO: Improve the spread code, this is very unoptimised
	ref TVectorArray GetWaypoints(vector position, int beh = eAIWaypointBehavior.HALT )
	{
		ref TVectorArray waypoints = new ref TVectorArray;
		vector waypoint;
		float minspread;
		float maxspread;
		if ( beh == eAIWaypointBehavior.HALT )
		{
			minspread = 5;
			maxspread = 15;
			if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
			{
				minspread = 0;
				maxspread = 50;
			}

			waypoint = ExpansionMath.GetRandomPointInRing(position, minspread, maxspread);
			waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

			waypoints.Insert(waypoint);
		} else {
			minspread = 5;
			maxspread = 20;
			int amountofwaypoints = Math.RandomIntInclusive(2, 6);
			if (EventName == "ContaminatedArea_Static" || EventName == "ContaminatedArea_Dynamic")
			{
				minspread = 0;
				maxspread = 150;
				amountofwaypoints = Math.RandomIntInclusive(4, 8);
			}
			
			for (int i = 0; i < amountofwaypoints; i++)
			{
				waypoint = ExpansionMath.GetRandomPointInRing(position, minspread, maxspread);
				waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

				waypoints.Insert(waypoint);
			}
		}

		return waypoints;
	}
};
