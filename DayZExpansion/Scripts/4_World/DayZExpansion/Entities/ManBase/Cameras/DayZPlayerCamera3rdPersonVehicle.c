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
		
		#ifdef DAYZ_1_10
		ExpansionVehicleScript expVehicle;
		if ( Class.CastTo( expVehicle, m_pPlayer.GetParent() ) )
		{
			m_fDistance 		= expVehicle.GetCameraDistance();
			m_CameraOffsetMS	= Vector( 0, expVehicle.GetCameraHeight(), 0 );
		}
		#else
		if ( m_pPlayer )
		{
			CarScript vehicle;
			ExpansionVehicleScript expVehicle;
			if ( Class.CastTo( vehicle, m_pPlayer.GetParent() ) )
			{
				if ( vehicle.IsInherited( ExpansionHelicopterScript ) )
				{
					m_fDistance 			= 12.0;
					m_CameraOffsetMS		= "0.0 1.0 0.0";	
					m_PreviousOrientation	= vehicle.GetOrientation();
				} else if ( vehicle.IsInherited( ExpansionPlaneScript ) )
				{
					m_fDistance 			= vehicle.GetCameraDistance();
					m_CameraOffsetMS		= "0.0 2.0 0.0";	
				} else if ( vehicle.IsInherited( ExpansionBoatScript ) )
				{
					m_fDistance 			= vehicle.GetCameraDistance();
					m_CameraOffsetMS		= Vector( 0, vehicle.GetCameraHeight(), 0 );
				} else if ( vehicle.IsInherited( ExpansionBulldozerScript ) )
				{
					m_fDistance 			= 6.0;
					m_CameraOffsetMS		= "0.0 1.5 0.0";	
				} else if ( vehicle.IsInherited( ExpansionBus ) )
				{
					m_fDistance 			= 12.0;
					m_CameraOffsetMS		= "0.0 1.5 0.0";	
				} else 
				{
					m_fDistance 			= 4.0;
					m_CameraOffsetMS		= "0.0 1.3 0.0";
				}
			} else if ( Class.CastTo( expVehicle, m_pPlayer.GetParent() ) )
			{
				m_fDistance 				= expVehicle.GetCameraDistance();
				m_CameraOffsetMS			= Vector( 0, expVehicle.GetCameraHeight(), 0 );
			}
		} else 
		{
			m_fDistance 		= 4.0;
			m_CameraOffsetMS	= "0.0 1.3 0.0";
		}
		#endif

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

		ExpansionVehicleScript exVehicle;
		if ( Class.CastTo( exVehicle, m_pPlayer.GetParent() ) )
		{
			pOutResult.m_fUseHeading	= 1.0;

			/* Found a workaround for the crash, clients will always now have dynamic vehicles, TODO: test and see if the simulation is still smooth
			if ( pOutResult.m_CollisionIgnoreEntity == NULL && !exVehicle.IsPhysicsHost() )
			{
				//! TODO: fix crash when this is uncommented
				//pOutResult.m_CollisionIgnoreEntity = exVehicle;
			}
			*/
		}

		ExpansionHelicopterScript helicopter;
		if ( Class.CastTo( helicopter, m_pPlayer.GetParent() ) )
			OnUpdateHelicopter( pDt, helicopter, pOutResult );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera3rdPersonVehicle::OnUpdate - End");
		#endif
	}

	void OnUpdateHelicopter( float pDt, ExpansionHelicopterScript pHelicopter, out DayZPlayerCameraResult pOutResult )
	{
		vector playerTransformWS[4];
		m_pPlayer.GetTransform( playerTransformWS );

		vector vehiclePositionWS = pHelicopter.GetOrigin();
		vector vehiclePositionMS = vehiclePositionWS.InvMultiply4( playerTransformWS );
		vector cameraPosition = vehiclePositionMS + m_CameraOffsetMS;

		vector newOrientation = pHelicopter.GetOrientation();
		vector orientDiff = vector.Zero;

		orientDiff[0] = Math.DiffAngle( m_PreviousOrientation[0], newOrientation[0] );
		orientDiff[1] = Math.DiffAngle( m_PreviousOrientation[1], newOrientation[1] );
		orientDiff[2] = Math.DiffAngle( m_PreviousOrientation[2], newOrientation[2] );

		vector posDiffWS = GetVelocity( pHelicopter ) * pDt;
		vector posDiffLS = posDiffWS.InvMultiply3( playerTransformWS );

		vector rotation = vector.Zero;

		//! smooth it!
		m_ExLagOffsetPosition[0] = Math.SmoothCD( m_ExLagOffsetPosition[0], posDiffLS[0], m_ExLagOffsetVelocityX, 0.4, 1000, pDt );
		m_ExLagOffsetPosition[1] = Math.SmoothCD( m_ExLagOffsetPosition[1], posDiffLS[1], m_ExLagOffsetVelocityY, 0.4, 1000, pDt );
		m_ExLagOffsetPosition[2] = Math.SmoothCD( m_ExLagOffsetPosition[2], posDiffLS[2], m_ExLagOffsetVelocityZ, 0.4, 1000, pDt );

		m_ExLagOffsetOrientation[1] = Math.SmoothCD( m_ExLagOffsetOrientation[1], -newOrientation[1], m_ExLagOffsetVelocityPitch, 0.3, 1000, pDt );

		if ( !pHelicopter.IsFreeLook() )
		{
			m_fLeftRightAngle = 0;

			rotation[0] = m_fLeftRightAngle;
			rotation[1] = m_ExLagOffsetOrientation[1];

			Math3D.YawPitchRollMatrix( rotation, pOutResult.m_CameraTM );
		}
			
		pOutResult.m_CameraTM[3] 			= cameraPosition - m_ExLagOffsetPosition;

		pOutResult.m_fIgnoreParentRoll		= 1.0;
		pOutResult.m_fInsideCamera		  	= 0.0;
		pOutResult.m_iDirectBone			= -1.0;
		pOutResult.m_fUseHeading			= 0.0;
		pOutResult.m_fDistance 				= m_fDistance;
		pOutResult.m_fPositionModelSpace	= 1.0;

		m_PreviousOrientation				= newOrientation;
	}
}