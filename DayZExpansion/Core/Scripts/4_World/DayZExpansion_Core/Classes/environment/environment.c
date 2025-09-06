modded class Environment
{
	//! Normal fog
	//! -----------------------------------------------------------------------

	static float s_Expansion_Visibility_In[5] = {
		0.0,
		0.25,
		0.5,
		0.75,
		1.0
	};

	//! At 25% fog, visibility is around 900 m
	//! At 50% fog, visibility is around 445 m
	//! At 75% fog, visibility is around 250 m
	//! At 100% fog, visibility is around 44 m
	static float s_Expansion_FogVisibility_Out[5] = {
		1.000,
		0.900,
		0.445,
		0.250,
		0.044
	};

	//! Dynamic volumetric fog
	//! -----------------------------------------------------------------------

	static float s_Expansion_DistDensity_In[6] = {
		0,
		0.1,
		0.25,
		0.5,
		0.75,
		1
	};

	static float s_Expansion_DistDensity_Out[6] = {
		1.000,
		0.500,
		0.100,
		0.070,
		0.050,
		0.040
	};

	static float s_Expansion_HeightDiff_DistDensity0_In[9] = {
		0,
		30,
		40,
		50,
		60,
		70,
		80,
		90,
		100
	};

	//! Values for height density = 0
	static float s_Expansion_HeightDiff_DistDensity0_Out[9] = {
		1.000,
		0.900,
		0.300,
		0.100,
		0.045,
		0.020,
		0.0075,
		0.003,
		0.001
	};

	static float s_Expansion_HeightDiff_DistDensity1_In[9] = {
		-40,
		-30,
		-20,
		-10,
		0,
		10,
		20,
		30,
		40
	};

	//! Values for height density = 0
	static float s_Expansion_HeightDiff_DistDensity1_Out[9] = {
		1.000,
		0.900,
		0.300,
		0.100,
		0.045,
		0.020,
		0.0075,
		0.0025,
		0.0
	};

	static float s_Expansion_DistDensity_Scale_In[6] = {
		0,
		0.01,
		0.05,
		0.1,
		0.35,
		1
	};

	static float s_Expansion_HeightDiff_Scale_Out[6] = {
		1,
		0.166666,
		0.066666,
		0.033333,
		0.008333,
		0
	};

	//! -----------------------------------------------------------------------

	static void Expansion_GetWeatherVisibility(float fog, float overcast, float rain, float snow, out float fogVisibility, out float overcastVisibility, out float rainVisibility, out float snowVisibility)
	{
		fogVisibility = ExpansionMath.LookUp(fog, 5, s_Expansion_Visibility_In, s_Expansion_FogVisibility_Out);
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

	float Expansion_GetDynVolFogVisibility()
	{
		return Expansion_GetDynVolFogVisibilityEx(m_PlayerHeightPos);
	}

	static float Expansion_GetDynVolFogVisibilityEx(float height)
	{
		/** 
		 * Default values for dynamic fog
		 * 
		 * Chernarus: Height bias 50..200, distance density 0.01..0.05, height density 0.9..1
		 * Enoch: Height bias 155..170, distance density 0.01..0.05, height density 0.9..1
		 * Sakhal: Height bias 0, distance density 0.001..0.35, height density 1
		 * 
		 * Observations:
		 * 
		 * If distance density is 0 and height density is 1, there is no meaningful fog regardless of height bias.
		 * If both distance and height density are 1, height bias has no effect.
		 * If height density is 1, height bias has little to no effect and fog density is controlled by distance density.
		 * If height density is 0, height bias has full effect.
		 * 
		 * View distances for distance/height density = 0 (VERY rough visually determined estimates):
		 * 
		 * Height bias = current altitude - 20 m: No discernible fog
		 * Height bias = current altitude - 10 m: Faint fog on horizon
		 * Height bias = current altitude: Slightly less faint fog on horizon
		 * Height bias = current altitude + 10 m: Fog more clearly discernible but doesn't limit view distance yet
		 * Height bias = current altitude + 20 m: ~1000 m
		 * Height bias = current altitude + 30 m: ~900 m
		 * Height bias = current altitude + 40 m: ~300 m
		 * Height bias = current altitude + 50 m: ~100 m
		 * Height bias = current altitude + 60 m: ~50 m
		 * Height bias = current altitude + 70 m: ~20 m
		 * Height bias = current altitude + 80 m: ~7.5 m
		 * Height bias = current altitude + 90 m: ~3 m
		 * Height bias = current altitude + 100 m: ~1 m
		 * Height bias = current altitude + 110 m: ~0 m
		 * 
		 * View distances for distance density = 1 and height density = 0 (VERY rough visually determined estimates):
		 * 
		 * Height bias = current altitude - 80 m: No discernible fog
		 * Height bias = current altitude - 70 m: Faint fog on horizon
		 * Height bias = current altitude - 60 m: Slightly less faint fog on horizon
		 * Height bias = current altitude - 50 m: Fog more clearly discernible but doesn't limit view distance yet
		 * Height bias = current altitude - 40 m: ~1000 m
		 * Height bias = current altitude - 30 m: ~900 m
		 * Height bias = current altitude - 20 m: ~300 m
		 * Height bias = current altitude - 10 m: ~100 m
		 * Height bias = current altitude: ~40 m
		 * Height bias = current altitude + 10 m: ~20 m
		 * Height bias = current altitude + 20 m: ~7 m
		 * Height bias = current altitude + 30 m: ~2.5 m
		 * Height bias = current altitude + 40 m: ~0 m
		 * 
		 * The following values result in roughly same effective view distance:
		 * 
		 * Height bias = current altitude with dist density 1 and height density 0
		 * Height bias = current altitude + 15 with dist density 0.5 and height density 0.5
		 */

		Weather weather = g_Game.GetWeather();

		float dynVolFogDistDensity = weather.GetDynVolFogDistanceDensity();  //! Density percentage in <0,1> range
		float dynVolFogHeightDensity = weather.GetDynVolFogHeightDensity();  //! Density percentage in <0,1> range
		float dynVolFogHeightBias = weather.GetDynVolFogHeightBias();  //! Height offset in meters relative to 0

		float heightDiff = dynVolFogHeightBias - height;  //! How "deep" we are into dynamic fog vertically. Negative value = above fog

		if (dynVolFogDistDensity > 0 || dynVolFogHeightDensity < 1)
		{
			float hBias_Vis = 1.0;

			float distDensity_Vis = ExpansionMath.LookUp(dynVolFogDistDensity, 6, s_Expansion_DistDensity_In, s_Expansion_DistDensity_Out);

			if (dynVolFogHeightDensity < 1)
			{
				if (dynVolFogDistDensity == 0)
				{
					hBias_Vis = ExpansionMath.LookUp(heightDiff, 9, s_Expansion_HeightDiff_DistDensity0_In, s_Expansion_HeightDiff_DistDensity0_Out);
				}
				else if (dynVolFogDistDensity == 1)
				{
					hBias_Vis = ExpansionMath.LookUp(heightDiff, 9, s_Expansion_HeightDiff_DistDensity1_In, s_Expansion_HeightDiff_DistDensity1_Out);
				}
				else
				{
					float hBias_distDensity0_Vis = ExpansionMath.LookUp(heightDiff, 9, s_Expansion_HeightDiff_DistDensity0_In, s_Expansion_HeightDiff_DistDensity0_Out);
					float hBias_distDensity1_Vis = ExpansionMath.LookUp(heightDiff, 9, s_Expansion_HeightDiff_DistDensity1_In, s_Expansion_HeightDiff_DistDensity1_Out);

					//! @note this over-corrects (estimated view distance is too low) for 0 < dist density < 1 with height density 1,
					//! but a more accurate solution would have to use more/finer grained lookup tables and thus require more manual labor in
					//! painstakingly obtaining the table values, so this will have to do
					float scale = ExpansionMath.LookUp(dynVolFogDistDensity, 6, s_Expansion_DistDensity_Scale_In, s_Expansion_HeightDiff_Scale_Out);
					hBias_distDensity0_Vis *= ExpansionMath.LinearConversion(-40, 20, heightDiff, 1 - dynVolFogDistDensity, scale);

					hBias_Vis = ExpansionMath.LinearConversion(0, 1, dynVolFogDistDensity, hBias_distDensity0_Vis, hBias_distDensity1_Vis);
				}
			}
            else
            {
                return distDensity_Vis;
            }

			return ExpansionMath.LinearConversion(0, 1, dynVolFogHeightDensity, hBias_Vis, distDensity_Vis);
		}

		return 1.0;
	}
}
