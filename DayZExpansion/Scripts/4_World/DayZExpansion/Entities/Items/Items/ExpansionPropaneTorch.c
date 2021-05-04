/**
 * ExpansionPropaneTorchLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPropaneTorchLight extends PointLightBase
{
	protected static float m_DefaultBrightness = 1.0;
	protected static float m_DefaultRadius = 2.0;
	
	void ExpansionPropaneTorchLight()
	{
		SetVisibleDuringDaylight( false );
		SetRadiusTo( m_DefaultRadius );
		SetBrightnessTo( m_DefaultBrightness );
		SetFlareVisible( false );
		SetAmbientColor( 0.1, 0.3, 1.0 );
		SetDiffuseColor( 0.1, 0.3, 1.0 );
		SetFadeOutTime( 0.2 );
		SetCastShadow( true );
	}
}

class ExpansionPropaneTorch extends ItemBase
{
	Particle					m_FireParticle;

	static vector				m_ParticleLocalPos = Vector( 0.053, 0.255, 0 );

	ExpansionPropaneTorchLight	m_Light;

	//sound
	const string SOUND_BURNING = "Expansion_PropaneTorch_Burn_SoundSet";
	const string SOUND_WELDING = "Expansion_PropaneTorch_Weld_SoundSet";
	const string SOUND_TURN_ON = "portablegaslamp_turn_on_SoundSet";
	const string SOUND_TURN_OFF = "portablegaslamp_turn_off_SoundSet";
	
	protected EffectSound m_SoundWeldingLoop;
	protected EffectSound m_SoundBurningLoop;
	protected EffectSound m_SoundTurnOn;
	protected EffectSound m_SoundTurnOff;
	
	override void EEDelete( EntityAI parent )
	{
		StopAllParticles();
	}

	override bool IsIgnited()
	{
		return GetCompEM().IsWorking();
	}

	override bool CanIgniteItem( EntityAI ignite_target = NULL )
	{
		return GetCompEM().IsWorking();
	}

	override void ExpansionActionOnAnimationEvent( ActionData action_data )
	{
		if ( m_FireParticle )
			m_FireParticle.ScaleParticleParamFromOriginal( EmitorParam.SIZE, 1.25 );
	}

	override void ExpansionActionOnStartAnimationLoop( ActionData action_data )
	{
		SoundWeldingStart();
	}

	override void ExpansionActionOnEndAnimationLoop( ActionData action_data )
	{
		SoundWeldingStop();
	}

	override void ExpansionActionOnEnd( ActionData action_data )
	{
		if ( m_FireParticle )
			m_FireParticle.ScaleParticleParamFromOriginal( EmitorParam.SIZE, 0.75 );

		SoundWeldingStop();
	}

	//--- POWER EVENTS
	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		//sound (client only)
		SoundTurnOn();
	}

	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		//sound (client only)
		SoundTurnOff();
	}

	override void OnWorkStart()
	{
		if ( IsMissionClient() )
		{
			if ( !m_FireParticle )
			{
				m_FireParticle = Particle.PlayOnObject( ParticleList.EXPANSION_PROPANE_FLAME, this, m_ParticleLocalPos, Vector( 0, 0, 35 ) );
				m_FireParticle.ScaleParticleParamFromOriginal( EmitorParam.SIZE, 0.75 );
			}

			if ( !m_Light )
			{
				m_Light = ExpansionPropaneTorchLight.Cast( ScriptedLightBase.CreateLight( ExpansionPropaneTorchLight, m_ParticleLocalPos ) );

				Object direct_particle = m_FireParticle.GetDirectParticleEffect();

				if ( direct_particle && direct_particle != m_Light.GetAttachmentParent() )
				{
					m_Light.AttachOnObject( direct_particle, Vector( 0, 0, 0 ) );
				}
			}
		}

		//sound (client only)
		SoundBurningStart();
	}

	override void OnWorkStop()
	{
		if ( m_Light )
			m_Light.FadeOut();

		//sound (client only)
		SoundBurningStop();
		SoundWeldingStop();

		StopAllParticles();
	}

	//================================================================
	// SOUNDS
	//================================================================
	protected void SoundBurningStart()
	{
		PlaySoundSetLoop( m_SoundBurningLoop, SOUND_BURNING, 0.1, 0.3 );
	}

	protected void SoundBurningStop()
	{
		StopSoundSet( m_SoundBurningLoop );
	}

	protected void SoundTurnOn()
	{
		PlaySoundSet( m_SoundTurnOn, SOUND_TURN_ON, 0.1, 0.1 );
	}

	protected void SoundTurnOff()
	{
		PlaySoundSet( m_SoundTurnOff, SOUND_TURN_OFF, 0.1, 0.1 );
	}

	protected void SoundWeldingStart()
	{
		PlaySoundSetLoop( m_SoundWeldingLoop, SOUND_WELDING, 0.5, 0.25 );
	}

	protected void SoundWeldingStop()
	{
		StopSoundSet( m_SoundWeldingLoop );
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction( ActionTurnOnWhileInHands );
		AddAction( ActionTurnOffWhileInHands );
		AddAction( ActionLightItemOnFire );
	}

	void StopAllParticles()
	{
		if ( m_FireParticle )
		{
			//! Hack to avoid flickering
			m_FireParticle.ScaleParticleParamFromOriginal( EmitorParam.SIZE, 0.0 );

			//! Hack to avoid particle being visible at a skewed angle for a brief moment
			//! This apparently happens because Stop() will first move the particle from parent to world without accounting for orientation
			m_FireParticle.SetOrientation( Vector( 0, 0, -35 ) );

			m_FireParticle.Stop();
			m_FireParticle = NULL;
		}
	}
}
