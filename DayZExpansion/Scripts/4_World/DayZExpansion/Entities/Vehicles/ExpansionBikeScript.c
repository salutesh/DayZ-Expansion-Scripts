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

class ExpansionGearbox
{
	private float _reverse;
	private ref array< float > _ratios;
	
	void ExpansionGearbox(ExpansionVehicleScript vehicle)
	{
		string path;
		
		_ratios = new array< float >();
		path = "CfgVehicles " + vehicle.GetType() + " VehicleSimulation Gearbox ratios";
		GetGame().ConfigGetFloatArray( path, _ratios);
		
		path = "CfgVehicles " + vehicle.GetType() + " VehicleSimulation Gearbox reverse";
		_reverse = GetGame().ConfigGetFloat( path );
	}
	
	void ~ExpansionGearbox()
	{
		delete _ratios;
	}

	int Count()
	{
		return _ratios.Count() + 2;
	}
	
	float Get( int gear )
	{
		if ( gear == CarGear.REVERSE )
			return -_reverse;
		if ( gear == CarGear.NEUTRAL )
			return 0;
		
		return _ratios[gear - 2];
	}
};

/**@class		ExpansionBikeScript
 * @brief		This class handle bicycle simulation
 **/
class ExpansionBikeScript extends ExpansionVehicleScript
{
	private ExpansionBikeController m_BikeController;

	private float m_Thrust;
	private float m_Turn;

	private float m_TargetThrust;
	private float m_TargetTurn;

	private float m_Brake;
	
	private ref ExpansionGearbox m_Gearbox;
	
	private float m_TorqueMax;
	private float m_TorqueRPM;
	
	private float m_PowerMax;
	private float m_PowerRPM;
	
	private float m_RPMIdle;
	private float m_RPMMin;
	private float m_RPMClutch;
	private float m_RPMRedline;
	private float m_RPMMax;
	
	private float m_RPM;
	
	private float m_Throttle;

	void ExpansionBikeScript()
	{
		Class.CastTo( m_BikeController, m_Controller );
		
		m_Gearbox = new ExpansionGearbox( this );
		
		string path;
		
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine torqueMax";
		m_TorqueMax = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine torqueRpm";
		m_TorqueRPM = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine powerMax";
		m_PowerMax = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine powerRpm";
		m_PowerRPM = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine rpmIdle";
		m_RPMIdle = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine rpmMin";
		m_RPMMin = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine rpmClutch";
		m_RPMClutch = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine rpmRedline";
		m_RPMRedline = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + GetType() + " VehicleSimulation Engine rpmMax";
		m_RPMMax = GetGame().ConfigGetFloat( path );
	}
	
	void ~ExpansionBikeScript()
	{
		delete m_Gearbox;
	}

	override ExpansionController GetControllerInstance()
	{
		return new ExpansionBikeController( this );
	}

