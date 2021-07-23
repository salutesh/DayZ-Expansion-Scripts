/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionServer
 * @brief		
 **/

modded class MissionServer
{
	ExpansionRespawnHandlerModule m_RespawnHandlerModule;

	// ------------------------------------------------------------
	// MissionServer Constructor
	// ------------------------------------------------------------
	void MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - Start");
		#endif

		Class.CastTo(m_RespawnHandlerModule, GetModuleManager().GetModule(ExpansionRespawnHandlerModule));

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// MissionServer Destructor
	// ------------------------------------------------------------
	void ~MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - Start");
		#endif
		
		DestroyNotificationSystem();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// EquipCharacter
	// ------------------------------------------------------------
	override void EquipCharacter(MenuDefaultCharacterData char_data)
	{
		if (GetExpansionSettings().GetSpawn().StartingClothing.EnableCustomClothing)
		{
			m_RespawnHandlerModule.ExpansionEquipCharacter(m_player);
		} else
		{
			super.EquipCharacter(char_data);
		}
		
		if ( GetExpansionSettings().GetSpawn().SpawnHealthValue <= m_player.GetMaxHealth("GlobalHealth", "Health") )
			m_player.SetHealth("", "", GetExpansionSettings().GetSpawn().SpawnHealthValue );
		
		if ( GetExpansionSettings().GetSpawn().SpawnEnergyValue <= m_player.GetStatEnergy().GetMax() )
			m_player.GetStatEnergy().Set( GetExpansionSettings().GetSpawn().SpawnEnergyValue );
		
		if ( GetExpansionSettings().GetSpawn().SpawnWaterValue <= m_player.GetStatWater().GetMax()  )
			m_player.GetStatWater().Set( GetExpansionSettings().GetSpawn().SpawnWaterValue );
	}
	
	// ------------------------------------------------------------
	// OnClientNewEvent
	// ------------------------------------------------------------
	override PlayerBase OnClientNewEvent( PlayerIdentity identity, vector pos, ParamsReadContext ctx )
	{
		PlayerBase player = super.OnClientNewEvent( identity, pos, ctx );
		
		if ( GetExpansionSettings().GetSpawn().EnableSpawnSelection )
		{
			int spawnLocationsCount = GetExpansionSettings().GetSpawn().SpawnLocations.Count();
			bool territorySpawn = GetExpansionSettings().GetSpawn().SpawnOnTerritory && m_RespawnHandlerModule.GetTerritoryList( identity );

			if ( spawnLocationsCount || territorySpawn )
			{
				m_RespawnHandlerModule.ShowSpawnSelection( player.GetIdentity() );
			}
		}

		return player;
	} 
}
