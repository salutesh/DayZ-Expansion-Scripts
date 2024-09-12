//! TODO: Reduce the number of varibales
class ExpansionPhysicsState
{
	static const float PING_INTERVAL = 1.0;

	EntityAI m_Entity;

	float m_DeltaTime;

	vector m_Force;
	vector m_Torque;

	vector m_LinearVelocity;   // World Space
	vector m_LinearVelocityMS; // Model Space

	vector m_LinearAcceleration;   // World Space
	vector m_LinearAccelerationMS; // Model Space

	vector m_LastLinearVelocity; // World Space
	vector m_LastAngularVelocity; // World Space

	vector m_AngularVelocity;	// World Space
	vector m_AngularVelocityMS; // Model Space

	vector m_AngularAcceleration;	// World Space
	vector m_AngularAccelerationMS; // Model Space

	vector m_SyncLinearVelocity;	  // World Space
	vector m_SyncLinearAcceleration;  // World Space
	vector m_SyncAngularVelocity;	  // World Space
	vector m_SyncAngularAcceleration; // World Space

	vector m_Transform[4];
	vector m_TargetTransform[4];

	vector m_TensorWorld[4];

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

	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;
	vector m_SyncPosition;

	float m_OrientationX;
	float m_OrientationY;
	float m_OrientationZ;
	vector m_SyncOrientation;

	bool m_IsSync;
	float m_TimeSinceDesync;
	bool m_HaltPhysics;
	float m_TimeSincePing;
	bool m_ClientDesync;
	bool m_NotifyDesyncInvulnerability;
	float m_DesyncInvulnerabilityTimeoutSeconds;
	bool m_IsInvulnerable;
	bool m_IsInvulnerableSync;
	float m_InvulnerabilityTime;

	void ExpansionPhysicsState(EntityAI vehicle)
	{
		m_Entity = vehicle;
	}