	ExpansionBikeController GetController()
	{
		return m_BikeController;
	}

	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}
	
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}
	
	override bool IsVitalCarBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	override bool IsVitalRadiator()
	{
		return false;
	}
	
	override bool IsVitalGlowPlug()
	{
		return false;
	}

	override bool IsVitalEngineBelt()
	{
		return false;
	}

	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override float GetCameraDistance()
	{
		return 3;
	}
	
	override float EngineGetRPMMax()
	{
		return m_RPMMax;
	}

	override float EngineGetRPMRedline()
	{
		return m_RPMRedline;
	}

	override float EngineGetRPM()
	{
		return m_RPM;
	}

	int GetGearsCount()
	{
		return m_Gearbox.Count();
	}
	
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		m_TargetTurn = ( m_BikeController.GetTurnRight() - m_BikeController.GetTurnLeft() );
		
		m_Throttle = m_BikeController.GetForward();
		m_Brake = m_BikeController.GetBackward();

		float absForwardSpeed = Math.AbsFloat( m_LinearVelocityMS[2] ) + 1.0;

		if ( Math.AbsFloat( m_TargetTurn ) > Math.AbsFloat( m_Turn ) )
		{
			m_Turn += ( m_TargetTurn - m_Turn ) * pDt * ( 3.0 / absForwardSpeed );
		} else if ( Math.AbsFloat( m_TargetTurn ) < Math.AbsFloat( m_Turn ) )
		{
			m_Turn += ( m_TargetTurn - m_Turn ) * pDt * ( 50.0 / absForwardSpeed );
		}
	}

	protected override void OnPreSimulation( float pDt )
	{
		super.OnPreSimulation( pDt );

		if ( !m_IsPhysicsHost )
			return;
		
		if ( EngineIsOn() )
		{
			m_RPM += m_Throttle * 1000.0 * pDt;
			m_RPM -= 0.2 * m_RPM * pDt;
			
			m_RPM = Math.Clamp( m_RPM, m_RPMIdle, m_RPMMax );
		} else
		{
			m_RPM = 0;
			
			m_Brake = 1.0;
		}
		
		int gear = m_BikeController.GetGear();
		if ( m_Throttle == 0.0 && gear != CarGear.NEUTRAL && !m_HasDriver )
		{
			m_Brake = 1.0;
		}
		
		if ( m_HasDriver )
		{
			float ratio = m_Gearbox[gear];
			
			float torque = 0.0;
			float scale = 0.0;
			
			if ( m_RPM > m_TorqueRPM )
			{
				scale = ( m_RPMMax - m_RPM ) / ( m_RPMMax - m_TorqueRPM );
				torque = m_TorqueMax * scale;
			} else
			{
				scale = ( m_RPM - m_RPMMin ) / ( m_TorqueRPM - m_RPMMin );
				torque = m_TorqueMax * scale;
			}
			
			if ( torque < 0.0 )
				torque = 0.0;
			
			float wheelTorque = 0.0;
			if ( ratio != 0 )
				wheelTorque = m_Throttle * torque / ratio;
			
			ApplyAxleBrake( 0, m_Brake );
			ApplyAxleBrake( 1, m_Brake );
			
			ApplyAxleSteering( 0, m_Turn );
			
			ApplyAxleTorque( 1, wheelTorque );
		} else
		{
			ApplyAxleBrake( 0, 1.0 );
			ApplyAxleBrake( 1, 1.0 );
			
			ApplyAxleSteering( 0, 0.0 );
			ApplyAxleSteering( 1, 0.0 );
			
			ApplyAxleTorque( 0, 0.0 );
			ApplyAxleTorque( 1, 0.0 );
		}
	}

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

		//if ( m_HasDriver )
		{
			vector terrainSurface = m_Wheels[0].GetSuspensionContactNormal() + m_Wheels[1].GetSuspensionContactNormal();
			terrainSurface = terrainSurface.Normalized().Multiply3( m_Transform.GetBasis().data );

			vector estDirUp = GetEstimatedOrientation( pDt * 0.1 )[1].Normalized();
			
			Matrix3 vertical;
			Matrix3.DirectionAndUp( GetDirection(), "0 1 0", vertical );
			ExpansionDebugUI( "Terrain Normal: " + terrainSurface );

			float leanAmount = Math.Clamp( m_Turn * 0.1, -0.16, 0.16 );
			float leanOnX = leanAmount * absForwardSpeed;
			leanOnX = Math.Clamp( leanOnX, -0.86, 0.86 );

			ExpansionDebugUI( "Lean X: " + leanOnX );

			vector upDirWanted = Vector( -terrainSurface[0], 1.0, -terrainSurface[2] ).Normalized();
			upDirWanted = upDirWanted.InvMultiply3( vertical.data );
			upDirWanted[0] = leanOnX;
			upDirWanted.Normalize();
			upDirWanted = upDirWanted.Multiply3( vertical.data );

			DBGDrawLineDirectionMS( "0 0 0.1", upDirWanted, 0x88FF00FF );
		
			vector applyPosition = Vector( 0.0, 2.0 * m_BoundingRadius, 0.0 );

			vector stabilize = (upDirWanted - estDirUp);
			stabilize = stabilize.Normalized() * stabilize.LengthSq();
			const float maxStabCoef = 0.8;
			if ( stabilize.LengthSq() > maxStabCoef * maxStabCoef )
				stabilize = stabilize.Normalized() * maxStabCoef;
			
			stabilize = stabilize * 50.0 * m_BodyMass;

			stabilize = stabilize.InvMultiply3( m_Transform.GetBasis().data );
			stabilize[2] = 0;
		
			vector stabilizeForce = stabilize;
			vector stabilizeTorque = applyPosition * stabilize;
		
			force += stabilizeForce.Multiply3( m_Transform.GetBasis().data );
			torque += stabilizeTorque.Multiply3( m_Transform.GetBasis().data );
		}

		ExpansionDebugUI();
	}
}