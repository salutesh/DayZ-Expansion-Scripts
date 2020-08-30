/**
 * ExpansionVehicleWheel.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleWheel
{
	private static float s_SUSP_DEBUG_LENGTH = 0.0;

	private string m_InventorySlot;
	private string m_Selection;
	private string m_Name;
	private ExpansionVehicleAxle m_Axle;
	private ExpansionWheel m_WheelItem;
	private ExpansionVehicleScript m_Vehicle;

	private ref Transform m_TransformWS;
	private ref Transform m_TransformMS;
	private ref Matrix3 m_RotationMatrix;

	private vector m_InitialWheelPositionMS;

	private vector m_WheelDirectionMS;
	private vector m_WheelAxleMS;

	// wheel animations
	private string m_AnimTurn;
	private string m_AnimRotation;
	private string m_AnimDamper;
	private string m_WheelHub;

	private float m_AnimWheelPos;

	// Suspension
	private bool m_HasContact;
	private Object m_ContactObject;
	private vector m_ContactPosition;
	private vector m_ContactNormal;
	private float m_ContactFraction;
	private float m_ContactLength;

	private vector m_ContactPositionWS;
	private vector m_ContactNormalWS;

	private float m_SuspensionLength;
	private float m_SuspensionFraction;
	private float m_RestLength;

	private float m_SuspensionForce;

	private float m_SuspensionRelativeVelocity;
	private float m_SuspensionInvContact;

	private vector m_ContactVelocity;

	private vector m_TireLongDir;
	private vector m_TireLatDir;
	private float m_TireLongSpeed;
	private float m_TireLatSpeed;

	private vector m_RayStartMS;
	private vector m_RayStartWS;
	private vector m_RayEndMS;
	private vector m_RayEndWS;

	private vector m_ForwardWS;
	private vector m_AxleWS;

	private float m_AngularVelocity;

	float m_EngineForce;
	float m_BrakeForce;
	float m_Steering;
	
	private ref array< Shape > m_DebugShapes;
	private ref array< vector > m_WheelVertexPositions;
	private float m_TimeSlice;
	private float m_Mass;

	void ExpansionVehicleWheel( ExpansionVehicleScript vehicle, ref ExpansionVehicleAxle axle, string name )
	{
		m_DebugShapes = new array< Shape >();
		m_WheelVertexPositions = new array< vector >();
		
		m_Name = name;
		m_Vehicle = vehicle;
		m_Axle = axle;

		m_TransformWS = new Transform;
		m_TransformMS = new Transform;
		m_RotationMatrix = new Matrix3;

		m_WheelDirectionMS = "0 -1 0"; // TODO: get from model
		m_WheelAxleMS = "-1 0 0"; // TODO: get from model

		string inventory_slot_path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " inventorySlot";
		m_InventorySlot = GetGame().ConfigGetTextOut( inventory_slot_path );

		string anim_turn_path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " animTurn";
		m_AnimTurn = GetGame().ConfigGetTextOut( anim_turn_path );
		
		string anim_rotation_path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " animRotation";
		m_AnimRotation = GetGame().ConfigGetTextOut( anim_rotation_path );
		
		string anim_damper_path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " animDamper";
		m_AnimDamper = GetGame().ConfigGetTextOut( anim_damper_path );
		
		string wheel_hub_path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " wheelHub";
		m_WheelHub = GetGame().ConfigGetTextOut( wheel_hub_path );

		m_InitialWheelPositionMS = GetCenterPositionSelection( "geometry", m_WheelHub );
		m_RestLength = m_Axle.GetTravelMax() * 0.5;
	}

	void ~ExpansionVehicleWheel()
	{
		delete m_TransformWS;
		delete m_TransformMS;
		delete m_RotationMatrix;
		
		for ( int i = 0; i < m_DebugShapes.Count(); i++ )
			m_DebugShapes[i].Destroy();
		
		delete m_DebugShapes;
		delete m_WheelVertexPositions;
	}
	
	private vector GetCenterPositionSelection( string lod_name, string selection_name )
	{
		LOD lod = m_Vehicle.GetLODByName( lod_name );
		if ( !lod )
			return "0 0 0";

		Selection selection = lod.GetSelectionByName( selection_name );
		if ( !selection )
			return "0 0 0";
		
		vector offset = m_WheelDirectionMS * m_Axle.GetWheelHubRadius() * -0.5;
		
		vector totalVertices = "0 0 0";
		int count = selection.GetVertexCount();
		for ( int i = 0; i < count; ++i )
		{
			vector vp = lod.GetVertexPosition( selection.GetLODVertexIndex( i ) ) + offset;
			totalVertices += vp;
			m_WheelVertexPositions.Insert( vp );
		}
		
		vector center = Vector( totalVertices[0] / count, totalVertices[1] / count, totalVertices[2] / count );
		m_WheelVertexPositions.Insert( center );
		return center;
	}

	string GetInventorySlot()
	{
		return m_InventorySlot;
	}

	ExpansionVehicleAxle GetAxle()
	{
		return m_Axle;
	}

	float GetSuspensionLength()
	{
		return m_SuspensionLength;
	}

	float GetSuspensionFraction()
	{
		return m_SuspensionFraction;
	}

	vector GetSuspensionContactPosition()
	{
		return m_ContactPosition;
	}

	vector GetSuspensionContactNormal()
	{
		return m_ContactNormal;
	}

	vector GetInitialWheelPosition()
	{
		return m_InitialWheelPositionMS;
	}
	
	void ExpansionDebugUI( string message = "" )
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_WHEELS, message );
	}

	void OnWheelAttach( notnull ExpansionWheel wheel, bool isFromPrevious = false )
	{
		m_WheelItem = wheel;
	}

	void OnWheelDetach()
	{
		m_WheelItem = NULL;
	}

	void SetupSimulation( float pDt, out int numWheelsGrounded )
	{
		m_TimeSlice = pDt;
		m_Mass = dBodyGetMass( m_Vehicle );
		
		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		for ( int i = 0; i < m_DebugShapes.Count(); ++i )
			m_DebugShapes[i].Destroy();

		m_DebugShapes.Clear();
		#endif

		ExpansionDebugUI();
		ExpansionDebugUI( "Wheel " + m_Axle.GetName() + "::" + m_Name + " - " + m_WheelHub );

		if ( !m_WheelItem )
			return;

		m_Steering = m_Axle.GetSteering() * m_Axle.GetMaxSteeringAngle();

		Matrix3 basisMat = new Matrix3;
		basisMat.data[0] = -m_WheelAxleMS;
		basisMat.data[1] = -m_WheelDirectionMS;
		basisMat.data[2] = "0 0 1";
		m_RotationMatrix.FromYawPitchRoll( Vector( m_Steering, 0, 0 ) );
		m_TransformMS.SetBasis( basisMat.Multiply( m_RotationMatrix ) );
		m_TransformMS.data[3] = m_WheelVertexPositions[m_WheelVertexPositions.Count() - 1] + ( m_SuspensionLength * m_WheelDirectionMS );

		m_TransformWS = m_Vehicle.m_Transform.Multiply( m_TransformMS );

		Suspension( pDt );

		ExpansionDebugUI( "Suspension Length: " + m_SuspensionLength );

		if ( m_HasContact )
		{
			numWheelsGrounded++;
		}

		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		for ( int j = 0; j < m_WheelVertexPositions.Count() - 2; ++j )
		{
			vector wvps = m_WheelVertexPositions[j] + ( m_SuspensionLength * m_WheelDirectionMS ) - m_TransformMS[3];
			vector wvpe = m_WheelVertexPositions[j + 1] + ( m_SuspensionLength * m_WheelDirectionMS ) - m_TransformMS[3];
			
			vector pts[2];
			pts[0] = wvps.Multiply4( m_TransformWS.data );
			pts[1] = wvpe.Multiply4( m_TransformWS.data );
			
			int color = 0xFF00FF00;
			if ( !m_HasContact )
				color = 0xFFFF0000;
			
			m_DebugShapes.Insert( Shape.CreateLines( color, ShapeFlags.NOZBUFFER, pts, 2 ) );
		}
			
		m_DebugShapes.Insert( Shape.CreateSphere( 0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_TransformMS[3].Multiply4( m_Vehicle.m_Transform.data ), 0.05 ) );
		#endif
	}

	void Simulate( float pDt, int numWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		ExpansionDebugUI();
		ExpansionDebugUI( "Wheel " + m_Axle.GetName() + "::" + m_Name + " - " + m_WheelHub );
		
		if ( !m_WheelItem )
			return;

		vector impulse;
		vector impulseTorque;

		Suspension( pDt, impulse, impulseTorque );
		Friction( pDt, numWheelsGrounded, impulse, impulseTorque );

		// convert wheel forces to world space
		impulse = impulse.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
		impulseTorque = impulseTorque.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
		
		ExpansionDebugUI( "Linear: " + impulse );
		ExpansionDebugUI( "Angular: " + impulseTorque );

		pImpulse += impulse;
		pImpulseTorque += impulseTorque;

		m_AnimWheelPos += ( m_AngularVelocity / m_WheelItem.m_Radius ) * pDt;
		if ( m_AnimWheelPos > Math.PI2 )
			m_AnimWheelPos -= Math.PI2;
		else if ( m_AnimWheelPos < 0 )
			m_AnimWheelPos += Math.PI2;

		m_Vehicle.SetAnimationPhase( m_AnimDamper, m_SuspensionFraction );
		m_Vehicle.SetAnimationPhase( m_AnimTurn, m_Steering * Math.DEG2RAD );
		m_Vehicle.SetAnimationPhase( m_AnimRotation, m_AnimWheelPos );
	}

	private void Suspension( float pDt )
	{
		m_ContactLength = m_Axle.GetTravelMax() + m_WheelItem.m_Radius;

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.VEHICLE|PhxInteractionLayers.ROADWAY|PhxInteractionLayers.TERRAIN|PhxInteractionLayers.ITEM_SMALL|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
		
		m_RayStartMS = m_InitialWheelPositionMS;
		m_RayEndMS = m_RayStartMS + ( m_WheelDirectionMS * m_ContactLength );

		m_RayStartWS = m_RayStartMS.Multiply4(m_Vehicle.m_Transform.data);
		m_RayEndWS = m_RayEndMS.Multiply4(m_Vehicle.m_Transform.data);
		
		//DrawLine( m_RayStartWS, m_RayEndWS, 0xFFFFFFFFF );

		//ExpansionDebugUI( "Ray Start (MS): " + m_RayStartMS );
		//ExpansionDebugUI( "Ray End (MS): " + m_RayEndMS );
		//ExpansionDebugUI( "Ray Start (WS): " + m_RayStartWS );
		//ExpansionDebugUI( "Ray End (WS): " + m_RayEndWS );

		m_HasContact = DayZPhysics.RayCastBullet( m_RayStartWS, m_RayEndWS, collisionLayerMask, m_Vehicle, m_ContactObject, m_ContactPositionWS, m_ContactNormalWS, m_ContactFraction );
		if ( m_HasContact )
		{
			m_ContactPosition = m_ContactPositionWS.InvMultiply4(m_Vehicle.m_Transform.data);
			m_ContactNormal = m_ContactNormalWS.InvMultiply3( m_Vehicle.m_Transform.data );

			float wheelDiff = vector.Dot( m_ContactNormal, m_WheelDirectionMS );
			if ( wheelDiff >= -0.1 )
			{
				m_HasContact = false;
			} else
			{
				m_ContactVelocity = m_Vehicle.GetModelVelocityAt( m_ContactPosition );
				//m_ContactVelocity = m_Vehicle.GetWorldVelocityAt( m_ContactPosition ).InvMultiply3( m_TransformWS.GetBasis().data );

				float invWheelDiff = -1.0 / wheelDiff;
				m_SuspensionRelativeVelocity = vector.Dot( m_ContactNormal, m_ContactVelocity ) * invWheelDiff;

				m_SuspensionInvContact = invWheelDiff;
			}
			// ExpansionDebugUI( "wheelDiff: " + wheelDiff );
			// ExpansionDebugUI( "inv contact: " + m_SuspensionInvContact );
			// ExpansionDebugUI( "rel vel: " + m_SuspensionRelativeVelocity );
		}

		if ( !m_HasContact )
		{
			m_ContactVelocity = "0 0 0";

			m_AngularVelocity *= 0.5;

			m_SuspensionRelativeVelocity = 0.0;
			m_SuspensionInvContact = 10.0;

			m_ContactPositionWS = m_RayEndWS;
			m_ContactNormalWS = m_TransformWS.data[1];

			m_ContactPosition = m_RayEndMS;
			m_ContactNormal = -m_WheelDirectionMS;

			m_ContactFraction = 1.0;
		}
		
		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		ShowImpulseMS( m_ContactPosition, m_ContactVelocity * m_Mass * pDt, 0x9900FF00 );
		
		if ( s_SUSP_DEBUG_LENGTH != 0.0 )
		{
			DrawLine( m_ContactPositionWS, m_ContactPositionWS - (m_ContactNormal * s_SUSP_DEBUG_LENGTH), 0x9900FF00 );
			DrawLine( m_ContactPosition.Multiply4(m_Vehicle.m_Transform.data), m_ContactPosition.Multiply4(m_Vehicle.m_Transform.data) + (m_ContactNormal * s_SUSP_DEBUG_LENGTH), 0x99FF0000 );
		}
		#endif
		
		m_SuspensionFraction = ( m_Axle.GetTravelMax() - m_SuspensionLength ) / m_Axle.GetTravelMax();

		ExpansionDebugUI( "Has Contact: " + m_HasContact );
		ExpansionDebugUI( "Contact Velocity: " + m_ContactVelocity );
	}

	private void Suspension( float pDt, out vector impulse, out vector impulseTorque )
	{
		float suspLength = ( m_ContactFraction * m_ContactLength ) - m_WheelItem.m_Radius;
		if ( suspLength < 0 )
			suspLength = 0;

		float invWheels = 1.0 / m_Vehicle.GetNumWheels();

		if ( m_HasContact )
		{
			float ks = m_Axle.GetStiffness();
			float kc = m_Axle.GetCompression();
			float kd = m_Axle.GetDamping();

			float relDiff = ( m_Axle.GetTravelMax() - suspLength ) / m_Axle.GetTravelMax();
			m_SuspensionForce = relDiff * ks * m_SuspensionInvContact * invWheels;

			float relVel = m_SuspensionRelativeVelocity;
			if ( relVel < 0 )
			{
				m_SuspensionForce -= relVel * kc;
			} else
			{
				m_SuspensionForce -= relVel * kd;
			}

			vector susp =  -1.0 * m_WheelDirectionMS * m_SuspensionForce * pDt;

			impulse += susp;
			impulseTorque += m_ContactPosition * susp;
			
			#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
			ShowImpulseMS( m_ContactPosition + Vector( 0, s_SUSP_DEBUG_LENGTH, 0 ), susp, 0xFFC0D000 );
			#endif
		}

		m_SuspensionLength = suspLength;
	}

	private void Friction( float pDt, int numWheelsGrounded, out vector impulse, out vector impulseTorque )
	{
		if ( !m_HasContact )
			return;

		float sideImpulse = 0;
		float forwardImpulse = 0;
	
		forwardImpulse = m_EngineForce;
		if ( forwardImpulse != 0.0 )
			forwardImpulse -= 1.5 * vector.Dot( m_ContactVelocity.Normalized(), m_TransformMS[2] ) * m_ContactVelocity.Length() / numWheelsGrounded;
		forwardImpulse -= vector.Dot( m_ContactVelocity.Normalized(), m_TransformMS[2] ) * m_BrakeForce / m_Mass;
		
		float sideDot = vector.Dot( m_ContactVelocity.Normalized(), m_TransformMS[0] );
		//sideDot *= Math.AbsFloat( sideDot );
		float sideCoef = 4.0;
		sideImpulse = sideCoef * -sideDot * m_ContactVelocity.Length() / numWheelsGrounded;

		float maximp = m_SuspensionForce * pDt * 10.0;
		float maximpSide = maximp;

		float maximpSquared = maximp * maximpSide;

		float x = forwardImpulse * 0.5;
		float y = sideImpulse * 1.0;

		float impulseSquared = (x * x + y * y);

		m_AngularVelocity = m_ContactVelocity[2] * m_WheelItem.m_Radius;

		vector forwardImp = m_TransformMS[2] * forwardImpulse * m_Mass * pDt;
		vector sideImp = m_TransformMS[0] * sideImpulse * m_Mass * pDt;

		impulse += forwardImp;
		impulseTorque += m_ContactPosition * forwardImp;

		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		ShowImpulseMS( m_ContactPosition, forwardImp, 0xFF00FFFF );
		#endif

		impulse += sideImp;
		impulseTorque += m_ContactPosition * sideImp;

		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		ShowImpulseMS( m_ContactPosition, sideImp, 0xFFFFFF00 );
		#endif
	}
	
	void ShowImpulseMS( vector position, vector impulse, int color = 0x44FFFFFF )
	{
		ShowImpulse( position.Multiply4( m_Vehicle.m_Transform.data ), impulse.Multiply3( m_Vehicle.m_Transform.GetBasis().data ), color );
	}

	void ShowImpulse( vector position, vector impulse, int color = 0x44FFFFFF )
	{
		vector acceleration = impulse;
		acceleration[0] = acceleration[0] / m_TimeSlice / m_Mass;
		acceleration[1] = acceleration[1] / m_TimeSlice / m_Mass;
		acceleration[2] = acceleration[2] / m_TimeSlice / m_Mass;
		
		DrawLine( position, position + acceleration, color );
	}

	void DrawLine( vector start, vector end, int color = 0x44FFFFFF )
	{
		vector pts[2]
		pts[0] = start;
		pts[1] = end;
		
		m_DebugShapes.Insert( Shape.CreateLines( color, ShapeFlags.TRANSP | ShapeFlags.NOZBUFFER, pts, 2 ) );
	}
};