	void RegisterSync_CarScript(string varName)
	{
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearVelocityX", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearVelocityY", 0, 0, 4);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_LinearVelocityZ", 0, 0, 4);

		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_PositionX", 0, 0, 2);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_PositionY", 0, 0, 2);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_PositionZ", 0, 0, 2);

		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_OrientationX", 0, 0, 2);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_OrientationY", 0, 0, 2);
		m_Entity.RegisterNetSyncVariableFloat(varName + ".m_OrientationZ", 0, 0, 2);

		m_Entity.RegisterNetSyncVariableBool(varName + ".m_IsInvulnerable");
	}

	void Init()
	{
		m_BoundingRadius = m_Entity.ClippingInfo(m_BoundingBox);
		m_MaxSpeedMS = m_MaxSpeed * (1.0 / 3.6);

		vector pos = m_Entity.GetPosition();
		m_PositionX = pos[0];
		m_PositionY = pos[1];
		m_PositionZ = pos[2];

		vector ori = m_Entity.GetOrientation();
		m_OrientationX = ori[0];
		m_OrientationY = ori[1];
		m_OrientationZ = ori[2];

		//! Client/server (not netsynched), but used for different purpose
		m_IsSync = true;
		m_SyncPosition = pos;

		//! Server only
		if (GetGame().IsServer())
			m_DesyncInvulnerabilityTimeoutSeconds = GetExpansionSettings().GetVehicle().DesyncInvulnerabilityTimeoutSeconds;
	}

	void CreateDynamic()
	{
		int physLayer = PhxInteractionLayers.VEHICLE;

		m_Entity.CreateDynamicPhysics(physLayer);

		vector bbox[2];
		float radius = m_Entity.ClippingInfo(bbox);

		m_Entity.SetDynamicPhysicsLifeTime(-1);

		dBodyActive(m_Entity, ActiveState.ACTIVE);
		dBodyEnableGravity(m_Entity, false);

		//m_Entity.EnableDynamicCCD( true );
		dBodyEnableCCD(m_Entity, radius, radius * 0.45);

		dBodySetSleepingTreshold(m_Entity, 0.0, 0.0);
		dBodySetDamping(m_Entity, 0, 0);

		SetupSimulation(m_DeltaTime);

		SetVelocity(m_Entity, m_LinearVelocity);
		dBodySetAngularVelocity(m_Entity, m_AngularVelocity);
	}

	void OnVariablesSynchronized_CarScript()
	{
		m_TimeSince = (m_Time - m_Entity.GetSimulationTimeStamp()) / 1000.0;
		if (m_TimeSince < 0)
			m_TimeSince = 0;

		m_SyncPosition = Vector(m_PositionX, m_PositionY, m_PositionZ);
		m_SyncOrientation = Vector(m_OrientationX, m_OrientationY, m_OrientationZ);

		//m_LastLinearVelocity = m_LinearVelocity;
		m_SyncLinearVelocity = Vector(m_LinearVelocityX, m_LinearVelocityY, m_LinearVelocityZ);
	}

	void SetupSimulation(float pDt)
	{
		m_Entity.GetTransform(m_Transform);

		m_Mass = dBodyGetMass(m_Entity);
		m_InvMass = 1.0 / m_Mass;
		m_Center = dBodyGetCenterOfMass(m_Entity);

		m_DeltaTime = pDt;

		m_Force = vector.Zero;
		m_Torque = vector.Zero;

		dBodyGetInvInertiaTensorWorld(m_Entity, m_TensorWorld);

		m_LastLinearVelocity = m_LinearVelocity;
		m_LastAngularVelocity = m_AngularVelocity;

		m_LinearVelocity = GetVelocity(m_Entity);
		m_AngularVelocity = dBodyGetAngularVelocity(m_Entity);

		m_LinearAcceleration = (m_LinearVelocity - m_LastLinearVelocity);
		m_AngularAcceleration = (m_AngularVelocity - m_LastAngularVelocity);

		m_LinearVelocityMS = m_LinearVelocity.InvMultiply3(m_Transform);
		m_AngularVelocityMS = m_AngularVelocity.InvMultiply3(m_Transform);
		
		m_LinearAccelerationMS = m_LinearAcceleration.InvMultiply3(m_Transform);
		m_AngularAccelerationMS = m_AngularAcceleration.InvMultiply3(m_Transform);
	}

	void ApplySimulation(float pDt)
	{
		if (!dBodyIsActive(m_Entity) || !dBodyIsDynamic(m_Entity))
		{
			return;
		}
		
		dBodyApplyForce(m_Entity, m_Force);
		dBodyApplyTorque(m_Entity, m_Torque);
	}

	void ApplySimulation_CarScript(float pDt, bool isPhysHost, DayZPlayerImplement driver = NULL)
	{
		EXTrace trace;

		bool isServer = GetGame().IsServer();

		if (!m_IsSync)
			m_TimeSinceDesync += pDt;

		if (m_IsInvulnerable)
			m_InvulnerabilityTime += pDt;

		if (!isServer && isPhysHost)
		{
			//! Client - player is driver

			if (m_IsInvulnerable != m_IsInvulnerableSync)
			{
				m_IsInvulnerableSync = m_IsInvulnerable;
				if (GetExpansionClientSettings().ShowDesyncInvulnerabilityNotifications)
				{
					//! @note We only want to show the notifications if the player is controlling this vehicle right now, that's why this is here and not in
					//! OnVariablesSynchronized_CarScript (avoids an additional check for driver)
					if (m_IsInvulnerable)
						ExpansionNotification("Client Desync", "Temporary vehicle invulnerability ENABLED (this message may be delayed)", GetNotificationIcon(), COLOR_EXPANSION_NOTIFICATION_ORANGE).Create();
					else
						ExpansionNotification("Client Desync", "Timeout reached - temporary vehicle invulnerability DISABLED - was active for " + GetExpansionSettings().GetVehicle().DesyncInvulnerabilityTimeoutSeconds + " seconds", GetNotificationIcon(), COLOR_EXPANSION_NOTIFICATION_ORANGE).Create();
				}
			}

			bool prevSync = m_IsSync;

			//! If distance between position on client and last synced position from server is greater
			//! than what the vehicle could realistically have traveled in one second (+/- 1m tolerance), assume desync
			float velocity = m_SyncLinearVelocity.Length();
			float distanceThresholdSq = velocity * velocity;
			m_IsSync = vector.DistanceSq(m_SyncPosition, m_Entity.GetPosition()) <= distanceThresholdSq + 1;

			float vehicleResyncTimeout = GetExpansionClientSettings().VehicleResyncTimeout;
			if (vehicleResyncTimeout < 1.0)
				vehicleResyncTimeout = 1.0;

			if (prevSync && !m_IsSync)
			{
				m_TimeSinceDesync = 0;
				m_InvulnerabilityTime = 0;

				//! Let the server know we desynced
				SendPing(true);
			}
			else if (!prevSync && m_IsSync)
			{
				m_HaltPhysics = false;

				//! Let the server know we resynced
				SendPing(false);
			}
			else if (!prevSync && !m_IsSync && m_TimeSinceDesync > vehicleResyncTimeout && !m_HaltPhysics)
			{
#ifdef EXTRACE
				if (EXTrace.VEHICLES && driver != NULL)
					trace = EXTrace.Start(true, m_Entity, "Client desynced for " + m_TimeSinceDesync + " - halting physics updates");
#endif
				//! If <VehicleResyncTimeout> seconds after we desynced we are still desynced, halt vehicle physics updates on client
				m_HaltPhysics = true;
			}
		}

		if (isServer || !isPhysHost || !m_HaltPhysics)
		{
			ApplySimulation(pDt);
		}
		else
		{
			//! Client - player is driver - desynced - halt physics

			SetVelocity(m_Entity, "0 0 0");
			dBodySetAngularVelocity(m_Entity, "0 0 0");
			dBodyDynamic(m_Entity, false);

			if (m_SyncPosition != vector.Zero)
			{
				Math3D.YawPitchRollMatrix(m_SyncOrientation, m_TargetTransform);
				m_TargetTransform[3] = m_SyncPosition;
				m_Entity.SetTransform(m_TargetTransform);

				SetVelocity(m_Entity, "0 0 0");
				dBodySetAngularVelocity(m_Entity, "0 0 0");
				dBodyDynamic(m_Entity, true);
			}
		}

		if (isServer)
		{
			m_LinearVelocityX = m_LinearVelocity[0];
			m_LinearVelocityY = m_LinearVelocity[1];
			m_LinearVelocityZ = m_LinearVelocity[2];

			//vector pos = m_Transform[3];
			vector pos = EstimatePosition(pDt);
			m_PositionX = pos[0];
			m_PositionY = pos[1];
			m_PositionZ = pos[2];

			//vector ori = Math3D.MatrixToAngles(m_Transform);
			vector ori = EstimateOrientation(pDt);
			m_OrientationX = ori[0];
			m_OrientationY = ori[1];
			m_OrientationZ = ori[2];

			if (!m_DesyncInvulnerabilityTimeoutSeconds)
				return;

			if (m_IsInvulnerable)
			{
				if (m_InvulnerabilityTime > m_DesyncInvulnerabilityTimeoutSeconds)
				{
					//trace = EXTrace.Start(EXTrace.VEHICLES && driver != NULL, m_Entity, "Disabling temporary vehicle invulnerability");
					m_Entity.SetAllowDamage(true);
					m_IsInvulnerable = false;
				}
			}

			//if (m_TimeSincePing > PING_INTERVAL * 1.25)
			if (m_ClientDesync)
			{
				if (m_IsSync)
				{
					m_IsSync = false;
					m_TimeSinceDesync = 0;
					m_InvulnerabilityTime = 0;
					if (m_Entity.GetAllowDamage())
					{
						//trace = EXTrace.Start(EXTrace.VEHICLES && driver != NULL, m_Entity, "Client desynced - time since last ping from client " + m_TimeSincePing + " - enabling temporary vehicle invulnerability");
						m_Entity.SetAllowDamage(false);
						m_IsInvulnerable = true;
					}
					else
					{
						//trace = EXTrace.Start(EXTrace.VEHICLES && driver != NULL, m_Entity, "Client desynced - time since last ping from client " + m_TimeSincePing);
					}
				}
				else
				{
					m_TimeSincePing += pDt;
				}
			}
			else if (!m_IsSync)
			{
				//trace = EXTrace.Start(EXTrace.VEHICLES && driver != NULL, m_Entity, "Client resynced - time since desync " + m_TimeSinceDesync);
				m_IsSync = true;
			}
		}
		//else
		//{
			//if (m_TimeSincePing > PING_INTERVAL)
			//{
				//SendPing(m_ClientDesync);
				//m_TimeSincePing = 0;
			//}
		//}
	}

	string GetNotificationIcon()
	{
		if (m_Entity.IsInherited(ExpansionHelicopterScript))
			return "Helicopter";
		else if (m_Entity.IsInherited(ExpansionBoatScript))
			return "Boat";
		return "Car";
	}

	void SendPing(bool desync)
	{
		auto rpc = ExpansionScriptRPC.Create(CarScript.s_Expansion_ClientPing_RPCID);
		rpc.Write(desync);
		rpc.Expansion_Send(m_Entity, true);
	}

	void OnPing(ParamsReadContext ctx)
	{
		m_TimeSincePing = 0;
		ctx.Read(m_ClientDesync);
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.VEHICLES, m_Entity, "Received client ping - desynced " + m_ClientDesync);
#endif
	}

	void OnRPC(ParamsReadContext ctx)
	{
		int time;
		ctx.Read(time);

		// check if this is an old state and if so, remove it
		if (m_Time > time)
		{
			return;
		}
		
		m_Time = time;
		m_TimeSince = (m_Entity.GetSimulationTimeStamp() - m_Time) / 1000.0;
		if (m_TimeSince < 0)
		{
			m_TimeSince = 0;
		}

		vector pos;
		vector ori;

		ctx.Read(pos);
		ctx.Read(ori);

		Math3D.YawPitchRollMatrix(ori, m_TargetTransform);
		m_TargetTransform[3] = pos;

		ctx.Read(m_SyncLinearVelocity);
		ctx.Read(m_SyncAngularVelocity);

		ctx.Read(m_SyncLinearAcceleration);
		ctx.Read(m_SyncAngularAcceleration);
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
		instance.Add("Force", m_Force);
		instance.Add("Torque", m_Torque);
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

		DBGAddShape(Shape.CreateSphere(color, ShapeFlags.TRANSP | ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, position, size));
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
