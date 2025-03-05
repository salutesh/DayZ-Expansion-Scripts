/**
 * ExpansionAirdropContainerManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAirdropContainerManager
 * @brief		Keeps track of Infected and server marker associated to airdrop container. Removes them if container gets deleted.
 **/
class ExpansionAirdropContainerManager
{
	ExpansionAirdropContainerBase m_Container;
	vector m_ContainerPosition;
	protected autoptr array< Object > m_Infected;
	protected int m_InfectedCount;

	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	ref TStringArray Infected;
	int InfectedCount;
	float InfectedSpawnRadius;
	int InfectedSpawnInterval;

	//! Noise
	protected ref NoiseParams m_NoisePar;
	protected NoiseSystem m_NoiseSystem;
	float m_NoiseTickTime;

	void ExpansionAirdropContainerManager( ExpansionAirdropContainerBase container, TStringArray infected, int infectedCount )
	{
		m_Container = container;
		m_ContainerPosition = container.GetPosition();
		m_Infected = new array< Object >;

		Infected = infected;
		InfectedCount = infectedCount;
		InfectedSpawnRadius = GetExpansionSettings().GetAirdrop().InfectedSpawnRadius;
		if ( InfectedSpawnRadius == 0 )  //! Backwards compatibility with old settings
			InfectedSpawnRadius = 50;
		InfectedSpawnInterval = GetExpansionSettings().GetAirdrop().InfectedSpawnInterval;

		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif

		m_NoiseSystem = GetGame().GetNoiseSystem();
		if (m_NoiseSystem && !m_NoisePar)
		{
			m_NoisePar = new NoiseParams();
			m_NoisePar.LoadFromPath("CfgVehicles Roadflare NoiseRoadFlare");
		}
	}

	void ~ExpansionAirdropContainerManager()
	{
		Print("~ExpansionAirdropContainerManager");

		StopUpdateNoise();
	}

	void Cleanup()
	{
		m_Container = NULL;

		Print("[ExpansionAirdropContainerManager] Container at " + m_ContainerPosition + " was deleted");

		RemoveServerMarker();
		RemoveInfected();
	}

	void KillSingleInfected( Object infected )
	{
		if ( infected && infected.IsAlive() )
		{
			//! Only kill if not currently chasing/fighting

			ZombieBase zombie;
			AnimalBase animal;
		#ifdef EXPANSIONMODAI
			eAIBase ai;
		#endif
			if (Class.CastTo(zombie, infected) && zombie.GetInputController().GetMindState() >= DayZInfectedConstants.MINDSTATE_CHASE)
			{
				zombie.SetLifetime(60.0);
			}
			else if (Class.CastTo(animal, infected) && animal.GetInputController().GetAlertLevel() > 1)
			{
				animal.SetLifetime(60.0);
			}
		#ifdef EXPANSIONMODAI
			else if (Class.CastTo(ai, infected))
			{
				if (ai.GetThreatToSelf() >= 0.4)
					ai.eAI_SetDespawnOnLoosingAggro(true);
				else
					ai.eAI_Despawn();
			}
		#endif
			else
			{
				//! Make it drop dead
				infected.SetHealth( 0 );

				//! Remove dead body shortly after
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( DeleteSingleInfected, 3000, false, infected );
			}
		}

		//! On to the next
		RemoveInfected();
	}

	void RemoveInfected()
	{
		if ( m_Infected.Count() > 0 )
		{
			int index = m_Infected.Count() - 1;

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( KillSingleInfected, InfectedSpawnInterval, false, m_Infected[ index ] );

			m_Infected.Remove( index );
		} else
		{
			m_Infected = NULL;

			ExpansionAirdropContainerManagers.Remove( this );
		}
	}

	void DeleteSingleInfected( Object infected )
	{
		Print("ExpansionAirdropContainerManager::DeleteSingleInfected");

		if ( infected )
			GetGame().ObjectDelete( infected );
	}

	void Send_SpawnParticle( string spawnPosStr )
	{
		if (!m_Container)
			return;

		vector spawnPos = spawnPosStr.ToVector();

		auto rpc = ExpansionScriptRPC.Create(ExpansionAirdropContainerBase.s_Expansion_SpawnParticle_RPCID);
		rpc.Write( spawnPos );
		PlayerBase.Expansion_Send(rpc, m_Container, true);
	}

