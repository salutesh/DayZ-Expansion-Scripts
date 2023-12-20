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
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "OnUpdate").Add(pDt);
#endif

		super.OnUpdate( pDt, pOutResult );
		
		ExpansionVehicleBase exVehicle;
		m_bForceFreeLook = Class.CastTo( exVehicle, m_pPlayer.GetParent() );
		m_bForceFreeLook = false;
		
		pOutResult.m_fUseHeading			= 0.0;

		PlayerBase pb = PlayerBase.Cast(m_pPlayer);
		
		if ( exVehicle )
		{
			pOutResult.m_fUseHeading		= 1.0;
			pOutResult.m_fInsideCamera 		= 0.0;
			pb.SetHeadInvisible_Ex( true );
		} else
		{
			pb.SetHeadInvisible_Ex( false );
		}

		bool isFreeLook = false;
		bool isHeliParent = false;

		ExpansionHelicopterScript d_heli;
		if ( Class.CastTo( d_heli, m_pPlayer.GetParent() ) )
		{
			isHeliParent = true;
			isFreeLook = d_heli.IsFreeLook();
		}

		ExpansionVehicleHelicopterBase heli;
		if ( Class.CastTo( heli, m_pPlayer.GetParent() ) )
		{
			isHeliParent = true;
			isFreeLook = heli.IsFreeLook();
		}

		if ( isHeliParent && !isFreeLook && pb.Expansion_IsDriver() )
		{
			m_fUpDownAngle = 0;
			m_fLeftRightAngle = 0;

			m_fUpDownAngleAdd = 0;
			m_fLeftRightAngle = 0;
			
			Math3D.YawPitchRollMatrix( "0 0 0", pOutResult.m_CameraTM );

			pOutResult.m_fUseHeading 			= 0.0;
			pOutResult.m_fInsideCamera 			= 1.0;
			pOutResult.m_fPositionModelSpace	= 1.0;
			//pOutResult.m_CameraTM[3] 			= //m_pPlayer.GetBonePositionMS( m_iBoneIndex ) + m_OffsetLS;
		}

	}
};
#endif
