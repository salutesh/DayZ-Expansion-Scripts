/**
 * ExpansionRockets.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionRocket
 * @brief		This class handle rocket post processing
 **/
class ExpansionRocket extends Roadflare
{
	protected bool m_Exploded;
	protected bool m_ExplodedSynchRemote;

	EffectSound m_Sound;
	EffectSound m_ExplosionSound;
	
	protected Particle m_ParticleEfx;
	ExpansionPointLight m_FlareLight;
	ExpansionPointLight m_RocketLight;
   
	void ExpansionRocket()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] Constructor start");
		#endif

		SetEventMask(EntityEvent.CONTACT | EntityEvent.SIMULATE);

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() ) 
		{
			m_Sound = SEffectManager.PlaySound("Expansion_Rocket_Flare_SoundSet", GetPosition());
			m_Sound.SetSoundAutodestroy( true );

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SpawnParticles, 250, false );
		} 

		RegisterNetSyncVariableBool( "m_ExplodedSynchRemote" );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( RemoveLater, 30000, false ); 

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] Constructor end");
		#endif
	}

	// ------------------------------------------------------------
	void ~ExpansionRocket()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] Deconstructor start");
		#endif

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			if ( m_ParticleEfx )
			{
				m_ParticleEfx.Stop();
				GetGame().ObjectDelete(m_ParticleEfx);
			}

			if ( m_Sound ) 
				m_Sound.SoundStop();

			if ( m_FlareLight )
				GetGame().ObjectDelete( m_FlareLight );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] Deconstructor end");
		#endif
	}

	// ------------------------------------------------------------
	void SpawnSounds()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] SpawnSounds start");
		#endif

		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
		{
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] SpawnSounds end");
		#endif
	}
	
	// ------------------------------------------------------------
	void SpawnParticles()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] SpawnParticles start");
		#endif

		// m_ParticleEfx = Particle.PlayOnObject(ParticleList.EXPANSION_ROCKET_SMOKE, this);

		if ( !m_FlareLight )
		{
			m_FlareLight = ExpansionPointLight.Cast( ScriptedLightBase.CreateLight( ExpansionPointLight, Vector( 0, 0, 0 ) )  ); 

			m_FlareLight.SetDiffuseColor( 1, 1, 1 );
			m_FlareLight.SetPosition( GetPosition() );
			m_FlareLight.SetRadiusTo( 10 );
			m_FlareLight.SetBrightnessTo( 1 ); 
			m_FlareLight.AttachOnObject( this, "0 0 0" );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] SpawnParticles end");
		#endif
	}

	// ------------------------------------------------------------
	void RemoveLater()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] RemoveLater start");
		#endif

		ToDelete();

		SetPosition( vector.Zero );
		GetGame().ObjectDelete( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] RemoveLater end");
		#endif
	}

	override void EOnContact(IEntity other, Contact extra) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "[ExpansionRocket] EOnContact start" );
		#endif

		if ( IsMissionHost() && !m_Exploded )
		{
			m_Exploded = true;
			m_ExplodedSynchRemote = true;

			//! Explode( DT_EXPLOSION, "ExpansionRocket_Ammo" );
			ExpansionCreateExplosion( this, "ExpansionRocket_Ammo", 2.5, 250 );
			
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( RemoveLater, 150, false ); 

			if ( !GetGame().IsMultiplayer() )
			{
				HandleClientExplosion();
			} else
			{
				SetSynchDirty();
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRocket] EOnContact end");
		#endif
	}

	private void HandleClientExplosion()
	{
		if ( m_ParticleEfx )
			m_ParticleEfx.Stop();

		if ( m_Sound ) 
			m_Sound.SoundStop();

		SEffectManager.PlaySound( "Expansion_Explosive_Large_SoundSet", GetPosition() );

		m_RocketLight = ExpansionPointLight.Cast( ScriptedLightBase.CreateLight( ExpansionPointLight, Vector( 0, 0, 0 ) ) ); 

		m_RocketLight.SetDiffuseColor( 1, 0.25, 0 );
		m_RocketLight.SetPosition( GetPosition() );
		m_RocketLight.SetRadiusTo( 60 );
		m_RocketLight.SetBrightnessTo( 4.5 ); 
		m_RocketLight.SetLifetime( 0.1 );
		m_RocketLight.SetFadeOutTime( 0.1 );
		m_RocketLight.SetFlareVisible( false );

		float distance_to_player = vector.Distance(GetPosition(), GetGame().GetPlayer().GetPosition());
		float strength_factor = Math.InverseLerp(GameConstants.CAMERA_SHAKE_GRENADE_DISTANCE, 0, distance_to_player);
		// GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(strength_factor * 4);

		vector n = GetPosition().VectorToAngles();

		//Particle p1 = Particle.Play(ParticleList.SMOKE_GENERIC_WRECK, GetPosition() );
		//p1.SetOrientation(n);
		
		Particle p2 = Particle.Play(ParticleList.EXPLOSION_LANDMINE, GetPosition() );
		p2.SetOrientation(n);

		Particle p3 = Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, GetPosition() );
		p3.SetOrientation(n);
		
		Particle p4 = Particle.Play(ParticleList.IMPACT_GRAVEL_RICOCHET, GetPosition() );
		p4.SetOrientation(n);
	}

	// ------------------------------------------------------------
	// OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRocket::OnVariablesSynchronized - Start");
		#endif

		super.OnVariablesSynchronized();

		if ( m_ExplodedSynchRemote && !m_Exploded ) 
		{
			m_Exploded = true;
			
			HandleClientExplosion();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRocket::OnVariablesSynchronized - End");
		#endif
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo (EntityAI parent)
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
};