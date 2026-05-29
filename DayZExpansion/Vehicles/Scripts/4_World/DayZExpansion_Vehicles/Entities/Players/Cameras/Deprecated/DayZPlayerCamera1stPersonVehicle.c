/**
 * DayZPlayerCamera1stPersonVehicle.c
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
modded class DayZPlayerCamera1stPersonVehicle
{
	void DayZPlayerCamera1stPersonVehicle(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_Expansion_CameraHandler = new ExpansionCameraHandler(this);
	}

	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "OnUpdate").Add(pDt);
#endif

		IEntity parent = m_Ex_Player.Expansion_GetParent();

		m_Expansion_CameraHandler.OnUpdate(pDt, m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd);

		bool isFreeLook = true;
		bool isHeliParent = false;

		ExpansionVehicleHelicopter_OLD simulation;
		ExpansionPhysicsState physicsState;

		ExpansionHelicopterScript d_heli;
		if ( Class.CastTo( d_heli, parent ) )
		{
			isHeliParent = true;
			isFreeLook = d_heli.IsFreeLook();

			simulation = d_heli.m_Simulation;
			physicsState = d_heli.m_State;
		}

		ExpansionVehicleHelicopterBase heli;
		if ( Class.CastTo( heli, parent ) )
		{
			isHeliParent = true;
			isFreeLook = heli.IsFreeLook();

			physicsState = heli.m_State;
		}

		if (simulation)
		{
			if (simulation.m_VRSSeverity > 0.0)
			{
				float shakeStrength = physicsState.m_LinearVelocity[1] / -30.0 * simulation.m_VRSSeverity;
				m_fLeftRightAngleAdd += Math.RandomFloat(-shakeStrength, shakeStrength);
				m_fUpDownAngleAdd += Math.RandomFloat(-shakeStrength, shakeStrength);
			}
		}

		super.OnUpdate( pDt, pOutResult );

		ExpansionVehicleBase exVehicle;
		m_bForceFreeLook = Class.CastTo( exVehicle, parent );
		m_bForceFreeLook = false;
		
		pOutResult.m_fUseHeading			= 0.0;
		
		if ( exVehicle )
		{
			pOutResult.m_fUseHeading		= 1.0;
			pOutResult.m_fInsideCamera 		= 0.0;
			m_Ex_Player.SetHeadInvisible_Ex( true );
		} else
		{
			m_Ex_Player.SetHeadInvisible_Ex( false );
		}

		m_ExIsFreeLook = isFreeLook;

		if ( isHeliParent && !isFreeLook && m_Ex_Player.Expansion_IsDriver() )
		{
			Math3D.YawPitchRollMatrix( "0 0 0", pOutResult.m_CameraTM );

			pOutResult.m_fUseHeading 			= 0.0;
			pOutResult.m_fInsideCamera 			= 1.0;
			pOutResult.m_fPositionModelSpace	= 1.0;
			//pOutResult.m_CameraTM[3] 			= //m_pPlayer.GetBonePositionMS( m_iBoneIndex ) + m_OffsetLS;
		}

	}
};
#endif
