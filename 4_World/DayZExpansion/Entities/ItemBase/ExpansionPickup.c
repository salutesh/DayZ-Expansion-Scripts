class ExpansionPickup extends ItemBase
{
	private vector m_PositionWS;
	private vector m_OrientationWS;

	private float m_Levitate;
	private float m_LevitateSpeed;
	private float m_LevitateBound;
	private float m_LevitateSwingAngleMax;
	private bool m_LevitateReverse;

	void ExpansionPickup()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPickup::ExpansionPickup - Start");
		#endif
		
		SetEventMask( EntityEvent.SIMULATE );

		m_LevitateSpeed = 20.0;
		m_LevitateBound = 1.0;
		m_LevitateSwingAngleMax = 45.0;

		m_PositionWS = GetPosition();
		m_OrientationWS = GetOrientation();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPickup::ExpansionPickup - End");
		#endif
	}

	override void EOnSimulate( IEntity owner, float dt ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPickup::EOnSimulate - Start");
		#endif
		
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return;
			
		if ( m_LevitateReverse )
		{
			m_Levitate -= m_LevitateSpeed * dt;
			if ( m_Levitate <= -m_LevitateBound )
			{
				m_LevitateReverse = false;
			}
		} else if ( m_LevitateReverse )
		{
			m_Levitate += m_LevitateSpeed * dt;
			if ( m_Levitate >= m_LevitateBound )
			{
				m_LevitateReverse = true;
			}
		}

		vector up = "0 1 0";
		vector position = m_PositionWS + ( up * m_Levitate ) + ( up * m_LevitateBound );

		float normalizedLevitate = m_Levitate / m_LevitateBound;
		vector rotation = m_OrientationWS + Vector( normalizedLevitate * m_LevitateSwingAngleMax, 0, 0 );

		vector rotationMat[3];
		Math3D.YawPitchRollMatrix( rotationMat, rotationMat );

		vector transform[4];
		transform[0] = rotationMat[0];
		transform[1] = rotationMat[1];
		transform[2] = rotationMat[2];
		transform[3] = position;

		SetTransform( transform );

		InventoryLocation currentIL = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation( currentIL );
		if ( currentIL.GetType() != InventoryLocationType.GROUND )
			return;

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player || vector.Distance( player.GetPosition(), m_PositionWS ) > 0.5 )
			return;
		
		InventoryLocation il = new InventoryLocation;

		player.GetInventory().FindFreeLocationFor( this, FindInventoryLocationType.ANY, il );
		if ( player.GetInventory().HasInventoryReservation( this, il ) )
			return;

		player.GetInventory().AddInventoryReservation( this, il, GameInventory.c_InventoryReservationTimeoutMS);
		
		player.PredictiveTakeToDst( currentIL, il );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPickup::EOnSimulate - End");
		#endif
	}

	override bool OnNetworkTransformUpdate( out vector pos, out vector ypr )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPickup::EOnSimulate - Start");
		#endif
		
		m_PositionWS = pos;
		m_OrientationWS = ypr;
		m_OrientationWS[0] = m_OrientationWS[0] * Math.RAD2DEG;
		m_OrientationWS[1] = m_OrientationWS[1] * Math.RAD2DEG;
		m_OrientationWS[2] = m_OrientationWS[2] * Math.RAD2DEG;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPickup::EOnSimulate - Return true");
		#endif
		
		return true;
	}
}