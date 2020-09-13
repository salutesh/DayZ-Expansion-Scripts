/**
 * ExpansionAirdropPlane.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAirdropPlane extends House
{
	float m_SpawnRadius;
	float m_HeadingAngle;

	float m_Speed;
	float m_Height;

	float m_RotorAnimationPosition;

	protected bool m_WarnedProximity = false;
	protected bool m_HasWarnedProximity = false;
	protected bool m_AirdropCreated = false;

	protected vector m_AirdropPosition;
	protected string m_AirdropName;

	ExpansionPointLight m_Light1;
	ExpansionPointLight m_Light2;
	ExpansionPointLight m_Light3;

	protected EffectSound m_SoundLoop;

	protected float m_PreviousDistance;

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
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( DeferredInit, 10000, false );
		}

		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] Constructor end");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane Destructor
	// ------------------------------------------------------------
	void ~ExpansionAirdropPlane()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] Deconstructor start");
		#endif

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] Deconstructor end");
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
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane DeferredInit
	// ------------------------------------------------------------
	void DeferredInit()
	{
		if ( IsMissionClient() )
		{
			PlayLoop();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane PlayLoop
	// ------------------------------------------------------------
	void PlayLoop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] PlayLoop start");
		#endif
		
		PlaySoundSetLoop( m_SoundLoop, "Expansion_C130J_Loop_SoundSet", 0.0, 0.0 );
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] PlayLoop end");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane SetupPlane
	// ------------------------------------------------------------
	void SetupPlane( vector dropPosition, string name, float maxRadius, float speed )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] SetupPlane start");
		#endif

		vector position = GetPosition();

		m_Speed = speed;
		
		// Message( PlayerBase.Cast( GetGame().GetPlayer() ), "m_Speed " + m_Speed );
		// Message( PlayerBase.Cast( GetGame().GetPlayer() ), "speed " + speed );

		m_SpawnRadius = Math.RandomFloat( 0, maxRadius );

		m_AirdropName = name;

		m_AirdropPosition = Vector( dropPosition[0], position[1], dropPosition[2] );

		SetDirection( position - m_AirdropPosition );

		m_HeadingAngle = Math.Atan2( m_AirdropPosition[2] - position[2], m_AirdropPosition[0] - position[0] );
		
		m_PreviousDistance = vector.Distance( position, Vector( m_AirdropPosition[0], position[1], m_AirdropPosition[2] ) );

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
			vector velocity = Vector( Math.Cos( m_HeadingAngle ), 0, Math.Sin( m_HeadingAngle ) );

			velocity[0] = velocity[0] * m_Speed * dt;
			velocity[2] = velocity[2] * m_Speed * dt;

			vector transform[4];
			GetTransform( transform );
			transform[3] = transform[3] + velocity;
			transform[3][1] = m_Height;

			//vector transform[4];
			//GetTransform( transform );

			//transform[0] = { Math.Sin( m_HeadingAngle ), 0, Math.Cos( m_HeadingAngle ) };
			//transform[1] = { 0, 1, 0 };
			//transform[2] = { Math.Cos( m_HeadingAngle ), 0, Math.Sin( m_HeadingAngle ) };

			//vector velocity = transform[2] * m_Speed * dt;

			//transform[3] = transform[3] + velocity;
			////transform[3][1] = m_Height;

			MoveInTime( transform, dt );
		}

		m_RotorAnimationPosition += 1 * dt * 20.0 / ( Math.PI * 2.0 );
		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		SetAnimationPhase( "rotor", m_RotorAnimationPosition );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] EOnSimulate end");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane CheckForRemove
	// ------------------------------------------------------------
	bool CheckForRemove()
	{
		vector position = GetPosition();
		if ( !IsInRect( position[0], position[2], -10.0, 15470.0, -10.0, 15470.0 ) )
		{
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionAirdropPlane CheckForRemove
	// ------------------------------------------------------------
	bool IsWarningProximity()
	{
		return m_WarnedProximity;
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane CheckForRemove
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

		m_WarnedProximity = false;

		if ( distance <= 1500 && !m_HasWarnedProximity )
		{
			m_HasWarnedProximity = true;
			m_WarnedProximity = true;	
		} else if ( !m_AirdropCreated )
		{
			if ( distance <= m_SpawnRadius )
			{
				return true;
			}
		}

		m_PreviousDistance = distance;

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] CheckPosition end");
		#endif

		return false;
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
	
		// GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_EVENT_DROP", m_AirdropName ), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 7 );
		Object obj = GetGame().CreateObjectEx( container, dropPosition, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_AIRBORNE );
		
		ExpansionAirdropContainerBase drop;
		if ( Class.CastTo( drop, obj ) )
		{
			drop.SetPosition( dropPosition );	
			drop.SetOrientation( "0 0 0" );
		}

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("[ExpansionAirdropPlane] CreateDrop end");
		#endif

		return drop;
	}

	// ------------------------------------------------------------
	// ExpansionAirdropPlane IsInRect
	// ------------------------------------------------------------
	protected bool IsInRect(float x, float y, float min_x, float max_x, float min_y, float max_y) 
	{
		if ( x > min_x && x < max_x && y > min_y && y < max_y ) return true;
		return false;
	}
}