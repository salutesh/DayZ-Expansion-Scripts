modded class Environment
{
	float m_Expansion_Visibilty_In[5];
	float m_Expansion_FogVisibilty_Out[5];

	override void Init(PlayerBase pPlayer)
	{
		super.Init(pPlayer);

		if (pPlayer.IsAI())
		{
			m_Expansion_Visibilty_In[0] = 0.0;
			m_Expansion_Visibilty_In[1] = 0.25;
			m_Expansion_Visibilty_In[2] = 0.5;
			m_Expansion_Visibilty_In[3] = 0.75;
			m_Expansion_Visibilty_In[4] = 1.0;

			//! At 25% fog, visibility is around 900 m
			//! At 50% fog, visibility is around 440 m
			//! At 75% fog, visibility is around 250 m
			//! At 100% fog, visibility is around 45 m
			m_Expansion_FogVisibilty_Out[0] = 1.000;
			m_Expansion_FogVisibilty_Out[1] = 0.900;
			m_Expansion_FogVisibilty_Out[2] = 0.440;
			m_Expansion_FogVisibilty_Out[3] = 0.250;
			m_Expansion_FogVisibilty_Out[4] = 0.045;
		}
	}

	void Expansion_GetWeatherVisibility(out float fogVisibility, out float overcastVisibility, out float rainVisibility)
	{
		fogVisibility = ExpansionMath.LookUp(m_Fog, 5, m_Expansion_Visibilty_In, m_Expansion_FogVisibilty_Out);
		overcastVisibility = ExpansionMath.LinearConversion(0.75 - Math.Min(m_Rain, 0.75), 1.0, m_Clouds, 1.0, 0.5 * fogVisibility);
		rainVisibility = ExpansionMath.LinearConversion(0.5, 1.0, m_Rain, 1.0, 0.33);
	}
}
