/**
 * DayZPlayerCamera1stPerson.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		pOutResult.m_bUpdateWhenBlendOut = false;
		pOutResult.m_fIgnoreParentRoll = 0;
		pOutResult.m_iDirectBone = -1;
		pOutResult.m_iDirectBoneMode = 3;
		pOutResult.m_fPositionModelSpace = 0.0;
		pOutResult.m_fUseHeading = 1;

		ExpansionVehicleBase vehicle;
		if ( Class.CastTo( vehicle, m_pPlayer.GetParent() ) )
		{
			pOutResult.m_fInsideCamera = 0;
			
			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( true );
		} else
		{			
			pOutResult.m_fInsideCamera = 1.0;
			
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY).Call( PlayerBase.Cast( m_pPlayer ).SetHeadInvisible, false );
		}
		
		vector rot;
		
		HumanInputController input = m_pPlayer.GetInputController();
		float heading = 0; //input.GetHeadingAngle();

		if ( m_pPlayer.GetCommand_Vehicle() || PlayerBase.Cast( m_pPlayer ).GetCommand_ExpansionVehicle() )
		{
			heading = 0;
		}

		rot[0] = m_fLeftRightAngle;
		rot[1] = m_CurrentCameraPitch;
		rot[2] = 0;
		
		/*
		if (vehicle)
		{
			Debug.DestroyAllShapes();
		
			rot[1] = 0;
			rot[0] = rot[0];
			
			vector tempA[4];
			vector tempB[4];
			vector tempC[4];
			
			vector parentTransform[4];
			vehicle.GetTransform(parentTransform);
						
			vector playerTransform[4];
			m_pPlayer.GetTransform(playerTransform);
			Math3D.YawPitchRollMatrix(Vector(heading, 0, 0), playerTransform);
			Math3D.MatrixMultiply3(playerTransform, parentTransform, tempA);
			
			Debug.DrawLine(tempA[3].Multiply4(parentTransform), tempA[3].Multiply4(parentTransform) + tempA[0].Multiply3(parentTransform), 0xFFFFFF00, ShapeFlags.NOZBUFFER);
			Debug.DrawLine(tempA[3].Multiply4(parentTransform), tempA[3].Multiply4(parentTransform) + tempA[1].Multiply3(parentTransform), 0xFFFF00FF, ShapeFlags.NOZBUFFER);
			Debug.DrawLine(tempA[3].Multiply4(parentTransform), tempA[3].Multiply4(parentTransform) + tempA[2].Multiply3(parentTransform), 0xFFFFFFFF, ShapeFlags.NOZBUFFER);
			
			vector cameraTransform[4];
			Math3D.MatrixIdentity4(cameraTransform);
			Math3D.YawPitchRollMatrix(rot, cameraTransform);
			
			Math3D.MatrixInvMultiply3(tempA, cameraTransform, tempB);
			Math3D.MatrixInvMultiply3(tempB, parentTransform, pOutResult.m_CameraTM);
		} else*/
		{
			rot[0] = rot[0] + heading;
			Math3D.YawPitchRollMatrix( rot, pOutResult.m_CameraTM );
		}
		
		pOutResult.m_CameraTM[3] = m_pPlayer.GetBonePositionMS( m_iBoneIndex ) + m_OffsetLS;
		
		//ExpansionDebugger.Push(EXPANSION_DEBUG_PLAYER);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPerson::OnUpdate End");
		#endif
	}
}