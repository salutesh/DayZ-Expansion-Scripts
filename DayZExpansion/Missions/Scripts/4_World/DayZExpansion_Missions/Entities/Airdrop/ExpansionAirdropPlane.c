/**
 * ExpansionAirdropPlane.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAirdropPlaneBase: House
{
	static ref map<int, ExpansionAirdropPlaneBase> s_Expansion_AirdropPlanes = new map<int, ExpansionAirdropPlaneBase>;
	static int s_Expansion_AirdropPlaneNextID;

	int m_Expansion_AirdropPlaneID;

	float m_SpawnRadius;
	float m_HeadingAngleDeg;
	float m_HeadingAngle;
	float m_Distance;

	float m_Speed;
	float m_Height;
	float m_FollowTerrainFraction;

	bool m_HeightIsRelativeToGroundLevel;

	float m_RotorAnimationPosition;

	protected bool m_WarningProximity = false;
	protected ref StringLocaliser m_WarningProximityMsg;
	protected bool m_HasWarnedProximity = false;
	protected bool m_AirdropCreated = false;
	protected ref StringLocaliser m_AirdropCreatedMsg;
	
	ref ExpansionLootContainer m_LootContainer;
	ExpansionAirdropContainerBase m_Container;
	float m_ContainerLifetime;

	protected vector m_AirdropPosition;
	protected string m_AirdropName;

	ExpansionPointLight m_Light1;
	ExpansionPointLight m_Light2;
	ExpansionPointLight m_Light3;

	protected string m_Expansion_EngineSoundSet;
	protected EffectSound m_Expansion_EngineSound;

	protected int m_TargetAltitudeBufferCount;
	protected float m_TargetAltitudeBuffer[40];
	protected float m_PrevTargetAltitude;
	protected float m_WorldSize;

	private Object m_HitObject;
	private vector m_HitPosition;
	private vector m_HitNormal;
	private float m_HitFraction;

	protected bool m_Expansion_IsUpdateEnabled;
	float m_Expansion_LastUpdateTickTime;

	void Expansion_EnableUpdate()
	{
	#ifdef SERVER
		//SetEventMask( EntityEvent.SIMULATE );
		m_Expansion_LastUpdateTickTime = GetGame().GetTickTime();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_Update, 25, true);
	#else
		//! Client or SP
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(Expansion_OnUpdate);
	#endif

		m_Expansion_IsUpdateEnabled = true;
	}

	void Expansion_DisableUpdate()
	{
		EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_DisableUpdate " + m_Expansion_IsUpdateEnabled);

		if (m_Expansion_IsUpdateEnabled)
		{
		#ifdef SERVER
			//ClearEventMask(EntityEvent.SIMULATE);
			if (GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_Update);
		#else
			if (GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM))
				GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_OnUpdate);
		#endif

			m_Expansion_IsUpdateEnabled = false;
		}
	}

	void ~ExpansionAirdropPlaneBase()
	{
	#ifndef DIAG_DEVELOPER
		if (!GetGame())
			return;
	#endif

		Expansion_DisableUpdate();

		if (s_Expansion_AirdropPlanes)
			s_Expansion_AirdropPlanes.Remove(m_Expansion_AirdropPlaneID);
	}
		
	// ------------------------------------------------------------
	// ExpansionAirdropPlane EEDelete
	// ------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		super.EEDelete(parent);
		
		if ( GetGame() )
		{
			if (m_Light1)
			{
				m_Light1.Destroy();
			}
	
			if (m_Light2)
			{
				m_Light2.Destroy();
			}
	
			if (m_Light3)
			{
				m_Light3.Destroy();
			}
			
			if ( m_Expansion_EngineSound )
			{
				StopSoundSet( m_Expansion_EngineSound );
			}
		}
	}

	static vector GetSpawnPoint( vector dropPosition, float height )
	{
		float size = GetDayZGame().GetWorldSize();
		vector worldCenter = GetDayZGame().GetWorldCenterPosition();

		EXPrint("ExpansionAirdropPlane::GetSpawnPoint - drop position " + dropPosition + " - map is " + GetGame().GetWorldName() + " - size " + size + " - center " + worldCenter);

		vector spawnPoint = Vector( 0, height, 0 );

		TIntArray directions = {0, 1, 2, 3};

		float minX = 0;
		float maxX = size;
		float minY = 0;
		float maxY = size;

		//! Make plane spawn opposite from drop location relative to map center

		if (dropPosition[0] < worldCenter[0] * 0.75)
		{
			//! Don't spawn from west
			directions.RemoveItem(0);
			minX = size - worldCenter[0] * 0.5;
		}
		else if (dropPosition[0] > worldCenter[0] * 1.25)
		{
			//! Don't spawn from east
			directions.RemoveItem(2);
			maxX = worldCenter[0] * 0.5;
		}

		if (dropPosition[2] < worldCenter[2] * 0.75)
		{
			//! Don't spawn from south
			directions.RemoveItem(1);
			minY = size - worldCenter[2] * 0.5;
		}
		else if (dropPosition[2] > worldCenter[2] * 1.25)
		{
			//! Don't spawn from north
			directions.RemoveItem(3);
			maxY = worldCenter[2] * 0.5;
		}

		int selectedDir = directions.GetRandomElement();

		switch( selectedDir )
		{
			case 0: {  // West
				spawnPoint[0] = 0.0;
				spawnPoint[2] = Math.RandomFloat( minY, maxY );
				break;
			}
			case 1: {  // South
				spawnPoint[0] = Math.RandomFloat( minX, maxX );
				spawnPoint[2] = 0.0;
				break;
			}
			case 2: {  // East
				spawnPoint[0] = size;
				spawnPoint[2] = Math.RandomFloat( minY, maxY );
				break;
			}
			case 3: {  // North
				spawnPoint[0] = Math.RandomFloat( minX, maxX );
				spawnPoint[2] = size;
				break;
			}
		}

		if ( GetExpansionSettings().GetAirdrop().HeightIsRelativeToGroundLevel )
		{
			float surfaceY = Math.Max( GetGame().SurfaceY( spawnPoint[0], spawnPoint[2] ), GetGame().SurfaceGetSeaLevel() );
			spawnPoint[1] = surfaceY + height;
		}

		EXPrint("ExpansionAirdropPlane::GetSpawnPoint - selected spawn point " + spawnPoint);

		return spawnPoint;
	}
	
	void Expansion_PlayEngineSoundLoop()
	{
		if (m_Expansion_EngineSoundSet)
			PlaySoundSetLoop(m_Expansion_EngineSound, m_Expansion_EngineSoundSet, 3.0, 3.0);
	}
	
	void Expansion_SetAirdropPlaneID(int planeID)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MISSIONS, this, "Airdrop plane ID " + planeID);
	#endif

		m_Expansion_AirdropPlaneID = planeID;
		s_Expansion_AirdropPlanes[planeID] = this;
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane SetupPlane
	// ------------------------------------------------------------
	void SetupPlane(vector dropPosition, string name, float maxRadius, bool heightIsRelativeToGround, float height, float followTerrainFrac, float speed, ExpansionLootContainer container, StringLocaliser warningProximityMsg = NULL, StringLocaliser airdropCreatedMsg = NULL, float containerLifeTime = 0.0)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		vector position = GetPosition();

		m_HeightIsRelativeToGroundLevel = heightIsRelativeToGround;
		m_Height = height;
		m_FollowTerrainFraction = followTerrainFrac;
		m_Speed = speed;
		
		// Message( PlayerBase.Cast( GetGame().GetPlayer() ), "m_Speed " + m_Speed );
		// Message( PlayerBase.Cast( GetGame().GetPlayer() ), "speed " + speed );

		m_LootContainer = container;

		m_WarningProximityMsg = warningProximityMsg;
		m_AirdropCreatedMsg = airdropCreatedMsg;

		m_ContainerLifetime = containerLifeTime;

		m_SpawnRadius = maxRadius;

		m_AirdropName = name;

		m_AirdropPosition = dropPosition;

		vector dir;

		if (IsBuilding())
			dir = position - m_AirdropPosition;
		else
			dir = m_AirdropPosition - position;

		SetDirection( dir );

		m_HeadingAngleDeg = dir.VectorToAngles()[0];

		m_HeadingAngle = Math.Atan2( m_AirdropPosition[2] - position[2], m_AirdropPosition[0] - position[0] );
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane EOnSimulate
	// ------------------------------------------------------------
#ifdef SERVER
/*
	override void EOnSimulate( IEntity other, float dt )
	{
		Expansion_OnUpdate(dt);
	}
*/
	void Expansion_Update()
	{
		float time = GetGame().GetTickTime();
		float dt = time - m_Expansion_LastUpdateTickTime;

		Expansion_OnUpdate(dt);

		m_Expansion_LastUpdateTickTime = time;
	}
