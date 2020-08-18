/**
 * ExpansionWreck.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionWreck: Container_Base
{
	protected ref TStringArray m_CraterSurfaces;
	protected ref Timer m_CraterTimer;

	protected vector m_CraterOffset;

	protected bool m_CraterParticle;
	protected bool m_CraterSurface;
	protected bool m_CraterSpawned;

	protected float m_Altitude;

	protected Object m_Crater;

	private vector m_PlantFallSoundPosition;

	void ExpansionWreck()
	{
		m_CraterSurfaces = new TStringArray;
		
		m_CraterSurfaces.Insert( "cp_dirt" );

		m_CraterSurfaces.Insert( "cp_broadleaf_dense1" );
		m_CraterSurfaces.Insert( "cp_broadleaf_dense2" );
		m_CraterSurfaces.Insert( "cp_broadleaf_sparse1" );
		m_CraterSurfaces.Insert( "cp_broadleaf_sparse2" );

		m_CraterSurfaces.Insert( "cp_conifer_common1" );
		m_CraterSurfaces.Insert( "cp_conifer_common2" );
		m_CraterSurfaces.Insert( "cp_conifer_moss1" );
		m_CraterSurfaces.Insert( "cp_conifer_moss2" );

		m_CraterSurfaces.Insert( "cp_grass" );
		m_CraterSurfaces.Insert( "cp_grass_tall" );

		m_CraterTimer = new Timer();

		SetEventMask( EntityEvent.CONTACT | EntityEvent.INIT );

		RegisterNetSyncVariableBool( "m_CraterParticle" );

		if ( IsMissionHost() )
		{
			SetHealth( 0.0 );	//! Object::SetHealth cannot be called on client.
		}
	}

	void ~ExpansionWreck()
	{
		if ( IsMissionHost() )
		{
			if ( m_Crater )
			{
				GetGame().ObjectDelete( m_Crater );
			}
		}
	}

	void SetOffset( vector offset )
	{
		m_CraterOffset = offset;
	}

	void SetAltitude( float altitude )
	{
		m_Altitude = altitude;
	}

	// ------------------------------------------------------------
	// OnCrater
	// ------------------------------------------------------------
	void OnCrater()
	{   
		//! TODO: Add collision check so if heli is colliding with any building it wont spawn crater

		string surface; 
		GetGame().SurfaceGetType( GetPosition()[0], GetPosition()[2], surface );

		for ( int b = 0; b < m_CraterSurfaces.Count(); b++ )
		{
			if ( !m_CraterSurface )
			{
				m_CraterSurface = m_CraterSurfaces[b].Contains( surface );
			}
		}

		if ( m_CraterSurface )
		{
			SetOrientation( vector.Zero );

			PlaceOnSurface( );

			m_Crater = GetGame().CreateObjectEx( "CraterLong", GetPosition(), ECE_PLACE_ON_SURFACE );
			if ( m_Crater )
			{
				SetDynamicPhysicsLifeTime( 0.001 );

				dBodyActive( this, ActiveState.INACTIVE );
				dBodyDynamic( this, false );

				SetVelocity( this, "0 0 0" );
				dBodySetAngularVelocity( this, "0 0 0" );

				m_Crater.SetPosition( GetPosition( ) + m_CraterOffset );

				m_Crater.SetOrientation( vector.Zero );

				//m_Crater.PlaceOnSurface( );
			}

			m_CraterParticle = true;
		}

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// OnParticle
	// ------------------------------------------------------------
	void OnParticle()
	{
		vector random;

		random[0] = Math.RandomFloat(0, 5);
		random[1] = random[1];
		random[2] = Math.RandomFloat(0, 5);

		EffectSound sound = SEffectManager.PlaySound( "Expansion_Explosive_Vehicle_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );

		Particle.PlayOnObject( ParticleList.EXPANSION_EXPLOSION_HELICOPTER, this, random );
	}

	// ------------------------------------------------------------
	// OnContact
	// ------------------------------------------------------------
	void OnContact()
	{
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// EOnContact
	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra )
	{
	}

	// ------------------------------------------------------------
	// EOnContact
	// ------------------------------------------------------------
	override void EOnContact( IEntity other, Contact extra ) 
	{
		if ( IsMissionHost() )
		{
			if ( !m_CraterSpawned && GetPosition()[1] - GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ) < m_Altitude )
			{
				// m_CraterTimer.Run( 0.1, this, "OnCrater", NULL, false ); 

				OnContact();

				m_CraterSpawned = true;
			}

			Object obj;
			if ( Class.CastTo( obj, other ) && extra.Impulse > 1000 )
			{
				TreeHard treeHard;
				TreeSoft treeSoft;
				BushHard bushHard;
				BushSoft bushSoft;
				if ( Class.CastTo( treeHard, obj ) )
				{
					GetGame().RPCSingleParam( this, PlantType.TREE_HARD, new Param1< vector >( obj.GetPosition() ), true );
				} else if ( Class.CastTo( treeSoft, obj ) )
				{
					GetGame().RPCSingleParam( this, PlantType.TREE_SOFT, new Param1< vector >( obj.GetPosition() ), true );
				} else if ( Class.CastTo( bushHard, obj ) )
				{
					GetGame().RPCSingleParam( this, PlantType.BUSH_HARD, new Param1< vector >( obj.GetPosition() ), true );
				} else if ( Class.CastTo( bushSoft, obj ) )
				{
					GetGame().RPCSingleParam( this, PlantType.BUSH_SOFT, new Param1< vector >( obj.GetPosition() ), true );
				} else 
				{
					return;
				}

				if ( obj.GetHealth( "", "" ) > 0 )
				{
					obj.SetHealth( "", "", 0 );

					if ( dBodyIsSet( obj ) )
						dBodyDestroy( obj );
						
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( GetGame().ObjectDelete, obj );
				}
			}
		}
	}
			
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx ) 
	{
		if ( rpc_type != PlantType.TREE_HARD )
			if ( rpc_type != PlantType.TREE_SOFT )
				if ( rpc_type != PlantType.BUSH_HARD )
					if ( rpc_type != PlantType.BUSH_SOFT )
					{
						super.OnRPC( sender, rpc_type, ctx );
						return;
					}
		
		Param1< vector > data = new Param1< vector >( GetPosition() );
		if ( !ctx.Read( data ) )
			m_PlantFallSoundPosition = GetPosition();
		else
			m_PlantFallSoundPosition = data.param1;

		switch( rpc_type )
		{
			case PlantType.TREE_HARD:
				SoundHardTreeFallingPlay();
			break;
			
			case PlantType.TREE_SOFT:
				SoundSoftTreeFallingPlay();
			break;
			
			case PlantType.BUSH_HARD:
				SoundHardBushFallingPlay();
			break;
			
			case PlantType.BUSH_SOFT:
				SoundSoftBushFallingPlay();
			break;
		}
	}
	
	override void SoundHardTreeFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "hardTreeFall_SoundSet", m_PlantFallSoundPosition );
		sound.SetSoundAutodestroy( true );
	}
		
	override void SoundSoftTreeFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "softTreeFall_SoundSet", m_PlantFallSoundPosition );
		sound.SetSoundAutodestroy( true );
	}
		
	override void SoundHardBushFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "hardBushFall_SoundSet", m_PlantFallSoundPosition );
		sound.SetSoundAutodestroy( true );
	}
		
	override void SoundSoftBushFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "softBushFall_SoundSet", m_PlantFallSoundPosition );
		sound.SetSoundAutodestroy( true );
	}

	// ------------------------------------------------------------
	// OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( m_CraterParticle )
		{
			for ( int i = 1; i < 5; i++ )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnParticle, i * 100, false );
			}
		}
	}

	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return true;
	}

	// ------------------------------------------------------------
	// CanUseConstruction
	// ------------------------------------------------------------
	override bool CanUseConstruction()
	{
		return true;
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
}