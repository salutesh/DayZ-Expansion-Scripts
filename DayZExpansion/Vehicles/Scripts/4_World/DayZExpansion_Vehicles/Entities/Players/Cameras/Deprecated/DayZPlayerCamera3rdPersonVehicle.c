/**
 * DayZPlayerCamera3rdPersonVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_OBSOLETE_CAMERA
modded class DayZPlayerCamera3rdPersonVehicle
{
	float m_ExUpDownLockedAngle;

	vector m_ExLagOffsetPosition;
	vector m_ExLagOffsetOrientation;
	float m_ExLagOffsetVelocityX[1];
	float m_ExLagOffsetVelocityY[1];
	float m_ExLagOffsetVelocityZ[1];
	float m_ExLagOffsetVelocityYaw[1];
	float m_ExLagOffsetVelocityPitch[1];
	float m_ExLagOffsetVelocityRoll[1];

	float m_ExDistanceMultiplier = 1.0;
	float m_ExHeightMultiplier = 1.0;

	void DayZPlayerCamera3rdPersonVehicle( DayZPlayer pPlayer, HumanInputController pInput )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "DayZPlayerCamera3rdPersonVehicle").Add(pPlayer).Add(pInput);
#endif

		m_ExUpDownLockedAngle = -10;
		
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
	}
	
	override void OnActivate( DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnActivate").Add(pPrevCamera).Add(pPrevCameraResult);
#endif

		super.OnActivate( pPrevCamera, pPrevCameraResult );
		
		m_ExLagOffsetPosition = vector.Zero;
		m_ExLagOffsetVelocityX[0] = 0;
		m_ExLagOffsetVelocityY[0] = 0;
		m_ExLagOffsetVelocityZ[0] = 0;
	}

	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "OnUpdate").Add(pDt);
#endif

		m_ExDistanceMultiplier = 1.0;
		m_ExHeightMultiplier = 1.0;

		ExpansionVehicleBase exVehicle;
		m_bForceFreeLook = Class.CastTo( exVehicle, m_pPlayer.GetParent() );
		
		auto pbPlayer = PlayerBase.Cast(m_pPlayer);

		super.OnUpdate( pDt, pOutResult );

		pbPlayer.SetHeadInvisible_Ex( false );

		pOutResult.m_CollisionIgnoreEntity = pbPlayer.GetParent();

		ExpansionVehicleBase vehicle;
		if ( Class.CastTo( vehicle, pOutResult.m_CollisionIgnoreEntity ) )
		{
			if ( !dBodyIsDynamic( pOutResult.m_CollisionIgnoreEntity ) )
			{
				pOutResult.m_CollisionIgnoreEntity = NULL;
			}
		}

		EntityAI entParent;
		if ( !Class.CastTo( entParent, m_pPlayer.GetParent() ) )
			return;

		bool freelook = true;

		auto hcv = pbPlayer.GetCommand_Vehicle();

		ExpansionHelicopterScript d_helicopter;
		if (hcv && Class.CastTo(d_helicopter, m_pPlayer.GetParent()))
		{
			if (hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
				freelook = d_helicopter.IsFreeLook();
			
			OnUpdateHelicopter(pDt, d_helicopter, freelook, pOutResult);
			return;
		}

		auto ehcv = pbPlayer.GetCommand_ExpansionVehicle();

		ExpansionVehicleHelicopterBase helicopter;
		if (ehcv && Class.CastTo(helicopter, m_pPlayer.GetParent()))
		{
			if (ehcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
				freelook = helicopter.IsFreeLook();

			OnUpdateHelicopter(pDt, helicopter, freelook, pOutResult);
			return;
		}
	}

	void OnUpdateHelicopter( float pDt, EntityAI pHelicopter, bool pIsFreeLook, out DayZPlayerCameraResult pOutResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "OnUpdateHelicopter").Add(pDt).Add(pHelicopter).Add(pIsFreeLook);
#endif

		vector playerTransformWS[4];
		m_pPlayer.GetTransform( playerTransformWS );

		vector helicopterTransform[4];
		pHelicopter.GetTransform( helicopterTransform );

		vector vehiclePositionWS = pHelicopter.GetOrigin();
		vector vehiclePositionMS = vehiclePositionWS.InvMultiply4( playerTransformWS );
		vector cameraPosition = vehiclePositionMS + Vector(m_CameraOffsetMS[0], m_CameraOffsetMS[1] * m_ExHeightMultiplier, m_CameraOffsetMS[2]);

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
			rotation[0] = m_CurrentCameraYaw + CONST_ANGULAR_LAG_YAW_STRENGTH * m_LagOffsetOrientation[0];
			rotation[1] = Limit(m_fUpDownAngle + m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX) + CONST_ANGULAR_LAG_PITCH_STRENGTH * m_LagOffsetOrientation[1];
			rotation[2] = m_CurrentCameraRoll;
		} else {		
			Input input = GetGame().GetInput(); 	//! Reference to input
			if ( input.LocalValue( "UANextAction" ) != 0 && input.LocalHold( "UAVehicleSlow", false ) )
			{
				m_ExUpDownLockedAngle += 1.0;
			}
			if ( input.LocalValue( "UAPrevAction" ) != 0 && input.LocalHold( "UAVehicleSlow", false ) )
			{
				m_ExUpDownLockedAngle -= 1.0;
			}

			rotation[1] = m_ExUpDownLockedAngle;
		}
		
		Math3D.YawPitchRollMatrix( rotation, pOutResult.m_CameraTM );
		pOutResult.m_CameraTM[3] 			= cameraPosition - m_ExLagOffsetPosition;

		pOutResult.m_fIgnoreParentRoll		= 1.0;
		pOutResult.m_fIgnoreParentPitch 	= 1.0;
		pOutResult.m_fInsideCamera		  	= 0.0;
		pOutResult.m_fUseHeading 			= 0.0;
		pOutResult.m_iDirectBone			= -1.0;
		pOutResult.m_fDistance = m_fDistance * m_ExDistanceMultiplier;
		pOutResult.m_fPositionModelSpace	= 1.0;
	}
};
#endif