#endif

	void Expansion_OnUpdate(float dt)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		//if ( IsMissionHost() )
		{
			vector angles = GetAngles();
			vector position = GetPosition();
			vector velocity = Vector( Math.Cos( m_HeadingAngle ), 0, Math.Sin( m_HeadingAngle ) );

			velocity[0] = velocity[0] * m_Speed;
			velocity[2] = velocity[2] * m_Speed;

			float altitude = position[1];
			float targetAltitude;
			float terrainY;

			//! @note following terrain implies relative to ground level
			if (m_FollowTerrainFraction > 0)
			{
				//! Plane will be at this pos in 1s - react to terrain elevation changes in advance
				terrainY = GetTerrainY( position + velocity );

				targetAltitude = Math.Lerp( altitude, terrainY + m_Height, m_FollowTerrainFraction );

				//! Altitude buffer for smoothing

				float targetAltitudeSum;

				for ( int i = 0; i < 40; i++ )
				{
					float bufferVal;

					if ( i < 39 )
						bufferVal = m_TargetAltitudeBuffer[i + 1];
					else
						bufferVal = targetAltitude;

					m_TargetAltitudeBuffer[i] = bufferVal;

					targetAltitudeSum += bufferVal;
				}

				if ( m_TargetAltitudeBufferCount < 40 )
					m_TargetAltitudeBufferCount++;

				targetAltitude = targetAltitudeSum / m_TargetAltitudeBufferCount;

				if ( m_TargetAltitudeBufferCount > 1 )
				{
					//! The current pitch angle of the plane determines how much its altitude can change up/down
					//! Smooths out motion and reduces wobbling on uneven terrain
					//! Also prevents excessive altitude changes from frame to frame,
					//! e.g. if terrain has, ahem, "interesting" geometry (Namalsk *cough* tunnel *cough*)

					float signScale;

					if ( targetAltitude < altitude )
					{
						//! Descend at half the rate of ascend (if we have to ascend again directly afterwards, this makes the transition smoother)
						signScale = 0.5;
					} else
					{
						signScale = -1.0;
					}

					float f = Math.Min( Math.Max( angles[0] / 45.0 * signScale, 0 ), 1.0 );

					float allowedAltitudeDelta;
					float allowedAltitudeDeltaNominal = m_Speed * dt;  //! Nominal at abs pitch angle of 45 deg
					float allowedAltitudeDeltaMin = allowedAltitudeDeltaNominal * 0.10;  //! 10% of nominal, still allows decent reaction time

					allowedAltitudeDelta = Math.Lerp( allowedAltitudeDeltaMin, allowedAltitudeDeltaNominal, f );

					if ( targetAltitude < m_PrevTargetAltitude - allowedAltitudeDelta )
					{
						targetAltitude = m_PrevTargetAltitude - allowedAltitudeDelta;
					} else if ( targetAltitude > m_PrevTargetAltitude + allowedAltitudeDelta )
					{
						targetAltitude = m_PrevTargetAltitude + allowedAltitudeDelta;
					}

					//EXPrint("terrainY " + terrainY + " m_HitPosition[1] " + m_HitPosition[1] + " pitch " + angles[0] + " altitude " + altitude + " allowedAltitudeDelta " + allowedAltitudeDelta + " targetAltitude " + targetAltitude);
				}

				m_PrevTargetAltitude = targetAltitude;
			}
			else if (m_HeightIsRelativeToGroundLevel)
			{
				//! Set altitude to relative above the drop position

				terrainY = GetTerrainY(m_AirdropPosition);

				targetAltitude = terrainY + m_Height;
				altitude = targetAltitude;
			}
			else
			{
				targetAltitude = m_Height;
			}

			float delta = targetAltitude - altitude;

			if ( m_Speed > 0 )
			{
				//! Adjust pitch
				if (IsBuilding())
					angles[0] = -45.0 * ( delta / m_Speed );
				else
					angles[0] = 45.0 * ( delta / m_Speed );
				SetAngles( angles );
			}

			velocity[0] = velocity[0] * dt;
			velocity[2] = velocity[2] * dt;

			vector transform[4];
			GetTransform( transform );
			transform[3] = transform[3] + velocity;
			transform[3][1] = altitude + delta * dt;

			MoveInTime( transform, dt );

			if ( CheckForRemove() )
			{
				RemovePlane();
			}
		}

		if (GetGame().IsServer())
		{
			if ( CheckForDrop() && !m_Container )
			{	
				m_Container = CreateDrop( m_LootContainer.Container );
				
				if ( !m_Container )
				{
					Error( "ExpansionMissionEventAirdrop::Event_OnUpdate - ERROR: Could not create container object!" );
				} else
				{
					if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowAirdropDropped && m_AirdropCreatedMsg )
						ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop"), m_AirdropCreatedMsg, EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();
				}
			}

			if ( IsWarningProximity() )
			{
				if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowAirdropClosingOn && m_WarningProximityMsg )
					ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop"), m_WarningProximityMsg, EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();
			}
		}
	}

	float GetTerrainY( vector position )
	{
		//! Make sure we clamp surfaceY to sea level (when plane is over water)
		float terrainY = Math.Max( GetGame().SurfaceRoadY( position[0], position[2] ), GetGame().SurfaceGetSeaLevel() );

	/*
		float radius = 21.0;  //! Note: C-130J has a wingspan of 40.4m, radius is chosen accordingly

		vector start = Vector( position[0], position[1] - radius - 12.0, position[2] );
		vector end = Vector( position[0], terrainY, position[2] );

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.TERRAIN | PhxInteractionLayers.WATERLAYER;

		//! Note: If this wasn't a static method and we pass 'this' (our plane) as ignoreObj like we normally would, it crashes the host. Why DayZ, WHY?
		if ( DayZPhysics.SphereCastBullet( start, end, radius, collisionLayerMask, NULL, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction ) )
		{
			if ( m_HitPosition[1] > terrainY && Math.AbsFloat( position[1] - m_HitPosition[1] ) > radius )
				terrainY = m_HitPosition[1];
		}
	*/

		return terrainY;
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane CheckForRemove
	// ------------------------------------------------------------
	bool CheckForRemove()
	{
		vector position = GetPosition();
		if ( m_WorldSize == 0 )
			m_WorldSize = GetDayZGame().GetWorldSize();
		if ( !IsInRect( position[0], position[2], -10.0, m_WorldSize + 10, -10.0, m_WorldSize + 10 ) )
		{
			return true;
		}

		return false;
	}

	void RemovePlane()
	{
		Expansion_DisableUpdate();
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, 0, false, this );
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane IsWarningProximity
	// ------------------------------------------------------------
	bool IsWarningProximity()
	{
		return m_WarningProximity;
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane CheckForDrop
	// ------------------------------------------------------------
	bool CheckForDrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		if ( !IsMissionHost() )
			return false;

		if ( GetPosition()[0] == 0 || GetPosition()[2] == 0 )
			return false;

		if ( m_AirdropPosition[0] == 0 || m_AirdropPosition[2] == 0 )
			return false;

		vector position = GetPosition();

		bool drop = false;

		float distance = vector.Distance( position, Vector( m_AirdropPosition[0], position[1], m_AirdropPosition[2] ) );

		if ( !m_Distance || distance < m_Distance )
			m_Distance = distance;

		m_WarningProximity = false;

		if ( distance <= 1500 && !m_HasWarnedProximity )
		{
			m_HasWarnedProximity = true;
			m_WarningProximity = true;	
		} else if ( !m_AirdropCreated )
		{
			if ( distance == 0 || distance > m_Distance )
			{
				return true;
			}
		}

		return false;
	}

	//! Server
	static ExpansionAirdropPlaneBase CreatePlane(vector dropPosition, string name, float maxRadius, float height, float speed, ExpansionLootContainer container, StringLocaliser warningProximityMsg = NULL, StringLocaliser airdropCreatedMsg = NULL, float containerLifeTime = 0.0)
	{
		return CreatePlane("", dropPosition, name, maxRadius, height, speed, container, warningProximityMsg, airdropCreatedMsg, containerLifeTime);
	}

	//! Server
	static ExpansionAirdropPlaneBase CreatePlane(string planeClassName, vector dropPosition, string name, float maxRadius, float height, float speed, ExpansionLootContainer container, StringLocaliser warningProximityMsg = NULL, StringLocaliser airdropCreatedMsg = NULL, float containerLifeTime = 0.0)
	{
		vector spawnPoint = GetSpawnPoint( dropPosition, height );

		auto settings = GetExpansionSettings().GetAirdrop();

		if (!planeClassName)
			planeClassName = settings.AirdropPlaneClassName;
			
		if (!planeClassName)
		{
			if (ExpansionStatic.TypeExists("ExpansionAirdropPlane"))
				planeClassName = "ExpansionAirdropPlane";
			else
				planeClassName = "ExpansionAirdropPlaneBasic";
		}
		else if (!ExpansionStatic.Is(planeClassName, ExpansionAirdropPlaneBase))
		{
			if (planeClassName == "ExpansionAirdropPlane")
				EXError.Warn(null, "[Airdrop] The animated plane could not be created, falling back to basic version. Make sure @DayZ-Expansion-Licensed is loaded if you want the animated version.", {});
			else
				EXError.Warn(null, "[Airdrop] '" + planeClassName + "' is not a valid airdrop plane classname (needs to inherit from ExpansionAirdropPlaneBase), falling back to basic version.", {});

			planeClassName = "ExpansionAirdropPlaneBasic";
		}

		dropPosition = ExpansionMath.GetRandomPointInCircle( Vector( dropPosition[0], spawnPoint[1], dropPosition[2] ), maxRadius );

		auto plane = ExpansionAirdropPlaneBase.Cast( GetGame().CreateObjectEx(planeClassName, spawnPoint, ECE_AIRBORNE | ECE_LOCAL) );

		plane.Expansion_SetAirdropPlaneID(s_Expansion_AirdropPlaneNextID++);
		plane.SetupPlane( dropPosition, name, maxRadius, settings.HeightIsRelativeToGroundLevel, height, settings.FollowTerrainFraction, speed, container, warningProximityMsg, airdropCreatedMsg, containerLifeTime );

		plane.Expansion_SendCreatePlaneOnClient();

		plane.Expansion_EnableUpdate();

		return plane;
	}

	void Expansion_SendCreatePlaneOnClient(PlayerIdentity identity = null)
	{
		auto rpc = ExpansionMissionModule.s_Instance.Expansion_CreateRPC("RPC_CreateAirdropPlane");
		rpc.Write(m_Expansion_AirdropPlaneID);
		rpc.Write(GetType());
		rpc.Write(GetPosition());
		rpc.Write(m_AirdropPosition);
		rpc.Write(m_HeightIsRelativeToGroundLevel);
		rpc.Write(m_Height);
		rpc.Write(m_FollowTerrainFraction);
		rpc.Write(m_Speed);
		rpc.Expansion_Send(true, identity);
	}

	static void Expansion_SendCreatePlanesOnClient(PlayerBase player)
	{
		if (!player)
			return;

		auto identity = player.GetIdentity();

		if (!identity)
			return;

		foreach (int id, ExpansionAirdropPlaneBase plane: s_Expansion_AirdropPlanes)
		{
			plane.Expansion_SendCreatePlaneOnClient(identity);
		}
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane CreateDrop
	// ------------------------------------------------------------
	ExpansionAirdropContainerBase CreateDrop( string container )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		m_AirdropCreated = true;

		float negativeOffsetY = Expansion_GetDropOffset();

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MISSIONS, this, "Airdrop container negative offset Y " + negativeOffsetY);
	#endif

		vector dropPosition = Vector( m_AirdropPosition[0], GetPosition()[1] - negativeOffsetY, m_AirdropPosition[2] );
	
		// ExpansionNotification( new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_EVENT_DROP", m_AirdropName ), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_EXPANSION).Create();
		int flags = ECE_CREATEPHYSICS | ECE_AIRBORNE | ECE_NOLIFETIME;
		#ifndef DAYZ_1_20
		flags |= ECE_DYNAMIC_PERSISTENCY;
		#endif
		Object obj = GetGame().CreateObjectEx(container, dropPosition, flags);

		#ifdef DIAG_DEVELOPER
		if (DayZPlayerImplement.s_Expansion_DebugObjects_Enabled)
			GetGame().CreateObjectEx("ExpansionDebugRodBig", ExpansionStatic.GetSurfacePosition(dropPosition), ECE_NOLIFETIME);
		#endif
		
		ExpansionAirdropContainerBase drop;
		if ( Class.CastTo( drop, obj ) )
		{
			drop.SetPosition( dropPosition );
			if (GetGame().GetWeather())
			{
				//! Rotate to wind
				vector wind = GetGame().GetWeather().GetWind();
				vector ori = wind.Normalized().VectorToAngles();
				ori[0] = Math.NormalizeAngle(ori[0] + 90.0);
				ori[1] = 0.0;
				ori[2] = 0.0;
				drop.SetOrientation(ori);
			}

			//! @note container max lifetime is set to mission max time.
			//! If mission max time reaches zero before container lifetime (e.g. because container was interacted with by players),
			//! container health will be set to zero in ExpansionMissionEventAirdrop::Event_OnUpdate.
			//! If no one interacts with the container and its lifetime is zero, the game's CE will clean it up once players move away
			//! even if players are still in the larger 1100 m radius checked by ExpansionMissionEventAirdrop::CanEnd, which is intended.
			//! This handling guarantees that a container which is not interacted with at all by players in the radius
			//! doesn't stay around forever when the mission ends unless players are really close to it.
			if (m_ContainerLifetime > 0)
				drop.SetLifetimeMax(m_ContainerLifetime);

			drop.InitAirdrop(m_LootContainer.Loot, m_LootContainer.Infected, m_LootContainer.ItemCount, m_LootContainer.InfectedCount, m_LootContainer.FallSpeed, Math.Min(m_SpawnRadius * 0.01, 1.0));
		}

		return drop;
	}

	float Expansion_GetDropOffset()
	{
		vector minMax[2];
		if (GetCollisionBox(minMax))
			return Math.AbsFloat(minMax[0][1]) + 5;

		return 10;
	}

	bool AirdropCreated()
	{
		return m_AirdropCreated;
	}

	ExpansionAirdropContainerBase GetContainer()
	{
		return m_Container;
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane IsInRect
	// ------------------------------------------------------------
	protected bool IsInRect(float x, float y, float min_x, float max_x, float min_y, float max_y) 
	{
		if ( x > min_x && x < max_x && y > min_y && y < max_y ) return true;
		return false;
	}
};

class ExpansionAirdropPlane: ExpansionAirdropPlaneBase
{
	void ExpansionAirdropPlane()
	{
		m_Expansion_EngineSoundSet = "Expansion_C130J_Loop_SoundSet";
	}

#ifndef SERVER
	override void Expansion_OnUpdate(float dt)
	{
		super.Expansion_OnUpdate(dt);

		//! Animating rotor only on client gives much smoother rotor animation

		m_RotorAnimationPosition += 1 * dt * 20.0 / ( Math.PI * 2.0 );
		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		SetAnimationPhase( "rotor", m_RotorAnimationPosition );
	}
#endif
};