	void SpawnInfected()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		while ( m_InfectedCount < InfectedCount ) 
		{
			m_InfectedCount++;

			vector spawnPos = ExpansionMath.GetRandomPointInRing(m_Container.GetPosition(), InfectedSpawnRadius * 0.1, InfectedSpawnRadius);
			spawnPos[1] = GetGame().SurfaceY( spawnPos[0], spawnPos[2] );

			//! Have to convert vector to string for call queue

			int additionalDelay;
			if ( InfectedSpawnInterval > 0 )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( Send_SpawnParticle, InfectedSpawnInterval * m_InfectedCount, false, spawnPos.ToString( false ) );
				additionalDelay = Math.RandomFloat(100, 300);
			}

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( CreateSingleInfected, InfectedSpawnInterval * m_InfectedCount + additionalDelay, false, spawnPos.ToString( false ), m_InfectedCount );
		}
	}

	void CreateSingleInfected( string spawnPosStr, int count )
	{
		vector spawnPos = spawnPosStr.ToVector();
		string type = Infected.GetRandomElement();

#ifdef ENFUSION_AI_PROJECT
		bool isAI = type.IndexOf("eAI_Survivor") == 0;
		TStringArray parts();
		if (isAI)
		{
			type.Split("|", parts);
			type = parts[0];
			parts.Remove(0);
		}
#endif

		//! TODO: Create Z slightly in ground to give effect as if they emerge from underground? Also, is there a way to affect Z stance (crouching)?
		Object obj = GetGame().CreateObject( type, spawnPos, false, GetGame().IsKindOf(type, "DZ_LightAI") );

		if ( obj )
		{
#ifdef ENFUSION_AI_PROJECT
			DayZPlayerImplement ai;
			if (isAI && Class.CastTo(ai, obj) && parts.Count())
			{
				ExpansionObjectSpawnTools.ProcessGear(ai, parts[0]);
			}
#endif

			m_Infected.Insert( obj );

			//! Alert Infected/animals instantly after spawning
			//! See DZ\AI\config.cpp, CfgAIBehaviors -> AlertSystem
			ZombieBase zombie;
			AnimalBase animal;
			if (Class.CastTo(zombie, obj))
				zombie.Expansion_OverrideAlertLevel(21.0);
			else if (Class.CastTo(animal, obj))
				animal.GetInputController().OverrideAlertLevel(true, true, 1, 31.0);  //! Should work for wolf/bear

			if (m_NoiseSystem && m_Container)
				m_NoiseSystem.AddNoiseTarget(m_Container.GetPosition(), 0.34, m_NoisePar);
		} else
		{
			Print("[ExpansionAirdropContainerManager] Warning : '" + type + "' is not a valid type!");
		}

		if (count == InfectedCount)  //! Periodic noise at container to attract Infected
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateNoise, 1000, true);
	}

	//! Make "noise" around the container which AI like Infected will "hear" and get alerted by
	void UpdateNoise()
	{
		if (!m_NoiseSystem || !m_Container)
			return;

		//! Add noise if player within 5 m of container or 7-10 seconds elapsed since last noise
		float tickTime = GetGame().GetTickTime();
		if (ExpansionLootSpawner.IsPlayerNearby(m_Container, 5.0) || tickTime - m_NoiseTickTime > Math.RandomFloat(7.0, 10.0))
		{
			m_NoiseTickTime = tickTime;
			m_NoiseSystem.AddNoiseTarget(m_Container.GetPosition(), 1.0, m_NoisePar);
		}
	}

	void StopUpdateNoise()
	{
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateNoise);
	}

	void CreateServerMarker()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		#ifdef EXPANSIONMODNAVIGATION
		if ( !m_MarkerModule )
			return;
		
		string markerName = "#STR_EXPANSION_AIRDROP_SYSTEM_TITLE";
		if ( GetExpansionSettings().GetAirdrop().ShowAirdropTypeOnMarker )
			markerName = m_Container.Expansion_GetDisplayNameRaw();
		
		vector surfacePosition = ExpansionStatic.GetSurfacePosition(m_Container.m_SpawnPosition);

		vector hitPosition;
		vector contactDir;
		int contactComponent;
		set<Object> results = new set<Object>;

		if (!DayZPhysics.RaycastRV(m_Container.m_SpawnPosition, surfacePosition, hitPosition, contactDir, contactComponent, results, null, m_Container))
			hitPosition = surfacePosition;

		m_ServerMarker = m_MarkerModule.CreateServerMarker( markerName, "Airdrop", hitPosition, ARGB(255, 235, 59, 90), GetExpansionSettings().GetAirdrop().Server3DMarkerOnDropLocation );
		#endif
	}

	void RemoveServerMarker()
	{
		#ifdef EXPANSIONMODNAVIGATION
		if ( !m_ServerMarker )
			return;
		
		m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif
	}
};
