/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	}
	
	// ------------------------------------------------------------
	// OnClientNewEvent
	// ------------------------------------------------------------
	override PlayerBase OnClientNewEvent( PlayerIdentity identity, vector pos, ParamsReadContext ctx )
	{
		PlayerBase player = super.OnClientNewEvent( identity, pos, ctx );
		
		float spawnHealth = GetExpansionSettings().GetSpawn().SpawnHealthValue;
		if (spawnHealth > 0 && spawnHealth <= player.GetMaxHealth("GlobalHealth", "Health"))
			player.SetHealth("", "", spawnHealth );
		
		if (GetExpansionSettings().GetSpawn().SpawnEnergyValue <= player.GetStatEnergy().GetMax())
			player.GetStatEnergy().Set(GetExpansionSettings().GetSpawn().SpawnEnergyValue);
		
		if (GetExpansionSettings().GetSpawn().SpawnWaterValue <= player.GetStatWater().GetMax())
			player.GetStatWater().Set(GetExpansionSettings().GetSpawn().SpawnWaterValue);
		
		if (GetExpansionSettings().GetSpawn().EnableSpawnSelection)
			m_RespawnHandlerModule.StartSpawnSelection(player);

		return player;
	} 
}
