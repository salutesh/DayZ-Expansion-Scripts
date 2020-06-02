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

	void ExpansionVehicleWheel( ExpansionVehicleScript vehicle, ref ExpansionVehicleAxle axle, string name )
	{
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
	
		OnDebugEnd();
	}

	private EntityAI m_DbgWheelHub;

	void OnDebugStart()
	{
		CreateDebugObject( "ExpansionDebugBox_Blue", m_DbgWheelHub );
	}

	void OnDebugEnd()
	{
		DestroyDebugObject( m_DbgWheelHub );
	}

	void OnDebugUpdate( float pDt )
	{
		m_DbgWheelHub.SetTransform( m_TransformWS.data );
	}

	private vector GetCenterPositionSelection( string lod_name, string selection_name )
	{
		LOD lod = m_Vehicle.GetLODByName( lod_name );
		if ( !lod )
			return "0 0 0";

		Selection selection = lod.GetSelectionByName( selection_name );
		if ( !selection )
			return "0 0 0";
		
		vector totalVertices = "0 0 0";
		int count = selection.GetVertexCount();
		for ( int i = 0; i < count; ++i )
		{
			totalVertices += lod.GetVertexPosition( selection.GetLODVertexIndex( i ) );
		}

		return Vector( totalVertices[0] / count, totalVertices[1] / count, totalVertices[2] / count );
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

		if ( IsMissionClient() )
			DebugStart();
	}

	void OnWheelDetach()
	{
		m_WheelItem = NULL;

		if ( IsMissionClient() )
			DebugEnd();
	}

	void SetupSimulation( float pDt, out int numWheelsGrounded )
	{
		ExpansionDebugUI();
		ExpansionDebugUI( "Wheel " + m_Axle.GetName() + "::" + m_Name + " - " + m_WheelHub );

		if ( !m_WheelItem )
			return;

		Matrix3 basisMat = new Matrix3;
		basisMat.data[0] = -m_WheelAxleMS;
		basisMat.data[1] = -m_WheelDirectionMS;
		basisMat.data[2] = "0 0 1";
		m_RotationMatrix.FromYawPitchRoll( Vector( m_Steering, 0, 0 ) );
		m_TransformMS.SetBasis( basisMat.Multiply( m_RotationMatrix ) );
		m_TransformMS.data[3] = m_InitialWheelPositionMS + ( m_WheelDirectionMS * m_SuspensionLength );

		m_TransformWS = m_Vehicle.m_Transform.Multiply( m_TransformMS );

		Suspension( pDt );

		ExpansionDebugUI( "Suspension Length: " + m_SuspensionLength );

		if ( m_HasContact )
		{
			numWheelsGrounded++;
		}
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
		m_RayEndMS = m_InitialWheelPositionMS + ( m_WheelDirectionMS * m_ContactLength );

		m_RayStartWS = m_Vehicle.ModelToWorld( m_RayStartMS );
		m_RayEndWS = m_Vehicle.ModelToWorld( m_RayEndMS );

		//ExpansionDebugUI( "Ray Start (MS): " + m_RayStartMS );
		//ExpansionDebugUI( "Ray End (MS): " + m_RayEndMS );
		//ExpansionDebugUI( "Ray Start (WS): " + m_RayStartWS );
		//ExpansionDebugUI( "Ray End (WS): " + m_RayEndWS );

		m_HasContact = DayZPhysics.RayCastBullet( m_RayStartWS, m_RayEndWS, collisionLayerMask, m_Vehicle, m_ContactObject, m_ContactPositionWS, m_ContactNormalWS, m_ContactFraction );
		if ( m_HasContact )
		{
			m_ContactPosition = m_Vehicle.WorldToModel( m_ContactPositionWS ) - dBodyGetCenterOfMass( m_Vehicle );
			m_ContactNormal = m_ContactNormalWS.InvMultiply3( m_Vehicle.m_Transform.GetBasis().data );

			// m_ContactPositionWS = m_Vehicle.ModelToWorld( m_ContactPosition );

			float wheelDiff = vector.Dot( m_ContactNormal, m_WheelDirectionMS );
			if ( wheelDiff >= -0.1 )
			{
				m_HasContact = false;
			} else
			{
				m_ContactVelocity = m_Vehicle.GetModelVelocityAt( m_ContactPosition );
				// m_ContactVelocity = m_Vehicle.GetWorldVelocityAt( m_ContactPosition ).InvMultiply3( m_TransformWS.GetBasis().data );

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

			m_ContactPosition = m_RayEndMS - dBodyGetCenterOfMass( m_Vehicle );
			m_ContactNormal = -m_WheelDirectionMS;

			m_ContactFraction = 1.0;
		}

		m_SuspensionFraction = ( m_Axle.GetTravelMax() - m_SuspensionLength ) / m_Axle.GetTravelMax();

		ExpansionDebugUI( "Has Contact: " + m_HasContact );
		ExpansionDebugUI( "Contact Velocity: " + m_ContactVelocity );
	}

	private void Suspension( float pDt, out vector impulse, out vector impulseTorque )
	{
		float suspLength = ( m_ContactFraction * m_ContactLength ) - m_WheelItem.m_Radius;
		if ( suspLength < 0 )
			suspLength = 0;

		float mass = dBodyGetMass( m_Vehicle );
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

			vector susp = -1.0 * m_WheelDirectionMS * m_SuspensionForce * pDt;

			impulse += susp;
			impulseTorque += m_ContactPosition * susp;
		}

		m_SuspensionLength = suspLength;
	}

	private void Friction( float pDt, int numWheelsGrounded, out vector impulse, out vector impulseTorque )
	{
		if ( !m_HasContact )
			return;

		float mass = dBodyGetMass( m_Vehicle );

		m_AxleWS = -m_TransformWS.data[0];

		vector surfNormalWS = m_ContactNormalWS;

		float proj = vector.Dot( m_AxleWS, surfNormalWS );
		m_AxleWS -= surfNormalWS * proj;
		m_AxleWS.Normalize();

		m_ForwardWS = surfNormalWS * m_AxleWS;
		m_ForwardWS.Normalize();

		m_ForwardWS = m_ForwardWS.InvMultiply3( m_Vehicle.m_Transform.GetBasis().data );
		m_AxleWS = m_AxleWS.InvMultiply3( m_Vehicle.m_Transform.GetBasis().data );

		float sideImpulse = 0; // ExpansionPhysics.ResolveSingleBilateral( m_Vehicle, m_ContactPositionWS, m_ContactVelocity, m_Vehicle, m_ContactPositionWS, m_AxleWS );
		float forwardImpulse = 0;

		forwardImpulse = m_EngineForce - m_BrakeForce;

		sideImpulse = -vector.Dot( m_ContactVelocity.Normalized(), m_AxleWS ) * m_ContactVelocity.Length() / numWheelsGrounded;
/*
		if ( m_EngineForce != 0 )
		{
			forwardImpulse = m_EngineForce - m_BrakeForce;
		} else
		{
			float maxImpulse = 0;
			if ( m_BrakeForce )
				maxImpulse = m_BrakeForce;
				
        	forwardImpulse = CalculateRollingFriction( numWheelsGrounded, maxImpulse );
		}
*/

		float maximp = m_SuspensionForce * pDt * 10.0;
		float maximpSide = maximp;

		float maximpSquared = maximp * maximpSide;

		float x = forwardImpulse * 0.5;
		float y = sideImpulse * 1.0;

		float impulseSquared = (x * x + y * y);

		if (impulseSquared > maximpSquared)
		{
		}

		m_AngularVelocity = m_ContactVelocity[2] * m_WheelItem.m_Radius;

		vector forwardImp = m_ForwardWS * forwardImpulse * mass * pDt;
		vector sideImp = m_AxleWS * sideImpulse * mass * pDt;

		impulse += forwardImp;
		impulseTorque += m_ContactPosition * forwardImp;

		impulse += sideImp;
		impulseTorque += m_ContactPosition * sideImp;
	}

