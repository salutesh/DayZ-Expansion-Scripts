/**
 * DayZPlayerCamera1stPerson.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCamera1stPerson
{
	private bool m_NextFrame_AssignInsideCamera;
	
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		super.OnUpdate( pDt, pOutResult );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPerson::OnUpdate Start");
		#endif
		
		pOutResult.m_fInsideCamera = 1.0;

		ExpansionVehicleBase vehicle;
		CarScript car;
		Class.CastTo( car, m_pPlayer.GetParent() );

		PlayerBase m_pbPlayer = PlayerBase.Cast( m_pPlayer );
		
		if ( Class.CastTo( vehicle, m_pPlayer.GetParent() ) || m_pbPlayer.IsAttaching() )
		{
			pOutResult.m_fInsideCamera = 0.0;
			
			m_pbPlayer.SetHeadInvisible_Ex( true );
		} else
		{			
			pOutResult.m_fInsideCamera = 1.0;
			
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY).Call( m_pbPlayer.SetHeadInvisible_Ex, false );
		}
		
		if ( m_pPlayer.GetCommand_Vehicle() || m_pbPlayer.GetCommand_ExpansionVehicle() )
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

		rot[0] = m_fLeftRightAngle - heading;
		rot[1] = m_CurrentCameraPitch;
		rot[2] = 0;
			
		vector cameraTransform[4];
		Math3D.YawPitchRollMatrix(rot, cameraTransform);
		cameraTransform[3] = m_pPlayer.GetBonePositionWS(m_iBoneIndex);
			
		vector playerTransform[4];
		m_pPlayer.GetTransform(playerTransform);
			
		Math3D.MatrixInvMultiply4(playerTransform, cameraTransform, pOutResult.m_CameraTM);
		
		pOutResult.m_CameraTM[3] = pOutResult.m_CameraTM[3] + m_OffsetLS;	
			
		//DisplayMatrix4("CameraTM", pOutResult.m_CameraTM);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPerson::OnUpdate End");
		#endif
	}
	
	void DisplayMatrix4(string name, vector matrix[4])
	{
	}
	
	void DisplayMatrix3(string name, vector matrix[3])
	{
	}
};