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

	ExpansionClientSettings m_ExClientSettings;

	void DayZPlayerCamera3rdPersonVehicle( DayZPlayer pPlayer, HumanInputController pInput )
	{
		m_Expansion_CameraHandler = new ExpansionCameraHandler(this);
		m_ExpansionVehicle = ExpansionVehicle.Get(m_Ex_Player);

		m_ExClientSettings = GetExpansionClientSettings();
	}
	
	override void OnActivate( DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult )
	{
		super.OnActivate( pPrevCamera, pPrevCameraResult );
		
		m_Expansion_CameraHandler.OnActivate(m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd, pPrevCamera, pPrevCameraResult);

		m_ExLagOffsetPosition = vector.Zero;
		m_ExLagOffsetVelocityX[0] = 0;
		m_ExLagOffsetVelocityY[0] = 0;
		m_ExLagOffsetVelocityZ[0] = 0;
	}

	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		bool isFreeLook = true;

		ExpansionHelicopterScript helicopter = m_ExpansionVehicle.GetExpansionHelicopter();
		if (helicopter)
		{
			if (m_Ex_Player.Expansion_IsDriver())
				isFreeLook = helicopter.IsFreeLook();
		}

		m_ExIsFreeLook = isFreeLook;

		m_Expansion_CameraHandler.OnUpdate(pDt, m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd);

		float distance = m_fDistance;
		if (helicopter && m_ExClientSettings.UseVanilla3rdPersonHelicopterCamera)
			m_fDistance *= m_ExClientSettings.VehicleCameraDistance;

		super.OnUpdate( pDt, pOutResult );

		if (helicopter)
		{
			if (!m_ExClientSettings.UseVanilla3rdPersonHelicopterCamera)
			{
				Expansion_OnUpdateHelicopter(pDt, helicopter, isFreeLook, pOutResult);
			}
			else
			{
				m_fDistance = distance;

				float cameraY = pOutResult.m_CameraTM[3][1];
				pOutResult.m_CameraTM[3][1] = (cameraY - m_CameraOffsetMS[1]) + m_CameraOffsetMS[1] * m_ExClientSettings.VehicleCameraHeight;
			}
		}
	}

	void Expansion_OnUpdateHelicopter( float pDt, EntityAI pHelicopter, bool pIsFreeLook, out DayZPlayerCameraResult pOutResult )
	{
		vector playerTransformWS[4];
		m_pPlayer.GetTransform( playerTransformWS );

		IEntity vehicle = m_Transport;
		
		//! update camera offset
		vector cameraPosition = vector.Zero;
		if ( vehicle )
		{
			vector vehiclePositionWS = vehicle.GetOrigin();
			vector vehiclePositionMS = vehiclePositionWS.InvMultiply4(playerTransformWS);
			cameraPosition = vehiclePositionMS + Vector(m_CameraOffsetMS[0], m_CameraOffsetMS[1] * m_ExClientSettings.VehicleCameraHeight, m_CameraOffsetMS[2]);
		}
	
		//! create LS lag from vehicle velocities	
		vector posDiffLS = vector.Zero;
		vector rotDiffLS = vector.Zero;

		vector velocity = GetVelocity(vehicle);
		
		if (vehicle)
		{
			vector posDiffWS = velocity * CONST_LINEAR_VELOCITY_STRENGTH;
			posDiffLS = posDiffLS + posDiffWS.InvMultiply3(playerTransformWS);

			vector rotDiffWS = dBodyGetAngularVelocity(vehicle) * CONST_ANGULAR_VELOCITY_STRENGTH;
			rotDiffLS = rotDiffLS + rotDiffWS.InvMultiply3(playerTransformWS);
			
			vector tempRot1 = rotDiffLS;
			rotDiffLS[0] = tempRot1[1];
			rotDiffLS[1] = tempRot1[2];
			rotDiffLS[2] = tempRot1[0];
		}

		//! smooth it!
		m_ExLagOffsetPosition[0] = Math.SmoothCD( m_ExLagOffsetPosition[0], posDiffLS[0], m_ExLagOffsetVelocityX, 0.4, 1000, pDt );
		m_ExLagOffsetPosition[1] = Math.SmoothCD( m_ExLagOffsetPosition[1], posDiffLS[1], m_ExLagOffsetVelocityY, 0.4, 1000, pDt );
		m_ExLagOffsetPosition[2] = Math.SmoothCD( m_ExLagOffsetPosition[2], posDiffLS[2], m_ExLagOffsetVelocityZ, 0.4, 1000, pDt );

		//! setup orientation
		vector rot = m_LagOffsetOrientation * Math.RAD2DEG;
		if (vehicle)
		{
			vector rotTM[3];
			Math3D.YawPitchRollMatrix(rot, rotTM);
			
			Math3D.MatrixInvMultiply3(playerTransformWS, rotTM, rotTM);
			
			// because we don't have m_fIgnoreParentYaw set
			vector tempRot = Math3D.MatrixToAngles(rotTM);
			rot[0] = tempRot[0];
		}
		
		rot[0] = rot[0] * (1.0 + (Math.InverseLerp(0.0, 1.0 + CONST_ANGULAR_LAG_YAW_STRENGTH, Math.AbsFloat(rotDiffLS[0])) * CONST_ANGULAR_LAG_YAW_STRENGTH));
				
		rot = GetCurrentOrientation() + rot;
		
		float distance = m_fDistance;		
		distance -= (2 * Math.Clamp(Math.InverseLerp(UP_ANGLE_CAP * 0.5, CONST_UD_MAX, rot[1]), 0, 1));

		rot[1] = Limit(m_fUpDownAngle + m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX) + CONST_ANGULAR_LAG_PITCH_STRENGTH * m_LagOffsetOrientation[1];
		rot[2] = m_CurrentCameraRoll;
		
		Math3D.YawPitchRollMatrix(rot, pOutResult.m_CameraTM);
		
		//! setup position		
		pOutResult.m_CameraTM[3] = cameraPosition - m_ExLagOffsetPosition;
		
		//! setup rest
		pOutResult.m_fDistance = distance * m_ExClientSettings.VehicleCameraDistance;
	}

	override void Expansion_GetUDAngles(out float pAngle, out float pAngleAdd)
	{
		pAngle = m_fUpDownAngle;
		pAngleAdd = m_fUpDownAngleAdd;
	}
};
