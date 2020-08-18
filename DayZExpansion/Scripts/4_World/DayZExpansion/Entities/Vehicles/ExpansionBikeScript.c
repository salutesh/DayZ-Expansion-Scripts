/*
 * ExpansionBikeScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBikeScript
 * @brief		This class handle bicycle simulation
 **/
class ExpansionBikeScript extends ExpansionVehicleScript
{
	protected ExpansionBikeController m_BikeController;

	private float m_Thrust;
	private float m_Turn;

	private float m_TargetThrust;
	private float m_TargetTurn;

	private float m_EngineForce;
	private float m_BrakeForce;

	// ------------------------------------------------------------
	void ExpansionBikeScript()
	{
		m_Controller = new ExpansionBikeController( this );
		Class.CastTo( m_BikeController, m_Controller );
	}
		
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}
	
	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}
	
	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalRadiator()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalTruckBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 3;
	}

	// ------------------------------------------------------------
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		m_TargetTurn = ( m_BikeController.GetTurnRight() - m_BikeController.GetTurnLeft() );
		m_EngineForce = ( m_BikeController.GetForward() );
		m_BrakeForce = ( m_BikeController.GetBackward() );

		if ( Math.AbsFloat( m_TargetTurn ) > Math.AbsFloat( m_Turn ) )
		{
			m_Turn += ( m_TargetTurn - m_Turn ) * pDt * 2.0;
		} else if ( Math.AbsFloat( m_TargetTurn ) < Math.AbsFloat( m_Turn ) )
		{
			m_Turn = 0;
		}
	}

	// ------------------------------------------------------------
	protected override void OnPreSimulation( float pDt )
	{
		super.OnPreSimulation( pDt );

		if ( !m_IsPhysicsHost )
			return;

		SetWheelSteering( 0, m_Turn * 35.0 );

		SetWheelBrakeForce( 0, m_BrakeForce * 5 );
		SetWheelBrakeForce( 1, m_BrakeForce * 5 );

		SetWheelEngineForce( 1, m_EngineForce * 10 );
	}

	// ------------------------------------------------------------
	protected override void OnSimulation( float pDt, out vector force, out vector torque )
	{
		super.OnSimulation( pDt, force, torque );

		float clampedForwardVel = Math.Clamp( m_LinearVelocityMS[2], -1, 1 );
		if ( clampedForwardVel > 0.9 )
		{
			m_AdjustCenterOfMass[2] = clampedForwardVel * 1.5;
		} else if ( clampedForwardVel < -0.9 )
		{
			m_AdjustCenterOfMass[2] = clampedForwardVel * 1.5;
		} else
		{
			m_AdjustCenterOfMass[2] = 0;
		}

		float absForwardSpeed = Math.AbsFloat( m_LinearVelocityMS[2] ) + 0.1;

		vector upDir = m_Transform.data[1];
		ExpansionDebugUI( "Up Dir: " + upDir );

		//if ( upDir[1] > 0.1 )
		//{
			vector terrainSurface = m_Wheels[0].GetSuspensionContactNormal() + m_Wheels[1].GetSuspensionContactNormal();
			terrainSurface = terrainSurface.Normalized().Multiply3( m_Transform.GetBasis().data );

			vector estDirUp = GetEstimatedOrientation( 0.1 ).data[1];
			
			Matrix3 vertical;
			Matrix3.DirectionAndUp( GetDirection(), "0 1 0", vertical );

			ExpansionDebugUI( "Terrain Normal: " + terrainSurface );

			float leanAngle = m_Turn * 35.0 * Math.DEG2RAD;
			float leanAmount = m_Turn; // Math.Tan( leanAngle );
			leanAmount = Math.Clamp( leanAmount, -0.15, 0.15 );
			float leanOnX = leanAmount * 0.1 * absForwardSpeed;
			leanOnX = Math.Clamp( leanOnX, -0.86, 0.86 );

			ExpansionDebugUI( "Lean X: " + leanOnX );

			vector upDirWanted = Vector( -terrainSurface[0], 1.0, -terrainSurface[2] ).Normalized();
			upDirWanted = upDirWanted.InvMultiply3( vertical.data );
			upDirWanted[0] = leanOnX;
			upDirWanted.Normalize();
			upDirWanted = upDirWanted.Multiply3( vertical.data );

			vector stabilize = upDirWanted - estDirUp;
			const float maxStabCoef = 0.8;
			if ( stabilize.LengthSq() > maxStabCoef * maxStabCoef )
				stabilize = stabilize.Normalized() * maxStabCoef;

			stabilize = stabilize * 30.0 * m_BodyMass;

			stabilize = stabilize.InvMultiply3( m_Transform.GetBasis().data );
			stabilize[2] = 0;
			stabilize = stabilize.Multiply3( m_Transform.GetBasis().data );

			stabilize = upDir * stabilize;

			ExpansionDebugUI( "Stabilize: " + stabilize );
			
			torque += stabilize;
		//}

		ExpansionDebugUI();
	}
}