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

	override TVectorArray GetWaypoints(vector position = vector.Zero, int beh = eAIWaypointBehavior.HALT)
	{
		TVectorArray waypoints = new TVectorArray;
		vector waypoint;
		int amountofwaypoints;

		if (!MinSpreadRadius)
			MinSpreadRadius = 5;
		if (MaxSpreadRadius < MinSpreadRadius)
			MaxSpreadRadius = MinSpreadRadius + 5;

		if (ClassName == "ContaminatedArea_Static" || ClassName == "ContaminatedArea_Dynamic" || beh == eAIWaypointBehavior.HALT)
		{
			if (beh == eAIWaypointBehavior.HALT)
				amountofwaypoints = 1;
			else
				amountofwaypoints = Math.RandomIntInclusive(4, 8);

			for (int i = 0; i < amountofwaypoints; i++)
			{
				waypoint = ExpansionMath.GetRandomPointInRing(position, MinSpreadRadius, MaxSpreadRadius);
				waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

				waypoints.Insert(waypoint);
			}
		}
		else
		{
			//! For wrecks, we let AI follow a roughly circular pattern around the wreck so they don't run into it

			int ccw = Math.RandomIntInclusive(0, 1);
			float angle = Math.RandomFloat(0, 360);
			float angleIncCumulative;

			//! This will generate anywhere from 8 to 10 waypoints
			while (true)
			{
				waypoint = ExpansionMath.GetRandomPointAtDegrees(position, angle, MinSpreadRadius, MaxSpreadRadius);
				waypoint = ExpansionStatic.GetSurfacePosition(waypoint);

				waypoints.Insert(waypoint);

				float angleInc = Math.RandomFloat(36, 45);

				angleIncCumulative += angleInc;
				if (angleIncCumulative > 360)
					break;

				if (ccw)
				{
					angle -= angleInc;
					if (angle < 0)
						angle += 360;
				}
				else
				{
					angle += angleInc;
					if (angle > 360)
						angle -= 360;
				}
			}
		}

		if (WaypointInterpolation)
			return ExpansionMath.PathInterpolated(waypoints, typename.StringToEnum(ECurveType, WaypointInterpolation), MaxSpreadRadius > 0);

		return waypoints;
	}
};

//! Legacy
class ExpansionAICrashPatrol: ExpansionAIObjectPatrol
{
	string EventName;
};
