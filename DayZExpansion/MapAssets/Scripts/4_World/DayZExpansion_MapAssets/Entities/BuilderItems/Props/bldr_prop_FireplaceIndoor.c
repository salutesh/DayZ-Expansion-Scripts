class bldr_prop_FireplaceIndoor: House
{
	Particle m_ParticleEfx;
	protected EffectSound m_SoundFireLoop;
	protected FireplaceLight m_Light;

	void bldr_prop_FireplaceIndoor()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnParticles, 2000, false);
		}
	}

	void SpawnParticles()
	{
		m_ParticleEfx = Particle.PlayOnObject(ParticleList.HOUSE_NORMAL_FIRE, this, Vector( 0, 0.05, 0 ));
		PlaySoundSetLoop( m_SoundFireLoop, "LightFire_SoundSet", 1.0, 1.0 );
		SetLightEntity( FireplaceLight.Cast( ScriptedLightBase.CreateLight(FireplaceLight, GetPosition(), 20) ) );
		GetLightEntity().AttachOnMemoryPoint(this, "light");
	}

	void SetLightEntity( FireplaceLight light )
	{
		m_Light = light;
	}
	FireplaceLight GetLightEntity()
	{
		return m_Light;
	}

};