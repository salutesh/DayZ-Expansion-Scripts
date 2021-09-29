/**
 * ExpansionZoneModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionZoneModule
 * @brief		
 **/
class ExpansionZoneModule : JMModuleBase
{
	static const int COUNT = ExpansionZoneType.Count;

	private int m_Interval = 100;
	private int m_ActorsPerTick = 10;
	private static bool s_ExEnabled = false;

	private int m_TimeCounter = 0;

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		auto settings = GetExpansionSettings().GetSafeZone();
		if (!settings)
			return;

		m_Interval = settings.FrameRateCheckSafeZoneInMs;
		m_ActorsPerTick = 100; // settings.ActorsPerTicks;
		s_ExEnabled = settings.Enabled;

		bool failure = false;

		auto circleZones = settings.CircleZones;
		auto polygonZones = settings.PolygonZones;

		if (!circleZones || !polygonZones)
		{
			failure = true;
		}

		failure &= circleZones != null;
		if (circleZones)
		{
			foreach (ExpansionSafeZoneCircle circleZone : circleZones)
			{
				circleZone.Center[1] = 0;
				circleZone.Create();
			}
		}

		failure &= polygonZones != null;
		if (polygonZones)
		{
			int index = 0;
			foreach (ExpansionSafeZonePolygon polyZone : polygonZones)
			{
				polyZone.Create();
				index++;
			}
		}

		int count = 0;

		ExpansionZone element = g_ExpansionZoneHead;
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
		return IsInside(position, ExpansionZoneType.SAFE);
	}

	static bool IsInside(vector position, ExpansionZoneType type)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZoneModule::IsInside start");
#endif

		if (type == ExpansionZoneType.SAFE && !s_ExEnabled)
			return false;

		for (int i = 0; i < COUNT; i++)
			ExpansionZone.s_InsideBuffer[i] = false;

		position[1] = 0;

		ExpansionZone element = g_ExpansionZoneHead;
		while (element)
		{
			element.Check(position);
			element = element.m_Next;
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZoneModule::IsInside end");
#endif

		return ExpansionZone.s_InsideBuffer[type];
	}

	override void OnUpdate(float timeslice)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZoneModule::OnUpdate");
#endif

		if (!s_ExEnabled)
			return;

		m_TimeCounter += timeslice * 1000;

		if (m_TimeCounter < m_Interval)
			return;

		m_TimeCounter = 0;

		ExpansionZoneActor.UpdateAll(m_ActorsPerTick);
	}
};
