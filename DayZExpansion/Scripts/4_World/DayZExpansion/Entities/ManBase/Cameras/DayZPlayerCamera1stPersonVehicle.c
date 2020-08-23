modded class DayZPlayerCamera1stPersonVehicle
{
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		super.OnUpdate( pDt, pOutResult );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPersonVehicle::OnUpdate Start");
		#endif

		ExpansionVehicleScript vehicle;
		if ( Class.CastTo( vehicle, m_pPlayer.GetParent() ) )
		{
			pOutResult.m_fUseHeading 		= 1.0;
			pOutResult.m_fInsideCamera 		= 0.0;

			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( true );
		} else
		{
			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( false );
		}

		ExpansionHelicopterScript heli;
		if ( Class.CastTo( heli, m_pPlayer.GetParent() ) && !heli.IsFreeLook() )
		{
			m_fUpDownAngle = 0;
			m_fLeftRightAngle = 0;

			m_fUpDownAngleAdd = 0;
			m_fLeftRightAngle = 0;
			
			Math3D.YawPitchRollMatrix( "0 0 0", pOutResult.m_CameraTM );
			pOutResult.m_CameraTM[3] = m_pPlayer.GetBonePositionMS( m_iBoneIndex ) + m_OffsetLS;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPersonVehicle::OnUpdate End");
		#endif
	}
}