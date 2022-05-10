/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		CF_Modules<ExpansionRespawnHandlerModule>.Get(m_RespawnHandlerModule);
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
}
