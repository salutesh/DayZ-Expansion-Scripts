/**
 * ExpansionParachute.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionParachute extends ItemBase
{
    private bool m_IsCut;
    private int m_StartTime;

    private vector m_AngleControls;
    private vector m_PreviousAngleControls;

    private ExpansionParachuteBag m_Bag;
    private DayZPlayerImplement m_Player;

	void ExpansionParachute()
	{
	}

    vector GetPlayerOffset()
    {
        return "0 0 0";
    }

    void SetDeployed( ExpansionParachuteBag bag, DayZPlayerImplement player )
    {
        m_IsCut = false;

        m_Bag = bag;
        m_Player = player;
    }
    
	override void OnCreatePhysics()
    {
        super.OnCreatePhysics();

        if ( !m_Player || !m_Bag )
            return;

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( OnDeploy );
    }

    private void OnDeploy()
    {
		vector tmLocal[ 4 ];
        tmLocal[0] = "1 0 0";
        tmLocal[1] = "0 1 0";
        tmLocal[2] = "0 0 1";
        tmLocal[3] = GetPlayerOffset();

        m_Player.LinkToLocalSpaceOf( this, tmLocal );

        dBodyEnableGravity( m_Player, false );
        // dBodyActive( m_Player, ActiveState.INACTIVE );

        CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
        SetDynamicPhysicsLifeTime( -1 );

        m_StartTime = GetGame().GetTime();
    }

	void CutParachute()
	{
        if ( !m_Player || !m_Bag )
            return;

        m_IsCut = true;

        m_Player.UnlinkFromLocalSpace();
        
        // dBodyActive( m_Player, ActiveState.ALWAYS_ACTIVE );
        dBodyEnableGravity( m_Player, true );

        SetDynamicPhysicsLifeTime( ( GetGame().GetTime() - m_StartTime ) + 30 );

        // int layer = dBodyGetInteractionLayer( this ) & PhxInteractionLayers.CHARACTER;
		// dBodySetInteractionLayer( this, layer );

        vector force = GetGame().GetWeather().GetWind() + "0 20 0";
        dBodyApplyForce( this, force * dBodyGetMass( this ) );

        m_Bag = NULL;
        m_Player = NULL;
	}

    bool IsCut()
    {
        return m_IsCut;
    }

	void Input( vector angleControls )
	{
		m_AngleControls = angleControls;
	}
	
    void Simulate( float dt ) 
	{
        vector force = "0 9.5 0";

        vector yawVec = Vector( m_PreviousAngleControls[0] - m_AngleControls[0], 0, 0 ).AnglesToVector();
        force += yawVec;

        vector pitchVec = vector.Direction( GetPosition(), ModelToWorld( "0 0 1" ) );
        force += pitchVec * m_AngleControls[1];

        vector rollVec = vector.Direction( GetPosition(), ModelToWorld( "1 0 0" ) );
        force += rollVec * m_AngleControls[2];

        dBodyApplyImpulseAt( this, force * dt * dBodyGetMass( this ), ModelToWorld( "0 1 0" ) );

        m_PreviousAngleControls = m_AngleControls;
	}
}