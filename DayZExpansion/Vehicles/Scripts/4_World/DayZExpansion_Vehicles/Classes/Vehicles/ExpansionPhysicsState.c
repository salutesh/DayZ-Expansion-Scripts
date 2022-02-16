class ExpansionPhysicsState
{
	EntityAI m_Entity;
	float m_DeltaTime;

	vector m_Impulse;
	vector m_ImpulseTorque;

	vector m_LinearVelocity;   // World Space
	vector m_LinearVelocityMS; // Model Space

	vector m_LinearAcceleration;   // World Space
	vector m_LinearAccelerationMS; // Model Space

	vector m_LastLinearVelocity;   // World Space
	vector m_LastLinearVelocityMS; // Model Space

	vector m_AngularVelocity;	// World Space
	vector m_AngularVelocityMS; // Model Space

	vector m_AngularAcceleration;	// World Space
	vector m_AngularAccelerationMS; // Model Space

	vector m_LastAngularVelocity;	// World Space
	vector m_LastAngularVelocityMS; // Model Space

	vector m_SyncLinearVelocity;	  // World Space
	vector m_SyncLinearAcceleration;  // World Space
	vector m_SyncAngularVelocity;	  // World Space
	vector m_SyncAngularAcceleration; // World Space

	vector m_Transform[4];
	vector m_TargetTransform[4];

	int m_Time;
	float m_TimeSince;

	bool m_Exploded;
	bool m_HasDriver;
	float m_AltitudeLimiter;

	// Deprecated
	float m_MaxSpeed;	// (km/h)
	float m_MaxSpeedMS; // (m/s)

	float m_AltitudeFullForce; // (m)
	float m_AltitudeNoForce;   // (m)

	float m_BoundingRadius;
	vector m_BoundingBox[2];

	float m_Mass;
	float m_InvMass;
	vector m_Center;

	float m_LinearVelocityX;
	float m_LinearVelocityY;
	float m_LinearVelocityZ;
	float m_AngularVelocityX;
	float m_AngularVelocityY;
	float m_AngularVelocityZ;
	float m_LinearAccelerationX;
	float m_LinearAccelerationY;
	float m_LinearAccelerationZ;
	float m_AngularAccelerationX;
	float m_AngularAccelerationY;
	float m_AngularAccelerationZ;

	float m_ForcePilotSyncTick;

	void ExpansionPhysicsState(EntityAI vehicle)
	{
		m_Entity = vehicle;
	}

	void RegisterSync(string varName)
	{
		m_Entity.RegisterNetSyncVariableInt(varName + ".m_Time");

		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearVelocityX", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearVelocityY", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearVelocityZ", 0, 0, 4);

		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_AngularVelocityX", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_AngularVelocityY", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_AngularVelocityZ", 0, 0, 4);

		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearAccelerationX", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearAccelerationY", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearAccelerationZ", 0, 0, 4);

		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_AngularAccelerationX", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_AngularAccelerationY", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_AngularAccelerationZ", 0, 0, 4);
	}

	void Init()
	{
		m_BoundingRadius = m_Entity.ClippingInfo(m_BoundingBox);
		m_MaxSpeedMS = m_MaxSpeed * (1.0 / 3.6);
	}

	void Derive(inout ExpansionPhysicsState derived, float dt)
	{
		dt *= m_DeltaTime;

		derived.m_LastLinearVelocity = m_LinearVelocity;
		derived.m_LastAngularVelocity = m_AngularVelocity;

		derived.m_LinearVelocity = derived.m_LinearVelocity + (derived.m_LinearAcceleration * dt);
		derived.m_AngularVelocity = derived.m_AngularVelocity + (derived.m_AngularAcceleration * dt);

		vector futureAngularVelocity = derived.m_AngularVelocity * dt;

		derived.m_Transform[0][0] = 0.0;
		derived.m_Transform[1][1] = 0.0;
		derived.m_Transform[2][2] = 0.0;

		derived.m_Transform[0][1] = -futureAngularVelocity[2];
		derived.m_Transform[1][0] = futureAngularVelocity[2];
		derived.m_Transform[2][0] = -futureAngularVelocity[1];
		derived.m_Transform[0][2] = futureAngularVelocity[1];
		derived.m_Transform[1][2] = -futureAngularVelocity[0];
		derived.m_Transform[2][1] = futureAngularVelocity[0];

		Math3D.MatrixInvMultiply3(derived.m_Transform, m_Transform, derived.m_Transform);

		derived.m_Transform[0] = m_Transform[0] + derived.m_Transform[0];
		derived.m_Transform[1] = m_Transform[1] + derived.m_Transform[1];
		derived.m_Transform[2] = m_Transform[2] + derived.m_Transform[2];

		derived.m_Transform[0].Normalize();
		derived.m_Transform[1].Normalize();
		derived.m_Transform[2].Normalize();

		derived.m_Transform[3] = m_Transform[3] + (derived.m_LinearVelocity * dt);
	}

	void CreateDynamic()
	{
		int physLayer = PhxInteractionLayers.VEHICLE;
		int interactLayer = PhxInteractionLayers.VEHICLE | PhxInteractionLayers.VEHICLE_NOTERRAIN;
		
		m_Entity.CreateDynamicPhysics(physLayer);

		vector bbox[2];
		float radius = m_Entity.ClippingInfo(bbox);

		//!breaks vehicles at high speed
		m_Entity.SetDynamicPhysicsLifeTime(-1);

		dBodyDynamic(m_Entity, true);
		dBodyActive(m_Entity, ActiveState.ALWAYS_ACTIVE);
		dBodySetInteractionLayer(m_Entity, interactLayer);
		dBodyEnableGravity(m_Entity, false);

		//EnableDynamicCCD( true );
		dBodyEnableCCD(m_Entity, radius, radius * 0.45);

		dBodySetSleepingTreshold(m_Entity, 0.0, 0.0);
		dBodySetDamping(m_Entity, 0, 0);

		PreSimulate(m_DeltaTime);

		SetVelocity(m_Entity, m_LinearVelocity);
		dBodySetAngularVelocity(m_Entity, m_AngularVelocity);

		auto item = ItemBase.Cast(m_Entity);
		if (item)
			item.EnableCollisionsWithCharacter(false);
	}

	void OnVariablesSynchronized(bool isPhysHost)
	{
		m_TimeSince = (m_Time - m_Entity.GetSimulationTimeStamp()) / 1000.0;
		if (m_TimeSince < 0)
			m_TimeSince = 0;

		//m_LastLinearVelocity = m_LinearVelocity;
		m_SyncLinearVelocity = Vector(m_LinearVelocityX, m_LinearVelocityY, m_LinearVelocityZ);
		m_SyncLinearAcceleration = Vector(m_LinearAccelerationX, m_LinearAccelerationY, m_LinearAccelerationZ);

		//m_LastAngularVelocity = m_AngularVelocity;
		m_SyncAngularVelocity = Vector(m_AngularVelocityX, m_AngularVelocityY, m_AngularVelocityZ);
		m_SyncAngularAcceleration = Vector(m_AngularAccelerationX, m_AngularAccelerationY, m_AngularAccelerationZ);
	}

	void PreSimulate(float pDt)
	{
		m_DeltaTime = pDt;

		m_Entity.GetTransform(m_Transform);

		m_Mass = dBodyGetMass(m_Entity);
		m_InvMass = 1.0 / m_Mass;
		m_Center = dBodyGetCenterOfMass(m_Entity);
	}

	void PostSimulate(float pDt, bool isPhysHost, ExpansionVehicleNetworkMode mode, bool isServer, DayZPlayerImplement driver = NULL)
	{
		if (dBodyIsActive(m_Entity) && dBodyIsDynamic(m_Entity))
		{
			dBodyApplyImpulse(m_Entity, m_Impulse);
			dBodyApplyTorqueImpulse(m_Entity, m_ImpulseTorque);
		}

		m_Impulse = vector.Zero;
		m_ImpulseTorque = vector.Zero;

		if (!isPhysHost)
			return;

		m_LastLinearVelocity = m_LinearVelocity;
		m_LinearVelocity = GetVelocity(m_Entity);
		m_LinearAcceleration = (m_LastLinearVelocity - m_LinearVelocity) * m_DeltaTime;

		m_LastAngularVelocity = m_AngularVelocity;
		m_AngularVelocity = dBodyGetAngularVelocity(m_Entity);
		m_AngularAcceleration = (m_LastAngularVelocity - m_AngularVelocity) * m_DeltaTime;

		m_LastLinearVelocityMS = m_LinearVelocityMS;
		m_LinearVelocityMS = m_LinearVelocity.InvMultiply3(m_Transform);
		m_LinearAccelerationMS = (m_LastLinearVelocityMS - m_LinearVelocityMS) * m_DeltaTime;

		m_LastAngularVelocityMS = m_AngularVelocityMS;
		m_AngularVelocityMS = m_AngularVelocity.InvMultiply3(m_Transform);
		m_AngularAccelerationMS = (m_LastAngularVelocityMS - m_AngularVelocityMS) * m_DeltaTime;

		if (isServer && mode == ExpansionVehicleNetworkMode.CLIENT)
			return;
		else if (!isServer && mode == ExpansionVehicleNetworkMode.SERVER)
			return;

		float forcePilotSyncIntervalSeconds = GetExpansionSettings().GetVehicle().ForcePilotSyncIntervalSeconds;
		if (!forcePilotSyncIntervalSeconds)
			return;

		m_ForcePilotSyncTick += pDt;
		if (m_ForcePilotSyncTick > forcePilotSyncIntervalSeconds)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint(ToString() + "::PostSimulate - ExpansionVehicleRPC.ClientSync");
			#endif

			m_ForcePilotSyncTick = 0;

			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(m_Entity.GetSimulationTimeStamp());
			rpc.Write(m_Transform[3]);
			rpc.Write(Math3D.MatrixToAngles(m_Transform));
/*
			rpc.Write(m_LinearVelocity);
			rpc.Write(m_AngularVelocity);
			rpc.Write(m_LinearAcceleration);
			rpc.Write(m_AngularAcceleration);
*/
			if ( isServer )
			{
				if (driver && driver.GetIdentity())
					rpc.Send(m_Entity, ExpansionVehicleRPC.ClientSync, false, driver.GetIdentity());
			}
			else
			{
				rpc.Send(m_Entity, ExpansionVehicleRPC.ClientSync, false, NULL);
			}
		}
	}

	void OnRPC(ParamsReadContext ctx)
	{
		int time;
		ctx.Read(time);

		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::OnRPC - time " + time);
		#endif

		// check if this is an old state and if so, remove it
		if (m_Time > time)
			return;

		CarScript car;
		if (Class.CastTo(car, m_Entity) && car.Expansion_IsBeingTowed())
			return;

		if (time - m_Time <= GetExpansionSettings().GetVehicle().ForcePilotSyncIntervalSeconds * 1000)
			return;

		m_Time = time;
		m_TimeSince = (m_Entity.GetSimulationTimeStamp() - m_Time) / 1000.0;
		if (m_TimeSince < 0)
			m_TimeSince = 0;

		vector pos;
		vector ori;

		ctx.Read(pos);
		ctx.Read(ori);

		Math3D.YawPitchRollMatrix(ori, m_TargetTransform);
		m_TargetTransform[3] = pos;

		m_Entity.SetTransform(m_TargetTransform);

		if (GetGame().IsServer())
		{
			Transport transport;
			if (Class.CastTo(transport, m_Entity))
				transport.Synchronize();
		}
/*
		ctx.Read(m_SyncLinearVelocity);
		ctx.Read(m_SyncAngularVelocity);

		ctx.Read(m_SyncLinearAcceleration);
		ctx.Read(m_SyncAngularAcceleration);
*/
	}

	vector GetModelVelocityAt(vector relPos)
	{
		return m_LinearVelocityMS + (m_AngularVelocityMS * relPos);
	}

	vector GetWorldVelocityAt(vector relPos)
	{
		return (m_LinearVelocityMS + (m_AngularVelocityMS * relPos)).Multiply3(m_Transform);
	}

	void CalculateAltitudeLimiter()
	{
		float altitude = m_Entity.GetPosition()[1];

		if (altitude > m_AltitudeFullForce)
		{
			if (altitude > m_AltitudeNoForce)
			{
				m_AltitudeLimiter = 0;
			}
			else
			{
				m_AltitudeLimiter = 1 - (altitude - m_AltitudeFullForce) * (1 / (m_AltitudeNoForce - m_AltitudeFullForce));
			}
		}
		else
		{
			m_AltitudeLimiter = 1;
		}
	}

	vector EstimatePosition(float pDt)
	{
		pDt += m_TimeSince;

		return m_Transform[3] + (m_LinearVelocity * pDt) + (m_LinearAcceleration * pDt);
	}

	vector EstimateDirection(float pDt, int direction = 0)
	{
		pDt += m_TimeSince;

		vector futureAngularVelocity = (m_AngularVelocity * pDt) + (m_AngularAcceleration * pDt);
		vector mat[3];

		mat[0][0] = 0.0;
		mat[1][1] = 0.0;
		mat[2][2] = 0.0;

		mat[0][1] = -futureAngularVelocity[2];
		mat[1][0] = futureAngularVelocity[2];
		mat[2][0] = -futureAngularVelocity[1];
		mat[0][2] = futureAngularVelocity[1];
		mat[1][2] = -futureAngularVelocity[0];
		mat[2][1] = futureAngularVelocity[0];

		Math3D.MatrixInvMultiply3(mat, m_Transform, mat);

		mat[0] = m_Transform[0] + mat[0];
		mat[1] = m_Transform[1] + mat[1];
		mat[2] = m_Transform[2] + mat[2];

		mat[0].Normalize();
		mat[1].Normalize();
		mat[2].Normalize();

		return mat[direction];
	}

	vector EstimateOrientation(float pDt)
	{
		vector mat[4];
		EstimateTransform(pDt, mat);
		return Math3D.MatrixToAngles(mat);
	}

	void EstimateTransform(float pDt, inout vector mat[4])
	{
		pDt += m_TimeSince;

		vector futureAngularVelocity = (m_AngularVelocity * pDt) + (m_AngularAcceleration * pDt);

		mat[0][0] = 0.0;
		mat[1][1] = 0.0;
		mat[2][2] = 0.0;

		mat[0][1] = -futureAngularVelocity[2];
		mat[1][0] = futureAngularVelocity[2];
		mat[2][0] = -futureAngularVelocity[1];
		mat[0][2] = futureAngularVelocity[1];
		mat[1][2] = -futureAngularVelocity[0];
		mat[2][1] = futureAngularVelocity[0];

		Math3D.MatrixInvMultiply3(mat, m_Transform, mat);

		mat[0] = m_Transform[0] + mat[0];
		mat[1] = m_Transform[1] + mat[1];
		mat[2] = m_Transform[2] + mat[2];

		mat[0].Normalize();
		mat[1].Normalize();
		mat[2].Normalize();

		mat[3] = m_Transform[3] + (m_LinearVelocity * pDt) + (m_LinearAcceleration * pDt);
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Time 0", m_Time);
		instance.Add("Time 1", m_Entity.GetSimulationTimeStamp());
		instance.Add("Time Since", m_TimeSince);
		instance.Add("Impulse", m_Impulse);
		instance.Add("Impulse Torque", m_ImpulseTorque);
		instance.Add("Linear Velocity (WS)", m_LinearVelocity);
		instance.Add("Linear Velocity (MS)", m_LinearVelocityMS);
		instance.Add("Angular Velocity (WS)", m_AngularVelocity);
		instance.Add("Angular Velocity (MS)", m_AngularVelocityMS);

		return true;
	}
