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

	private vector m_SuspensionOffset;

	// wheel animations
	private string m_AnimTurn;
	private string m_AnimRotation;
	private string m_AnimDamper;
	private string m_WheelHub;

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
	private float m_SuspensionLengthPrevious;
	
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
	private float m_AngularRotation;
	
	private float m_Mass;

	private float m_WheelTorque;
	private float m_BrakeTorque;
	
	private float m_Steering;
	
	private string m_Surface;
	
	private ref array< vector > m_WheelVertexPositions;

	void ExpansionVehicleWheel( ExpansionVehicleScript vehicle, ref ExpansionVehicleAxle axle, string name )
	{
		m_WheelVertexPositions = new array< vector >();
		
		m_Name = name;
		m_Vehicle = vehicle;
		m_Axle = axle;

		m_TransformWS = new Transform;
		m_TransformMS = new Transform;
		m_RotationMatrix = new Matrix3;

		m_WheelDirectionMS = "0 -1 0";
		m_WheelAxleMS = "-1 0 0"; // TODO: get from model
		
		string path;

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " inventorySlot";
		m_InventorySlot = GetGame().ConfigGetTextOut( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " animTurn";
		m_AnimTurn = GetGame().ConfigGetTextOut( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " animRotation";
		m_AnimRotation = GetGame().ConfigGetTextOut( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " animDamper";
		m_AnimDamper = GetGame().ConfigGetTextOut( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " wheelHub";
		m_WheelHub = GetGame().ConfigGetTextOut( path );

		m_InitialWheelPositionMS = GetCenterPositionSelection( "geometry", m_WheelHub );
		m_RestLength = m_Axle.GetTravelMax() * 0.5;

		string n_axis_start;
		string n_axis_end;

		m_SuspensionOffset = "0 0 0";

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " axis_start";
		if ( GetGame().ConfigGetText( path, n_axis_start ) )
		{
			path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + axle.GetName() + " Wheels " + name + " axis_end";
			if ( GetGame().ConfigGetText( path, n_axis_end ) )
			{
				m_SuspensionOffset = m_Vehicle.GetMemoryPointPos( n_axis_start );
				m_WheelDirectionMS = vector.Direction( m_Vehicle.GetMemoryPointPos( n_axis_end ), m_SuspensionOffset ).Normalized();
				m_SuspensionOffset = m_InitialWheelPositionMS - m_SuspensionOffset;
			}
		}
	}

	void ~ExpansionVehicleWheel()
	{
		delete m_TransformWS;
		delete m_TransformMS;
		delete m_RotationMatrix;
		
		delete m_WheelVertexPositions;
	}

	float GetAngularVelocity()
	{
		return m_AngularVelocity;
	}

	void SetSteering( float steering )
	{
		m_Steering = steering;
	}

	void ApplyTorque( float torque )
	{
		m_WheelTorque = torque;
	}

	void ApplyBrake( float torque )
	{
		m_BrakeTorque = torque;
	}
	
	private vector GetCenterPositionSelection( string lod_name, string selection_name )
	{
		LOD lod = m_Vehicle.GetLODByName( lod_name );
		if ( !lod )
			return "0 0 0";

		Selection selection = lod.GetSelectionByName( selection_name );
		if ( !selection )
			return "0 0 0";
		
		vector offset = vector.Zero; // m_WheelDirectionMS * m_Axle.GetWheelHubRadius() * -0.5;
		
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
		
		m_Axle.UpdateWheelRadius();
	}

	void OnWheelDetach()
	{
		m_WheelItem = NULL;
	}
	
	ExpansionWheel GetItem()
	{
		return m_WheelItem;
	}

	void SetupSimulation( float pDt, out int numWheelsGrounded )
	{		
		ExpansionDebugUI();
		ExpansionDebugUI( "Wheel " + m_Axle.GetName() + "::" + m_Name + " - " + m_WheelHub );

		if ( !m_WheelItem )
			return;
		
		m_Mass = dBodyGetMass( m_Vehicle );
		
		vector suspensionOffset = (m_SuspensionLength - m_Axle.GetTravelMaxUp()) * m_WheelDirectionMS;

		vector wheelTransform[4];
		wheelTransform[0] = -m_WheelAxleMS;
		wheelTransform[1] = -m_WheelDirectionMS;
		wheelTransform[2] = "0 0 1";
		wheelTransform[3] = m_SuspensionOffset;
		
		vector rotationTransform[4];
		Math3D.YawPitchRollMatrix( Vector( m_Steering, 0, 0 ), rotationTransform );
		rotationTransform[3] = m_WheelVertexPositions[m_WheelVertexPositions.Count() - 1] + suspensionOffset;

		Math3D.MatrixMultiply4( rotationTransform, wheelTransform, m_TransformMS.data );

		Suspension( pDt );

		m_TransformWS = m_Vehicle.m_Transform.Multiply( m_TransformMS );

		ExpansionDebugUI( "Suspension Length: " + m_SuspensionLength );

		if ( m_HasContact )
		{
			numWheelsGrounded++;
		}

		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		for ( int j = 0; j < m_WheelVertexPositions.Count() - 2; ++j )
		{
			vector wvps = m_WheelVertexPositions[j] + suspensionOffset - m_TransformMS[3];
			vector wvpe = m_WheelVertexPositions[j + 1] + suspensionOffset - m_TransformMS[3];
			
			vector pts[2];
			pts[0] = wvps.Multiply4( m_TransformWS.data );
			pts[1] = wvpe.Multiply4( m_TransformWS.data );
			
			int color = 0xFF00FF00;
			if ( !m_HasContact )
				color = 0xFFFF0000;
			
			#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
			m_Vehicle.DBGAddShape( Shape.CreateLines( color, ShapeFlags.NOZBUFFER, pts, 2 ) );
			#endif
		}

		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		m_Vehicle.DBGAddShape( Shape.CreateSphere( 0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_TransformMS[3].Multiply4( m_Vehicle.m_Transform.data ), 0.05 ) );
		#endif
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

		Suspension( pDt, numWheelsGrounded, impulse, impulseTorque );
		Friction( pDt, numWheelsGrounded, impulse, impulseTorque );

		// convert wheel forces to world space
		impulse = impulse.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
		impulseTorque = impulseTorque.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
		
		ExpansionDebugUI( "Linear: " + impulse );
		ExpansionDebugUI( "Angular: " + impulseTorque );

		pImpulse += impulse;
		pImpulseTorque += impulseTorque;

		m_Vehicle.SetAnimationPhase( m_AnimDamper, m_SuspensionFraction );
		m_Vehicle.SetAnimationPhase( m_AnimTurn, m_Steering * Math.DEG2RAD );
		m_Vehicle.SetAnimationPhase( m_AnimRotation, m_AngularRotation );
	}

	private void Suspension( float pDt )
	{
		m_ContactLength = m_Axle.GetTravelMax() + m_WheelItem.m_Radius;

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.VEHICLE|PhxInteractionLayers.ROADWAY|PhxInteractionLayers.TERRAIN|PhxInteractionLayers.ITEM_SMALL|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
		
		vector rotationTransform[4];
		Math3D.YawPitchRollMatrix( Vector( -m_Steering, 0, 0 ), rotationTransform );
		rotationTransform[3] = m_InitialWheelPositionMS;
		
		m_RayStartMS = m_SuspensionOffset.Multiply4(rotationTransform);
		m_RayEndMS = m_RayStartMS + ( m_WheelDirectionMS * (m_Axle.GetTravelMaxDown() + m_WheelItem.m_Radius) );
		m_RayStartMS = m_RayStartMS - ( m_WheelDirectionMS * (m_Axle.GetTravelMaxUp()) );

		m_RayStartWS = m_RayStartMS.Multiply4(m_Vehicle.m_Transform.data);
		m_RayEndWS = m_RayEndMS.Multiply4(m_Vehicle.m_Transform.data);
		
		m_Vehicle.DBGDrawLine( m_RayStartWS, m_RayEndWS, 0xFFFFFFFFF );
		
		float hubRadius = m_Axle.GetWheelHubRadius();

		//ExpansionDebugUI( "Ray Start (MS): " + m_RayStartMS );
		//ExpansionDebugUI( "Ray End (MS): " + m_RayEndMS );
		//ExpansionDebugUI( "Ray Start (WS): " + m_RayStartWS );
		//ExpansionDebugUI( "Ray End (WS): " + m_RayEndWS );

		m_HasContact = DayZPhysics.SphereCastBullet( m_RayStartWS, m_RayEndWS, hubRadius, collisionLayerMask, m_Vehicle, m_ContactObject, m_ContactPositionWS, m_ContactNormalWS, m_ContactFraction );
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
				
				float invWheelDiff = -1.0 / wheelDiff;
				m_SuspensionRelativeVelocity = vector.Dot( m_ContactNormal, m_ContactVelocity ) * invWheelDiff;

				m_SuspensionInvContact = invWheelDiff;
			}
			
			
			ExpansionDebugUI( "wheelDiff: " + wheelDiff );
			ExpansionDebugUI( "inv contact: " + m_SuspensionInvContact );
			ExpansionDebugUI( "rel vel: " + m_SuspensionRelativeVelocity );
		}

		if ( !m_HasContact )
		{
			m_ContactVelocity = "0 0 0";

			m_SuspensionRelativeVelocity = 0.0;
			m_SuspensionInvContact = 10.0;

			m_ContactPositionWS = m_RayEndWS;
			m_ContactNormalWS = m_TransformWS.data[1];

			m_ContactPosition = m_RayEndMS;
			m_ContactNormal = -m_WheelDirectionMS;

			m_ContactFraction = 1.0;
			
			m_Surface = "";
		} else
		{
			/*
			vector temp1;
			vector temp2;
			int temp3;
			set< Object > temp4 = new set< Object >();
			DayZPhysics.RaycastRV( m_RayStartWS, m_RayEndWS, temp1, temp2, temp3, temp4, m_Vehicle, m_Vehicle, false, false, ObjIntersectGeom, hubRadius );
			
			if ( temp4.Count() > 0 )
				m_ContactObject = temp4[0];
			else
				m_ContactObject = null;
			
			if ( !m_Surface || !DayZPhysics.GetHitSurface( m_ContactObject, m_RayStartWS, m_RayEndWS, m_Surface ) )
			{
				m_Surface = "";
			}
			*/
			
			m_Surface = "";
		}
		
		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		m_Vehicle.DBGDrawImpulseMS( m_ContactPosition, m_ContactVelocity * m_Mass * pDt, 0x9900FF00 );
		
		if ( s_SUSP_DEBUG_LENGTH != 0.0 )
		{
			m_Vehicle.DBGDrawLine( m_ContactPositionWS, m_ContactPositionWS - (m_ContactNormal * s_SUSP_DEBUG_LENGTH), 0x9900FF00 );
			m_Vehicle.DBGDrawLine( m_ContactPosition.Multiply4(m_Vehicle.m_Transform.data), m_ContactPosition.Multiply4(m_Vehicle.m_Transform.data) + (m_ContactNormal * s_SUSP_DEBUG_LENGTH), 0x99FF0000 );
		}
		#endif
		
		m_SuspensionFraction = ( m_Axle.GetTravelMax() - m_SuspensionLength ) / m_Axle.GetTravelMax();

		ExpansionDebugUI( "Has Contact: " + m_HasContact );
		ExpansionDebugUI( "Contact Velocity: " + m_ContactVelocity );
	}

	private void Suspension( float pDt, int numWheelsGrounded, out vector impulse, out vector impulseTorque )
	{
		float suspLength = ( m_ContactFraction * m_ContactLength ) - m_WheelItem.m_Radius + m_Axle.GetWheelHubRadius();

		if ( m_HasContact )
		{
			Expansion_Assert_False( numWheelsGrounded != 0, "If m_HasContact is true, atleast 1 wheel must be grounded." );
			
			float invWheelsGrounded = 1.0 / ( numWheelsGrounded );
			float invWheels = 1.0 / ( m_Vehicle.GetNumWheels() );
			
			float ks = m_Axle.GetStiffness();
			float kc = m_Axle.GetCompression();
			float kd = m_Axle.GetDamping();

			float suspDist = ( m_Axle.GetTravelMax() - suspLength ) - m_Axle.GetTravelMaxDown();
			m_SuspensionForce = suspDist * ks * m_SuspensionInvContact;

			float damping = m_SuspensionRelativeVelocity * kc;
			if ( m_SuspensionRelativeVelocity > 0 )
				damping = m_SuspensionRelativeVelocity * kd;
			
			vector susp = m_ContactNormal * m_SuspensionForce;
			susp -= m_ContactNormal * damping;
			susp += Vector(0, -9.8, 0).InvMultiply3( m_Vehicle.m_Transform.data ) * invWheelsGrounded * invWheels * m_Mass;
			susp = susp * pDt;

			impulse += susp;
			impulseTorque += m_ContactPosition * susp;
			
			#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
			//m_Vehicle.DBGDrawImpulseMS( m_ContactPosition + Vector( 0, s_SUSP_DEBUG_LENGTH, 0 ), susp, 0xFFC0D000 );
			#endif
		}

		m_SuspensionLengthPrevious = m_SuspensionLength;
		m_SuspensionLength = suspLength;
	}

	private void Friction( float pDt, int numWheelsGrounded, out vector impulse, out vector impulseTorque )
	{
		if ( !m_HasContact )
			return;
	
		float forwardImpulse = 0;
		
		if ( m_WheelTorque != 0 )
		{
			forwardImpulse = m_WheelTorque * m_Mass / m_WheelItem.m_Radius;
		} else
		{
			forwardImpulse = -vector.Dot( m_ContactVelocity.Normalized(), m_TransformMS[2] ) * m_BrakeTorque;
		}

		forwardImpulse += -m_WheelItem.m_TyreRollResistance * m_ContactVelocity[2];
		
		float sideDot = vector.Dot( m_ContactVelocity.Normalized(), m_TransformMS[0] );
		float sideCoef = 10.0;
		float sideImpulse = sideCoef * m_Mass * -sideDot * m_ContactVelocity.Length() / numWheelsGrounded;

		m_AngularVelocity = m_ContactVelocity[2] / m_WheelItem.m_Radius;
		m_AngularRotation += m_AngularVelocity * pDt;
		if ( m_AngularRotation > Math.PI2 )
			m_AngularRotation -= Math.PI2;
		else if ( m_AngularRotation < 0 )
			m_AngularRotation += Math.PI2;
		
		float surfaceFriction = 0.95;
		
		if ( m_Surface != "" ) 
			surfaceFriction = Surface.GetParamFloat( m_Surface, "friction" );
		
		vector forwardImp = m_TransformMS[2] * forwardImpulse * pDt * surfaceFriction;
		vector sideImp = m_TransformMS[0] * sideImpulse * pDt * surfaceFriction;

		impulse += forwardImp;
		impulseTorque += m_ContactPosition * forwardImp;

		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		//m_Vehicle.DBGDrawImpulseMS( m_ContactPosition, forwardImp, 0xFF00FFFF );
		#endif

		impulse += sideImp;
		impulseTorque += m_ContactPosition * sideImp;

		#ifndef EXPANSION_WHEEL_DEBUG_DISABLE
		//m_Vehicle.DBGDrawImpulseMS( m_ContactPosition, sideImp, 0xFFFFFF00 );
		#endif
	}
};