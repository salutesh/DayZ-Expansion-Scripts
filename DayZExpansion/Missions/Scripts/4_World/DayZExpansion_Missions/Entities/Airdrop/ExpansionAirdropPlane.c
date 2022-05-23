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

class ExpansionAirdropPlane extends House
{
	float m_SpawnRadius;
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
	
	ref ExpansionAirdropLootContainer m_LootContainer;
	ExpansionAirdropContainerBase m_Container;

	protected vector m_AirdropPosition;
	protected string m_AirdropName;

	ExpansionPointLight m_Light1;
	ExpansionPointLight m_Light2;
	ExpansionPointLight m_Light3;

	protected EffectSound m_SoundLoop;

	protected int m_TargetAltitudeBufferCount;
	protected float m_TargetAltitudeBuffer[40];
	protected float m_PrevTargetAltitude;
	protected float m_WorldSize;

	private Object m_HitObject;
	private vector m_HitPosition;
	private vector m_HitNormal;
	private float m_HitFraction;

	// ------------------------------------------------------------
	// ExpansionAirdropPlane Constructor
	// ------------------------------------------------------------
	void ExpansionAirdropPlane()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] Constructor start");
		#endif

		if ( IsMissionClient() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( PlayLoop );
		}

		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] Constructor end");
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionAirdropPlane EEDelete
	// ------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropPlane::EEDelete - Start");
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
			
			if ( m_SoundLoop )
			{
				StopSoundSet( m_SoundLoop );
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropPlane::EEDelete - End");
		#endif
	}

	static vector GetSpawnPoint( float height )
	{
		float size = GetDayZGame().GetWorldSize();

		EXPrint("ExpansionAirdropPlane::GetSpawnPoint - map is " + GetGame().GetWorldName() + " - size " + size);

		vector spawnPoint = Vector( 0, height, 0 );

		switch( Math.RandomInt(0, 4) )
		{
			case 0: {
				spawnPoint[0] = 0.0;
				spawnPoint[2] = Math.RandomFloat( 0.0, size );
				break;
			}
			case 1: {
				spawnPoint[0] = size;
				spawnPoint[2] = Math.RandomFloat( 0.0, size );
				break;
			}
			case 2: {
				spawnPoint[0] = Math.RandomFloat( 0.0, size );
				spawnPoint[2] = 0.0;
				break;
			}
			case 3: {
				spawnPoint[0] = Math.RandomFloat( 0.0, size );
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
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane PlayLoop
	// ------------------------------------------------------------
	void PlayLoop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] PlayLoop start");
		#endif
		
		PlaySoundSetLoop( m_SoundLoop, "Expansion_C130J_Loop_SoundSet", 3.0, 3.0 );
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] PlayLoop end");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane SetupPlane
	// ------------------------------------------------------------
	void SetupPlane( vector dropPosition, string name, float maxRadius, float height, float speed, ExpansionAirdropLootContainer container, StringLocaliser warningProximityMsg = NULL, StringLocaliser airdropCreatedMsg = NULL )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] SetupPlane start");
		#endif

		vector position = GetPosition();

		m_HeightIsRelativeToGroundLevel = GetExpansionSettings().GetAirdrop().HeightIsRelativeToGroundLevel;
		m_Height = height;
		m_FollowTerrainFraction = GetExpansionSettings().GetAirdrop().FollowTerrainFraction;
		m_Speed = speed;
		
		// Message( PlayerBase.Cast( GetGame().GetPlayer() ), "m_Speed " + m_Speed );
		// Message( PlayerBase.Cast( GetGame().GetPlayer() ), "speed " + speed );

		m_LootContainer = container;

		m_WarningProximityMsg = warningProximityMsg;
		m_AirdropCreatedMsg = airdropCreatedMsg;

		m_SpawnRadius = maxRadius;

		m_AirdropName = name;

		m_AirdropPosition = ExpansionMath.GetRandomPointInCircle( Vector( dropPosition[0], position[1], dropPosition[2] ), m_SpawnRadius );

		SetDirection( position - m_AirdropPosition );

		m_HeadingAngle = Math.Atan2( m_AirdropPosition[2] - position[2], m_AirdropPosition[0] - position[0] );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] SetupPlane end");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane EOnSimulate
	// ------------------------------------------------------------
	override void EOnSimulate( IEntity owner, float dt )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] EOnSimulate start");
		#endif

		if ( IsMissionHost() )
		{
			vector angles = GetAngles();
			vector position = GetPosition();
			vector velocity = Vector( Math.Cos( m_HeadingAngle ), 0, Math.Sin( m_HeadingAngle ) );

			velocity[0] = velocity[0] * m_Speed;
			velocity[2] = velocity[2] * m_Speed;

			float altitude = position[1];
			float targetAltitude;

			if ( m_HeightIsRelativeToGroundLevel && m_FollowTerrainFraction > 0 )
			{
				//! Plane will be at this pos in 1s - react to terrain elevation changes in advance
				float terrainY = GetTerrainY( position + velocity );

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
			} else
			{
				targetAltitude = m_Height;
			}

			float delta = targetAltitude - altitude;

			if ( m_Speed > 0 )
			{
				//! Adjust pitch
				angles[0] = -45.0 * ( delta / m_Speed );
				SetAngles( angles );
			}

			velocity[0] = velocity[0] * dt;
			velocity[2] = velocity[2] * dt;

			vector transform[4];
			GetTransform( transform );
			transform[3] = transform[3] + velocity;
			transform[3][1] = altitude + delta * dt;

			MoveInTime( transform, dt );

			if ( CheckForDrop() && !m_Container )
			{	
				m_Container = CreateDrop( m_LootContainer.Container );
				
				if ( !m_Container )
				{
					Error( "ExpansionMissionEventAirdrop::Event_OnUpdate - ERROR: Could not create container object!" );
				} else
				{
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( m_Container.InitAirdrop, 0, false, m_LootContainer.Loot, m_LootContainer.Infected, m_LootContainer.ItemCount, m_LootContainer.InfectedCount );
				
					if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowAirdropDropped && m_AirdropCreatedMsg )
						ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop"), m_AirdropCreatedMsg, EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();
				}
			}

			if ( IsWarningProximity() )
			{
				if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().ShowAirdropClosingOn && m_WarningProximityMsg )
					ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop"), m_WarningProximityMsg, EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();
			}

			if ( CheckForRemove() )
			{
				RemovePlane();
			}
		}

		m_RotorAnimationPosition += 1 * dt * 20.0 / ( Math.PI * 2.0 );
		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		SetAnimationPhase( "rotor", m_RotorAnimationPosition );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] EOnSimulate end");
		#endif
	}

	float GetTerrainY( vector position )
	{
		//! Make sure we clamp surfaceY to sea level (when plane is over water)
		float terrainY = Math.Max( GetGame().SurfaceY( position[0], position[2] ), GetGame().SurfaceGetSeaLevel() );

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
		EXLogPrint("[ExpansionAirdropPlane] CheckPosition start");
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

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] CheckPosition end");
		#endif

		return false;
	}

	static ExpansionAirdropPlane CreatePlane( vector dropPosition, string name, float maxRadius, float height, float speed, ExpansionAirdropLootContainer container, StringLocaliser warningProximityMsg = NULL, StringLocaliser airdropCreatedMsg = NULL )
	{
		vector spawnPoint = GetSpawnPoint( height );

		ExpansionAirdropPlane plane = ExpansionAirdropPlane.Cast( GetGame().CreateObjectEx("ExpansionAirdropPlane", spawnPoint, ECE_AIRBORNE|ECE_CREATEPHYSICS) );

		plane.SetupPlane( dropPosition, name, maxRadius, height, speed, container, warningProximityMsg, airdropCreatedMsg );

		return plane;
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane CreateDrop
	// ------------------------------------------------------------
	ExpansionAirdropContainerBase CreateDrop( string container )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] CreateDrop start");
		#endif

		m_AirdropCreated = true;

		vector dropPosition = Vector( m_AirdropPosition[0], GetPosition()[1] - 10.0, m_AirdropPosition[2] );
	
		// ExpansionNotification( new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_EVENT_DROP", m_AirdropName ), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_EXPANSION).Create();
		Object obj = GetGame().CreateObjectEx( container, dropPosition, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_AIRBORNE );
		
		ExpansionAirdropContainerBase drop;
		if ( Class.CastTo( drop, obj ) )
		{			
			if ( m_SpawnRadius > 0 )
				drop.SetWindImpact(true);

			drop.SetPosition( dropPosition );
			drop.SetOrientation( "0 0 0" );
		}

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] CreateDrop end");
		#endif

		return drop;
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
