/**
 * DayZPlayerCamera1stPerson.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef DAYZ_1_25
#ifdef EXPANSION_OBSOLETE_CAMERA
modded class DayZPlayerCamera1stPerson
{	
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "OnUpdate").Add(pDt);
#endif

		super.OnUpdate( pDt, pOutResult );

		ExpansionVehicleBase vehicle;
		CarScript car;
		Class.CastTo( car, m_Ex_Player.Expansion_GetParent() );

		if ( Class.CastTo( vehicle, m_Ex_Player.Expansion_GetParent() ) || m_Ex_Player.Expansion_IsAttached() )
		{
			pOutResult.m_fInsideCamera = 0.0;
			m_Ex_Player.SetHeadInvisible_Ex( true );
		}
		else
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY).Call( m_Ex_Player.SetHeadInvisible_Ex, false );
			return;
		}
		
		if ( m_pPlayer.GetCommand_Vehicle() || m_Ex_Player.GetCommand_ExpansionVehicle() )
		{
			return;
		}

		pOutResult.m_bUpdateWhenBlendOut = false;
		pOutResult.m_fIgnoreParentRoll = 0;
		pOutResult.m_iDirectBone = -1;
		pOutResult.m_iDirectBoneMode = 3;
		pOutResult.m_fPositionModelSpace = 0.0;
		pOutResult.m_fUseHeading = 0;
		
		vector rot;
		
		HumanInputController input = m_pPlayer.GetInputController();
		float heading = input.GetHeadingAngle() * Math.RAD2DEG;
		
		if (vehicle)
		{
			heading -= vehicle.GetOrientation()[0];
		}
		
		if (car)
		{
			heading -= car.GetOrientation()[0];
		}

		rot[0] = m_CurrentCameraYaw - heading;
		rot[1] = m_CurrentCameraPitch;
		rot[2] = 0;
			
		vector cameraTransform[4];
		Math3D.YawPitchRollMatrix(rot, cameraTransform);
		cameraTransform[3] = m_pPlayer.GetBonePositionWS(m_iBoneIndex);
			
		vector playerTransform[4];
		m_pPlayer.GetTransform(playerTransform);
			
		Math3D.MatrixInvMultiply4(playerTransform, cameraTransform, pOutResult.m_CameraTM);
		
		pOutResult.m_CameraTM[3] = pOutResult.m_CameraTM[3] + m_OffsetLS;
	}
};
#endif
#endif