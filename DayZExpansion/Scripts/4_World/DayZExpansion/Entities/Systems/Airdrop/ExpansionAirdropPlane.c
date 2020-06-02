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
	void ExpansionAirdropPlane()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] Constructor start");
		#endif

		if ( IsMissionClient() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( DeferredInit, 10000, false );
		}

		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE );

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] Constructor end");
		#endif
	}
	
	// ------------------------------------------------------------
	void ~ExpansionAirdropPlane()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] Deconstructor start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] Deconstructor end");
		#endif
	}
	
	override void EEDelete(EntityAI parent)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropPlane::EEDelete - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropPlane::EEDelete - End");
		#endif
	}

	void DeferredInit()
	{
		if ( IsMissionClient() )
		{
			PlayLoop();
		
			// SpawnLights();
		}
	}

	void SpawnLights()
	{
		/* 
		if (!m_Light1)
		{
			m_Light1 = ExpansionPointLight.Cast(GetGame().CreateObject("ExpansionPointLight", ModelToWorld("-20 0 -2.589028"), true));

			m_Light1.SetDiffuseColor(1, 0, 0);
			m_Light1.SetRadiusTo(500);
			m_Light1.SetBrightnessTo(0.01);
			m_Light1.SetPosition(WorldToModel(m_Light1.GetPosition()));

			AddChild(m_Light1, 0);
		}
		
		if (!m_Light2)
		{
			m_Light2 = ExpansionPointLight.Cast(GetGame().CreateObject("ExpansionPointLight", ModelToWorld("20 0 -2.589026"), true));

			m_Light2.SetDiffuseColor(1, 0, 0);
			m_Light2.SetRadiusTo(500);
			m_Light2.SetBrightnessTo(0.01);
			m_Light2.SetPosition(WorldToModel(m_Light2.GetPosition()));

			AddChild(m_Light2, 0);
		}
		
		if (!m_Light3)
		{
			m_Light3 = ExpansionPointLight.Cast(GetGame().CreateObject("ExpansionPointLight", ModelToWorld("0.001175 6 12.296854"), true));

			m_Light3.SetDiffuseColor(1, 1, 1);
			m_Light3.SetRadiusTo(500);
			m_Light3.SetBrightnessTo(0.01);
			m_Light3.SetPosition(WorldToModel(m_Light3.GetPosition()));

			AddChild(m_Light3, 0);
		}
		*/
	}
	
	void PlayLoop()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] PlayLoop start");
		#endif
		
		PlaySoundSetLoop( m_SoundLoop, "Expansion_C130J_Loop_SoundSet", 0.0, 0.0 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] PlayLoop end");
		#endif
	}

	void SetupPlane( vector dropPosition, string name, float maxRadius, float speed )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] SetupPlane start");
		#endif

		vector position = GetPosition();

		m_Speed = speed;

		m_SpawnRadius = Math.RandomFloat( 0, maxRadius );

		m_AirdropName = name;

		m_AirdropPosition = Vector( dropPosition[0], position[1], dropPosition[2] );

		SetDirection( position - m_AirdropPosition );

		m_HeadingAngle = Math.Atan2( m_AirdropPosition[2] - position[2], m_AirdropPosition[0] - position[0] );
		
		m_PreviousDistance = vector.Distance( position, Vector( m_AirdropPosition[0], position[1], m_AirdropPosition[2] ) );

		//GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_EVENT", m_AirdropName ), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 7 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] SetupPlane end");
		#endif
	}

	override void EOnSimulate( IEntity owner, float dt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] EOnSimulate start");
		#endif

		if ( IsMissionHost() )
		{
			vector position = GetPosition();

			vector velocity = Vector( Math.Cos( m_HeadingAngle ), 0, Math.Sin( m_HeadingAngle ) );

			velocity[0] = velocity[0] * m_Speed * dt;
			velocity[2] = velocity[2] * m_Speed * dt;

			vector transform[4];
			GetTransform( transform );
			transform[3] = transform[3] + velocity;
			transform[3][1] = m_Height;

			MoveInTime( transform, dt );
		}

		m_RotorAnimationPosition += 1 * dt * 20.0 / ( Math.PI * 2.0 );
		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		SetAnimationPhase( "rotor", m_RotorAnimationPosition );

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] EOnSimulate end");
		#endif
	}

	bool CheckForRemove()
	{
		vector position = GetPosition();
		if ( !IsInRect( position[0], position[2], -10.0, 15470.0, -10.0, 15470.0 ) )
		{
			return true;
		}

		return false;
	}

	bool IsWarningProximity()
	{
		return m_WarnedProximity;
	}

	bool CheckForDrop()
	{
		//! Print("CheckForDrop " + GetPosition() );

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] CheckPosition start");
		#endif

		if ( !IsMissionHost() )
			return false;

		//! Print("CheckForDrop 1");

		if ( GetPosition()[0] == 0 || GetPosition()[2] == 0 )
			return false;

		if ( m_AirdropPosition[0] == 0 || m_AirdropPosition[2] == 0 )
			return false;

		//! Print("CheckForDrop 2");

		vector position = GetPosition();

		bool drop = false;

		float distance = vector.Distance( position, Vector( m_AirdropPosition[0], position[1], m_AirdropPosition[2] ) );

		m_WarnedProximity = false;

		//! Print("CheckForDrop 3 " + distance);

		if ( distance <= 1500 && !m_HasWarnedProximity )
		{
			m_HasWarnedProximity = true;
			m_WarnedProximity = true;	
			
			//GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_EVENT_CLOSE", m_AirdropName ), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 7 );
		} else if ( !m_AirdropCreated )
		{
			//! Print("CheckForDrop 4 " + distance);
			//! Print("CheckForDrop 5 " + m_SpawnRadius);
			//! Print("CheckForDrop 6 " + m_PreviousDistance);

			if ( distance <= Math.RandomFloat( 10, m_SpawnRadius ) )
			{
				//! Print("CheckForDrop 7 " + m_PreviousDistance);

				return true;
			}
		}

		m_PreviousDistance = distance;

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] CheckPosition end");
		#endif

		return false;
	}

	ExpansionAirdropContainerBase CreateDrop( string container )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] CreateDrop start");
		#endif

		m_AirdropCreated = true;

		vector dropPosition = Vector( m_AirdropPosition[0], GetPosition()[1] - 10.0, m_AirdropPosition[2] );
	
		// GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_AIRDROP_SYSTEM_EVENT_DROP", m_AirdropName ), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 7 );
		Object obj = GetGame().CreateObject( container, dropPosition );

		ExpansionAirdropContainerBase drop;
		if ( Class.CastTo( drop, obj ) )
		{
			drop.SetPosition( dropPosition );	
			drop.SetOrientation( "0 0 0" );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropPlane] CreateDrop end");
		#endif

		return drop;
	}

	protected bool IsInRect(float x, float y, float min_x, float max_x, float min_y, float max_y) 
	{
		if ( x > min_x && x < max_x && y > min_y && y < max_y ) return true;
		return false;
	}
}