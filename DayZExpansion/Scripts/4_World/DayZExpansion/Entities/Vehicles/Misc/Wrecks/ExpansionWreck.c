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
    protected bool m_Exploded;

    protected float m_Altitude;

    protected Object m_Crater;

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

        RegisterNetSyncVariableBool( "m_Exploded" );
        RegisterNetSyncVariableBool( "m_CraterParticle" );

        SetHealth( 0.0 );
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

            m_Crater = GetGame().CreateObject( "CraterLong", GetPosition() );
            if ( m_Crater )
            {    
                DisablePhysics( this );

                m_Crater.SetPosition( GetPosition( ) + m_CraterOffset );

                m_Crater.SetOrientation( vector.Zero );

                m_Crater.PlaceOnSurface( );
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
    override void EOnInit(IEntity other, int extra)
	{
        CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
		EnableDynamicCCD( true );
    }

    // ------------------------------------------------------------
	// EOnContact
	// ------------------------------------------------------------
    override void EOnContact( IEntity other, Contact extra ) 
	{
        if ( IsMissionClient() && !m_Exploded )
        {
            EffectSound sound =	SEffectManager.PlaySound( "Expansion_Explosive_Vehicle_SoundSet", GetPosition() );
		    sound.SetSoundAutodestroy( true );

            Particle.PlayOnObject( ParticleList.EXPANSION_EXPLOSION_HELICOPTER, this );
        }

        if ( IsMissionHost() )
		{
            if ( !m_CraterSpawned && GetPosition()[1] - GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ) < m_Altitude )
            {
                m_CraterTimer.Run( 0.1, this, "OnCrater", NULL, false ); 

                OnContact();

                m_CraterSpawned = true;
            }

            if ( !m_Exploded )
            {
                m_Exploded = true;
            }   
        }
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