modded class WorldData
{
	override void UpdateBaseEnvTemperature(float timeslice)
	{
		if (!m_EnTempUpdated)
		{
			if (m_Sunrise_Jan > 12 || m_Sunrise_Jan < 0)
				eAI_SunriseSunsetTimeError(m_Sunrise_Jan, "m_Sunrise_Jan", 5, 12);

			if (m_Sunset_Jan > 23.9833333 || m_Sunset_Jan <= m_Sunrise_Jan)
				eAI_SunriseSunsetTimeError(m_Sunset_Jan, "m_Sunset_Jan", 16, 23);

			if (m_Sunrise_Jul > 12 || m_Sunrise_Jul < 0)
				eAI_SunriseSunsetTimeError(m_Sunrise_Jul, "m_Sunrise_Jul", 5, 12);

			if (m_Sunset_Jul > 23.9833333 || m_Sunset_Jul <= m_Sunrise_Jul)
				eAI_SunriseSunsetTimeError(m_Sunset_Jul, "m_Sunset_Jul", 16, 23);
		}

		super.UpdateBaseEnvTemperature(timeslice);
	}

	void eAI_SunriseSunsetTimeError(float time, string varName, float min, float max)
	{
		string cls = string.Format("%1 (%2)", ClassName(), g_Game.GetWorldName());
		string msg = string.Format("Invalid time %1 for %2 (should be between %3 and %4)", time, varName, min, max);

		EXError.Error(this, string.Format("%1: %2", cls, msg), {});

		if (!g_Game.IsDedicatedServer())
			ExpansionNotification(cls, msg).Error();
	}
}
