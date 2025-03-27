modded class Environment
{
	static float s_Expansion_Visibilty_In[5] = {
		0.0,
		0.25,
		0.5,
		0.75,
		1.0
	};

	//! At 25% fog, visibility is around 900 m
	//! At 50% fog, visibility is around 440 m
	//! At 75% fog, visibility is around 250 m
	//! At 100% fog, visibility is around 45 m
	static float s_Expansion_FogVisibilty_Out[5] = {
		1.000,
		0.900,
		0.440,
		0.250,
		0.045
	};

	static void Expansion_GetWeatherVisibility(float fog, float overcast, float rain, float snow, out float fogVisibility, out float overcastVisibility, out float rainVisibility, out float snowVisibility)
	{
		fogVisibility = ExpansionMath.LookUp(fog, 5, s_Expansion_Visibilty_In, s_Expansion_FogVisibilty_Out);
		overcastVisibility = ExpansionMath.LinearConversion(0.75 - Math.Min(rain, 0.75), 1.0, overcast, 1.0, 0.5 * fogVisibility);
		rainVisibility = ExpansionMath.LinearConversion(0.5, 1.0, rain, 1.0, 0.33);
		snowVisibility = ExpansionMath.LinearConversion(0.5, 1.0, snow, 1.0, 0.33);
	}

	static void Expansion_GetWeatherVisibilityCurrent(out float fogVisibility, out float overcastVisibility, out float rainVisibility, out float snowVisibility)
	{
		Weather weather	= g_Game.GetWeather();

		float fog = weather.GetFog().GetActual();
		float overcast = weather.GetOvercast().GetActual();
		float rain = weather.GetRain().GetActual();
		float snow = weather.GetSnowfall().GetActual();

		Expansion_GetWeatherVisibility(fog, overcast, rain, snow, fogVisibility, overcastVisibility, rainVisibility, snowVisibility);
	}

	void Expansion_GetWeatherVisibility(out float fogVisibility, out float overcastVisibility, out float rainVisibility, out float snowVisibility)
	{
		Expansion_GetWeatherVisibility(m_Fog, m_Clouds, m_Rain, m_Snowfall, fogVisibility, overcastVisibility, rainVisibility, snowVisibility);
	}
}
