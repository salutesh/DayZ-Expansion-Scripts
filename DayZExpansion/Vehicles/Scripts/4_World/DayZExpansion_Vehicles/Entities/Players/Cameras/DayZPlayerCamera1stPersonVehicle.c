/**
 * DayZPlayerCamera1stPersonVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCamera1stPersonVehicle
{
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		super.OnUpdate( pDt, pOutResult );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPersonVehicle::OnUpdate Start");
		#endif

		ExpansionVehicleBase exVehicle;
		m_bForceFreeLook = Class.CastTo( exVehicle, m_pPlayer.GetParent() );
		m_bForceFreeLook = false;
		
		pOutResult.m_fUseHeading			= 0.0;
		
		if ( exVehicle )
		{
			pOutResult.m_fUseHeading		= 1.0;
			pOutResult.m_fInsideCamera 		= 0.0;
			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( true );
		} else
		{
			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( false );
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

		if ( isHeliParent && !isFreeLook )
		{
			m_fUpDownAngle = 0;
			m_fLeftRightAngle = 0;

			m_fUpDownAngleAdd = 0;
			m_fLeftRightAngle = 0;
			
			Math3D.YawPitchRollMatrix( "0 0 0", pOutResult.m_CameraTM );
			pOutResult.m_fUseHeading = 0;
			pOutResult.m_CameraTM[3] = m_pPlayer.GetBonePositionMS( m_iBoneIndex ) + m_OffsetLS;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPersonVehicle::OnUpdate End");
		#endif
	}
}