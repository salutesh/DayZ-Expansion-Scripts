modded class Environment
{
	void Expansion_GetWeatherVisibility(out float fogVisibility, out float overcastVisibility, out float rainVisibility, out float daylightVisibility)
	{
		//! At 100% fog, visibility is around 50 m
		//! At 50% fog, visibility is roughly 460 m
		fogVisibility =  1.0 - m_Fog * 0.95;

		//! At 100% overcast, visibility is around 250 m
		overcastVisibility = 1.0 - m_Clouds * 0.75;

		//! At 100% rain, visibility is around 500 m
		rainVisibility = 1.0 - m_Rain * 0.5;

		daylightVisibility = m_DayOrNight;  //! 0/1 Night/Day
	}
}
