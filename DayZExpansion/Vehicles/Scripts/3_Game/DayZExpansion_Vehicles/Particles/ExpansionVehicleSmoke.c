/**
 * ExpansionVehicleSmoke.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class EffVehicleSmoke
{
	void Expansion_SetParticleStateHelicopter()
	{
	}

	void Expansion_SetRotorSpeed(float rotorSpeed)
	{
	}
};

modded class EffExhaustSmoke
{
	float m_Expansion_FrameTime;
	float m_Expansion_RotorSpeed;

	override void Expansion_SetParticleStateHelicopter()
	{
		SetParticleState( ParticleList.EXPANSION_HELICOPTER_EXHAUST );

		Particle p = GetParticle();

		if (p)
		{
			p.ScaleParticleParamFromOriginal( EmitorParam.LIFETIME, 0.0 );
			p.ScaleParticleParamFromOriginal( EmitorParam.LIFETIME_RND, 0.0 );
			p.ScaleParticleParamFromOriginal( EmitorParam.BIRTH_RATE, 1.1 );
			p.ScaleParticleParamFromOriginal( EmitorParam.BIRTH_RATE_RND, 1.1 );
		}
	}

	override void Event_OnFrameUpdate(float time_delta)
	{
		if (GetParticleID() != ParticleList.EXPANSION_HELICOPTER_EXHAUST)
		{
			super.Event_OnFrameUpdate(time_delta);
			return;
		}

		m_Expansion_FrameTime += time_delta;
		
		if (m_Expansion_FrameTime < 0.25)
			return;

		m_Expansion_FrameTime = 0;

		Car parent = Car.Cast( GetAttachmentParent() );
		Particle p = GetParticle();
		
		if ( parent && p )
		{
			float speed = parent.GetSpeedometerAbsolute();
			float lifetime_scale;
				
			if (speed < 100)
				lifetime_scale = (100 - speed) / 100 * m_Expansion_RotorSpeed;
			else
				lifetime_scale = 0.1;
				
			float birthrate_scale = 1 + (speed * 0.1 );

			p.ScaleParticleParamFromOriginal( EmitorParam.LIFETIME, lifetime_scale );
			p.ScaleParticleParamFromOriginal( EmitorParam.LIFETIME_RND, lifetime_scale );
			p.ScaleParticleParamFromOriginal( EmitorParam.BIRTH_RATE, birthrate_scale );
			p.ScaleParticleParamFromOriginal( EmitorParam.BIRTH_RATE_RND, birthrate_scale );
		}
	}

	override void Expansion_SetRotorSpeed(float rotorSpeed)
	{
		m_Expansion_RotorSpeed = rotorSpeed;
	}
};
