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
		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );
		#endif
	}

	void ~ExpansionAirdropContainerManager()
	{
		Print("~ExpansionAirdropContainerManager");
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
		if ( infected )
		{
			//! Make it drop dead
			infected.SetHealth( 0 );

			//! Remove dead body shortly after
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( RemoveSingleInfected, 3000, false, infected );
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

	void RemoveSingleInfected( Object infected )
	{
		Print("ExpansionAirdropContainerManager::RemoveSingleInfected");

		if ( infected )
			GetGame().ObjectDelete( infected );
	}

	void Send_SpawnParticle( string spawnPosStr )
	{
		vector spawnPos = spawnPosStr.ToVector();

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( spawnPos );
		rpc.Send( m_Container, ExpansionAirdropContainerBase.EXPANSION_AIRDROP_RPC_ZSPAWN_PARTICLE, true );
	}

	void SpawnInfected()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerManager::SpawnInfected - Start");
		#endif

		while ( m_InfectedCount < InfectedCount ) 
		{
			m_InfectedCount++;

			vector spawnPos = Vector( m_Container.GetPosition()[0] + Math.RandomFloat( -InfectedSpawnRadius, InfectedSpawnRadius ), 0, m_Container.GetPosition()[2] + Math.RandomFloat( -InfectedSpawnRadius, InfectedSpawnRadius ) );
			spawnPos[1] = GetGame().SurfaceY( spawnPos[0], spawnPos[2] );

			//! Have to convert vector to string for call queue

			int additionalDelay;
			if ( InfectedSpawnInterval > 0 )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( Send_SpawnParticle, InfectedSpawnInterval * m_InfectedCount, false, spawnPos.ToString( false ) );
				additionalDelay = 300;
			}

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( CreateSingleInfected, InfectedSpawnInterval * m_InfectedCount + additionalDelay, false, spawnPos.ToString( false ) );
		}

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerManager::SpawnInfected - End");
		#endif
	}

	void CreateSingleInfected( string spawnPosStr )
	{
		vector spawnPos = spawnPosStr.ToVector();
		string type = Infected.GetRandomElement();

		//! TODO: Create Z slightly in ground to give effect as if they emerge from underground? Also, is there a way to affect Z stance (crouching)?
		Object obj = GetGame().CreateObject( type, Vector( spawnPos[0], 0, spawnPos[2] ), false, true );

		if ( obj )
		{
			m_Infected.Insert( obj );
		} else
		{
			Print("[ExpansionAirdropContainerManager] Warning : '" + type + "' is not a valid type!");
		}
	}

	void CreateServerMarker()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerManager::CreateServerMarker - Start");
		#endif

		#ifdef EXPANSIONMODNAVIGATION
		if ( !m_MarkerModule )
			return;
		
		string markerName = "#STR_EXPANSION_AIRDROP_SYSTEM_TITLE";
		if ( GetExpansionSettings().GetAirdrop().ShowAirdropTypeOnMarker )
			markerName = m_Container.GetDisplayName();
		
		m_ServerMarker = m_MarkerModule.CreateServerMarker( markerName, "Airdrop", m_Container.GetPosition(), ARGB(255, 235, 59, 90), GetExpansionSettings().GetAirdrop().Server3DMarkerOnDropLocation );
		#endif

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionAirdropContainerManager::CreateServerMarker - End");
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
