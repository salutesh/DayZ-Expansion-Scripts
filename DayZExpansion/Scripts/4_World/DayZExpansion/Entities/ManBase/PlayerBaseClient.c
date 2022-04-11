/**
 * PlayerBaseClient .c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBaseClient
{
#ifndef EXPANSION_PARTICLE_VOLUMETRIC_CLOUDS_DISABLE
	Particle m_Clouds;

	void CreateClouds()
	{
		if (!m_Clouds && ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ))
		{
			m_Clouds = Particle.PlayInWorld(ParticleList.EXPANSION_CLOUD_SMOKE, "0 500 0 ");
		}
	}

	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			Print("Tick");
			vector pos = GetGame().GetCurrentCameraPosition();
			vector dir = GetGame().GetCurrentCameraDirection();
			if (m_Clouds)
			{
				Print("Pos " + pos);
				m_Clouds.SetPosition(Vector(pos[0], 250, pos[2]));
			}
			else
			{
				CreateClouds();
			}
			
		}
	}
#endif
}