#endif

	void DBGDrawImpulseMS(vector position, vector impulse, int color = 0x44FFFFFF)
	{
		vector acceleration = impulse;
		acceleration[0] = acceleration[0] / m_DeltaTime / m_Mass;
		acceleration[1] = acceleration[1] / m_DeltaTime / m_Mass;
		acceleration[2] = acceleration[2] / m_DeltaTime / m_Mass;

		DBGDrawLineMS(position, position + acceleration, color);
	}

	void DBGDrawImpulse(vector position, vector impulse, int color = 0x44FFFFFF)
	{
		DBGDrawImpulse(position.InvMultiply4(m_Transform), impulse.InvMultiply3(m_Transform), color);
	}

	void DBGDrawLineMS(vector start, vector end, int color = 0x44FFFFFF)
	{
		vector pts[2];
		pts[0] = DBGFixDebugPosition(start);
		pts[1] = DBGFixDebugPosition(end);

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		DBGAddShape(Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.NOZBUFFER, pts, 2));
#endif
	}

	void DBGDrawLine(vector start, vector end, int color = 0x44FFFFFF)
	{
		DBGDrawLineMS(start.InvMultiply4(m_Transform), end.InvMultiply4(m_Transform), color);
	}

	void DBGDrawSphereMS(vector position, float size, int color = 0x44FFFFFF)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		position = DBGFixDebugPosition(position);

		DBGAddShape(Shape.CreateSphere(color, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, position, size));
#endif
	}

	void DBGDrawSphere(vector position, float size, int color = 0x44FFFFFF)
	{
		DBGDrawSphereMS(position.InvMultiply4(m_Transform), size, color);
	}

	void DBGDrawLineDirectionMS(vector start, vector direction, int color = 0x44FFFFFF)
	{
		DBGDrawLineMS(start, start + direction, color);
	}

	void DBGDrawLineDirection(vector start, vector direction, int color = 0x44FFFFFF)
	{
		DBGDrawLineDirectionMS(start.InvMultiply4(m_Transform), direction.InvMultiply3(m_Transform), color);
	}

	void DBGAddShape(Shape shape)
	{
	}

	vector DBGFixDebugPosition(inout vector position)
	{
		return position;
	}
};

class ExpansionPhysicsStateT<Class T> : ExpansionPhysicsState
{
	T m_Vehicle;

	void ExpansionPhysicsStateT(EntityAI vehicle)
	{
		Class.CastTo(m_Vehicle, vehicle);
	}

	override void DBGAddShape(Shape shape)
	{
		m_Vehicle.DBGAddShape(shape);
	}

	override vector DBGFixDebugPosition(inout vector position)
	{
		return m_Vehicle.DBGFixDebugPosition(position);
	}
};
