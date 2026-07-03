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

modded class DayZPlayerCamera3rdPersonVehicle
{
	vector m_ExLagOffsetPosition;
	float m_ExLagOffsetVelocityX[1];
	float m_ExLagOffsetVelocityY[1];
	float m_ExLagOffsetVelocityZ[1];

	float m_ExDistanceMultiplier = 1.0;
	float m_ExHeightMultiplier = 1.0;

	void DayZPlayerCamera3rdPersonVehicle( DayZPlayer pPlayer, HumanInputController pInput )
	{
		m_Expansion_CameraHandler = new ExpansionCameraHandler(this);
		m_ExpansionVehicle = ExpansionVehicle.Get(m_Ex_Player);
	}
	
	override void OnActivate( DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult )
	{
		super.OnActivate( pPrevCamera, pPrevCameraResult );
		
		m_Expansion_CameraHandler.OnActivate(m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd, pPrevCamera, pPrevCameraResult);

		m_ExLagOffsetPosition = vector.Zero;
		m_ExLagOffsetVelocityX[0] = 0;
		m_ExLagOffsetVelocityY[0] = 0;
		m_ExLagOffsetVelocityZ[0] = 0;

		m_ExDistanceMultiplier = GetExpansionClientSettings().VehicleCameraDistance;
		m_ExHeightMultiplier = GetExpansionClientSettings().VehicleCameraHeight;
	}

	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		bool isFreeLook = true;
		bool isHelicopter = false;

		ExpansionHelicopterScript helicopter = m_ExpansionVehicle.GetExpansionHelicopter();
		if (helicopter)
		{
			isHelicopter = true;

			if (m_Ex_Player.Expansion_IsDriver())
				isFreeLook = helicopter.IsFreeLook();
		}

		m_ExIsFreeLook = isFreeLook;

		m_Expansion_CameraHandler.OnUpdate(pDt, m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd);

		super.OnUpdate( pDt, pOutResult );

		if (isHelicopter)
			Expansion_OnUpdateHelicopter(pDt, helicopter, isFreeLook, pOutResult);
	}

	void Expansion_OnUpdateHelicopter( float pDt, EntityAI pHelicopter, bool pIsFreeLook, out DayZPlayerCameraResult pOutResult )
	{
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

		{
			rotation[0] = m_CurrentCameraYaw + CONST_ANGULAR_LAG_YAW_STRENGTH * m_LagOffsetOrientation[0];
			rotation[1] = Limit(m_fUpDownAngle + m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX) + CONST_ANGULAR_LAG_PITCH_STRENGTH * m_LagOffsetOrientation[1];
			rotation[2] = m_CurrentCameraRoll;
		}
		
		Math3D.YawPitchRollMatrix( rotation, pOutResult.m_CameraTM );
		pOutResult.m_CameraTM[3] 			= cameraPosition - m_ExLagOffsetPosition;

		pOutResult.m_fIgnoreParentRoll		= 1.0;
		pOutResult.m_fIgnoreParentPitch 	= 1.0;
		pOutResult.m_fIgnoreParentYaw 		= 0.0;
		pOutResult.m_fInsideCamera		  	= 0.0;
		pOutResult.m_fUseHeading 			= 0.0;
		pOutResult.m_iDirectBone			= -1.0;
		pOutResult.m_fDistance = m_fDistance * m_ExDistanceMultiplier;
		pOutResult.m_fPositionModelSpace	= 1.0;
	}

	override void Expansion_GetUDAngles(out float pAngle, out float pAngleAdd)
	{
		pAngle = m_fUpDownAngle;
		pAngleAdd = m_fUpDownAngleAdd;
	}
};