/////////////////////////////////////////////////////////////////////////////////////////////
// DEBUGGING HELPERS
/////////////////////////////////////////////////////////////////////////////////////////////

	void DebugStart()
	{
		m_IsDebugging = true;

		OnDebugStart();
	}

	void DebugEnd()
	{
		m_IsDebugging = false;

		OnDebugEnd();

		ClearForces();
	}

	void DebugUpdate( float pDt )
	{
		if ( !m_IsDebugging )
			return;

		OnDebugUpdate( pDt );

		m_Forces.Resize( m_ForceCount );

		for ( int i = 0; i < m_ForceCount; ++i )
		{
			m_Forces[i].UpdateObject( m_Vehicle );
		}

		m_ForceCount = 0;
	}

	private void AddForce( ExpansionForceColour colour_type, vector pos, vector force )
	{
		string colour = "";
		switch ( colour_type )
		{
		case ExpansionForceColour.GREEN:
			colour = "ExpansionDebugArrow";
			break;
		case ExpansionForceColour.RED:
			colour = "ExpansionDebugArrow_Red";
			break;
		case ExpansionForceColour.ORANGE:
			colour = "ExpansionDebugArrow_Orange";
			break;
		case ExpansionForceColour.BLUE:
			colour = "ExpansionDebugArrow_Blue";
			break;
		case ExpansionForceColour.PURPLE:
			colour = "ExpansionDebugArrow_Purple";
			break;
		}

		if ( colour == "" )
			return;

		ExpansionForceDebug efd;
		if ( m_ForceCount >= m_Forces.Count() )
		{
			efd = new ExpansionForceDebug;
			efd.m_Colour = colour;

			efd.Recreate();

			m_Forces.Insert( efd );
		} else
		{
			efd = m_Forces[m_ForceCount];
			efd.m_Colour = colour;

			if ( efd.m_Type != colour_type )
			{
				efd.Recreate();
			}
		}

		efd.m_Type = colour_type;
		efd.m_Position = pos;
		efd.m_Force = force;

		efd.UpdateObject( m_Vehicle );
		
		m_ForceCount++;
	}

	private void ClearForces()
	{
		for ( int i = 0; i < m_Forces.Count(); ++i )
		{
			DestroyDebugObject( m_Forces[i].m_Object );
		}

		m_Forces.Clear();
	}

	private autoptr array< ref ExpansionForceDebug > m_Forces = new array< ref ExpansionForceDebug >;
	private int m_ForceCount;

	private bool m_IsDebugging;

	private void CreateDebugObject( string obj_name, out EntityAI obj )
	{
		DestroyDebugObject( obj );

		obj = EntityAI.Cast( GetGame().CreateObject( obj_name, "0 0 0", true ) );
		if ( obj )
			dBodyDestroy( obj );
	}

	private void DestroyDebugObject( EntityAI obj )
	{
		if ( obj )
			GetGame().ObjectDelete( obj );
	}
}

enum ExpansionForceColour
{
	GREEN,
	RED,
	ORANGE,
	BLUE,
	PURPLE
}

class ExpansionForceDebug : Managed
{
	ExpansionForceColour m_Type;
	string m_Colour;
	vector m_Position;
	vector m_Force;

	EntityAI m_Object;

	void ~ExpansionForceDebug()
	{
		if ( m_Object )
			GetGame().ObjectDelete( m_Object );
	}

	void Recreate()
	{
		if ( m_Object )
			GetGame().ObjectDelete( m_Object );

		m_Object = EntityAI.Cast( GetGame().CreateObject( m_Colour, m_Position, true ) );
		if ( m_Object )
			dBodyDestroy( m_Object );
	}

	void UpdateObject( ExpansionVehicleScript veh )
	{
		vector position = veh.ModelToWorld( m_Position );
		vector forceDir = m_Force;
		float forceLen = forceDir.Normalize();

		m_Object.SetPosition( position );
		m_Object.SetDirection( forceDir );
		m_Object.SetOrientation( forceDir.VectorToAngles() );
		m_Object.SetAnimationPhase( "scale", forceLen );
	}
}