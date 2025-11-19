modded class Environment
{
	float m_Expansion_TimeVisibility_Out[6] = {
		0,
		0.5,  //! sunrise start
		1,
		1,
		0.75,  //! sunset start
		0
	};

	float Expansion_GetDaylightVisibility()
	{
		int year, month, day, hour, minute;
		g_Game.GetWorld().GetDate(year, month, day, hour, minute);

		return Expansion_GetDaylightVisibilityEx(month, hour, minute);
	}

	float Expansion_GetDaylightVisibilityEx(int month, int hour, int minute)
	{
		float sunriseTimeStart = m_WorldData.GetApproxSunriseTime(month);
		float sunsetTimeStart = m_WorldData.GetApproxSunsetTime(month);

		float time[6] = {
			Math.Max(sunriseTimeStart - 0.5, 0.0),
			sunriseTimeStart,
			sunriseTimeStart + 0.5,
			sunsetTimeStart - 0.5,
			sunsetTimeStart,
			Math.Min(sunsetTimeStart + 1.0, 23.0 + 59.0 / 60.0)
		};

		return ExpansionMath.LookUp(hour + minute / 60.0, 6, time, m_Expansion_TimeVisibility_Out);
	}

	bool Expansion_IsUnderRoofBuilding()
	{
		return m_IsUnderRoofBuilding;
	}
}
