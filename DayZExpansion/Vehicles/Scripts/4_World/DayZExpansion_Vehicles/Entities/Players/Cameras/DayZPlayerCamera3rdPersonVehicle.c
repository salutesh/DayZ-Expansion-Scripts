/**
 * DayZPlayerCamera3rdPersonVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCamera3rdPersonVehicle
{   
	protected vector m_PreviousOrientation;
	protected bool m_WasFreeLook;

	protected float m_UpDownAngle;

	protected vector m_ExLagOffsetPosition;
	protected vector m_ExLagOffsetOrientation;
	protected float m_ExLagOffsetVelocityX[1];
	protected float m_ExLagOffsetVelocityY[1];
	protected float m_ExLagOffsetVelocityZ[1];
	protected float m_ExLagOffsetVelocityYaw[1];
	protected float m_ExLagOffsetVelocityPitch[1];
	protected float m_ExLagOffsetVelocityRoll[1];

	void DayZPlayerCamera3rdPersonVehicle( DayZPlayer pPlayer, HumanInputController pInput )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::DayZPlayerCamera3rdPersonVehicle - Start");
		#endif
		
		ExpansionVehicleBase expVehicle;
		if ( Class.CastTo( expVehicle, pPlayer.GetParent() ) )
		{
			m_fDistance 					= expVehicle.GetTransportCameraDistance();
			m_CameraOffsetMS				= expVehicle.GetTransportCameraOffset();
		}
		CarScript vehicle;
		if ( Class.CastTo( vehicle, pPlayer.GetParent() ) )
		{
			m_fDistance 					= vehicle.GetTransportCameraDistance();
			m_CameraOffsetMS				= vehicle.GetTransportCameraOffset();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::DayZPlayerCamera3rdPersonVehicle - End");
		#endif
	}
	
	override void OnActivate( DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::OnActivate - Start");
		#endif
		
		super.OnActivate( pPrevCamera, pPrevCameraResult );
		
		m_ExLagOffsetPosition = vector.Zero;
		m_ExLagOffsetVelocityX[0] = 0;
		m_ExLagOffsetVelocityY[0] = 0;
		m_ExLagOffsetVelocityZ[0] = 0;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::OnActivate - End");
		#endif
	}

	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::OnUpdate - Start");
		#endif

		ExpansionVehicleBase exVehicle;
		m_bForceFreeLook = Class.CastTo( exVehicle, m_pPlayer.GetParent() );
		
		super.OnUpdate( pDt, pOutResult );

		PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( false );

		EntityAI entParent;
		if ( !Class.CastTo( entParent, m_pPlayer.GetParent() ) )
			return;

		if ( pOutResult.m_CollisionIgnoreEntity )
		{
			if ( !dBodyIsDynamic( pOutResult.m_CollisionIgnoreEntity ) )
			{
				pOutResult.m_CollisionIgnoreEntity = NULL;
			} else
			{
				
			}
		}

		ExpansionHelicopterScript d_helicopter;
		if ( Class.CastTo( d_helicopter, m_pPlayer.GetParent() ) ) {
			OnUpdateHelicopter( pDt, d_helicopter, d_helicopter.IsFreeLook(), pOutResult );
		}

		ExpansionVehicleHelicopterBase helicopter;
		if ( Class.CastTo( helicopter, m_pPlayer.GetParent() ) ) {
			OnUpdateHelicopter( pDt, helicopter, helicopter.IsFreeLook(), pOutResult );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::OnUpdate - End");
		#endif
	}

	void OnUpdateHelicopter( float pDt, EntityAI pHelicopter, bool pIsFreeLook, out DayZPlayerCameraResult pOutResult )
	{
		vector playerTransformWS[4];
		m_pPlayer.GetTransform( playerTransformWS );

		vector helicopterTransform[4];
		pHelicopter.GetTransform( helicopterTransform );

		vector vehiclePositionWS = pHelicopter.GetOrigin();
		vector vehiclePositionMS = vehiclePositionWS.InvMultiply4( playerTransformWS );
		vector cameraPosition = vehiclePositionMS + m_CameraOffsetMS;

		vector posDiffWS = GetVelocity( pHelicopter ) * pDt;
		vector posDiffLS = posDiffWS.InvMultiply3( playerTransformWS );
		
		vector orientDiffWS = dBodyGetAngularVelocity(pHelicopter) * pDt * Math.RAD2DEG;
		vector orientDiff = orientDiffWS.InvMultiply3( helicopterTransform );

		vector rotation = vector.Zero;

		//! smooth it!
		m_ExLagOffsetPosition[0] = Math.SmoothCD( m_ExLagOffsetPosition[0], posDiffLS[0], m_ExLagOffsetVelocityX, 0.4, 1000, pDt );
		m_ExLagOffsetPosition[1] = Math.SmoothCD( m_ExLagOffsetPosition[1], posDiffLS[1], m_ExLagOffsetVelocityY, 0.4, 1000, pDt );
		m_ExLagOffsetPosition[2] = Math.SmoothCD( m_ExLagOffsetPosition[2], posDiffLS[2], m_ExLagOffsetVelocityZ, 0.4, 1000, pDt );

		m_LagOffsetOrientation[0] = Math.SmoothCD(m_LagOffsetOrientation[0], orientDiff[0], m_fLagOffsetVelocityYaw, 0.3, 1000, pDt);
		m_LagOffsetOrientation[1] = Math.SmoothCD(m_LagOffsetOrientation[1], orientDiff[1], m_fLagOffsetVelocityPitch, 0.3, 1000, pDt);

		if ( pIsFreeLook )
		{
			rotation[0] = m_fLeftRightAngle + CONST_ANGULAR_LAG_YAW_STRENGTH * m_LagOffsetOrientation[0];
			rotation[1] = Limit(m_fUpDownAngle + m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX) + CONST_ANGULAR_LAG_PITCH_STRENGTH * m_LagOffsetOrientation[1];
			rotation[2] = m_fRoll;
			
			pOutResult.m_fUseHeading = 1;
		} else
		{
			pOutResult.m_fUseHeading = 0;
		}
		
		Math3D.YawPitchRollMatrix( rotation, pOutResult.m_CameraTM );
		pOutResult.m_CameraTM[3] 			= cameraPosition - m_ExLagOffsetPosition;

		pOutResult.m_fIgnoreParentRoll		= 1.0;
		pOutResult.m_fInsideCamera		  	= 0.0;
		pOutResult.m_iDirectBone			= -1.0;
		pOutResult.m_fDistance 				= m_fDistance;
		pOutResult.m_fPositionModelSpace	= 1.0;
	}
}