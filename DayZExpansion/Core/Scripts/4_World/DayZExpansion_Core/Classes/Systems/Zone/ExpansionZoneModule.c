/**
 * ExpansionZoneModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionZoneModule
 * @brief		
 **/
[CF_RegisterModule(ExpansionZoneModule)]
class ExpansionZoneModule : CF_ModuleWorld
{
	static const int COUNT = ExpansionZoneType.Count;

	int m_Interval = 100;
	int m_ActorsPerTick = 10;
	static bool s_ExEnabled = false;

	int m_TimeCounter = 0;

	override void OnInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnInit");
#endif

		super.OnInit();

		EnableMissionLoaded();
		EnableUpdate();
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		super.OnMissionLoaded(sender, args);

		auto settings = GetExpansionSettings().GetSafeZone();
		if (!settings)
			return;

		m_Interval = settings.FrameRateCheckSafeZoneInMs;
		m_ActorsPerTick = settings.ActorsPerTick;
		s_ExEnabled = settings.Enabled;

		bool failure = false;

		auto circleZones = settings.CircleZones;
		auto polygonZones = settings.PolygonZones;
		auto cylinderZones = settings.CylinderZones;

		if (!circleZones || !polygonZones || !cylinderZones)
		{
			failure = true;
		}

		if (circleZones)
		{
			foreach (ExpansionSafeZoneCircle circleZone : circleZones)
			{
				circleZone.Center[1] = 0;
				circleZone.Create();
			}
		}

		if (polygonZones)
		{
			foreach (ExpansionSafeZonePolygon polyZone : polygonZones)
			{
				polyZone.Create();
			}
		}

		if (cylinderZones)
		{
			foreach (ExpansionSafeZoneCylinder cylinderZone : cylinderZones)
			{
				cylinderZone.Create();
			}
		}

		int count = 0;

		ExpansionZone element = ExpansionZone.s_Head;
		while (element)
		{
			Print(element.ToStr());
			element = element.m_Next;
			count++;
		}

		Print("Found " + count + " zones!");

		if (failure)
		{
			Error("Zone setup failed.");
			return;
		}
	}

	override bool IsClient()
	{
		return false;
	}

	static bool IsInsideSafeZone(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, "ExpansionZoneModule", "IsInsideSafeZone");
#endif

		return IsInside(position, ExpansionZoneType.SAFE);
	}

	static bool IsInside(vector position, ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, "ExpansionZoneModule", "IsInside");
#endif

		if (type == ExpansionZoneType.SAFE && !s_ExEnabled)
			return false;

		for (int i = 0; i < COUNT; i++)
			ExpansionZone.s_InsideBuffer[i] = false;

		position[1] = 0;

		ExpansionZone element = ExpansionZone.s_Head;
		while (element)
		{
			element.Check(position);
			element = element.m_Next;
		}

		return ExpansionZone.s_InsideBuffer[type];
	}

	override void OnUpdate(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnUpdate");
#endif

		if (!s_ExEnabled)
			return;

		auto update = CF_EventUpdateArgs.Cast(args);

		m_TimeCounter += update.DeltaTime * 1000;
		if (m_TimeCounter < m_Interval)
			return;

		ExpansionZoneActor.UpdateAll(m_ActorsPerTick);

		m_TimeCounter = 0;
	}
};
