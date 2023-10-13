/**
 * MissionBaseWorld.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionBaseWorld
{
#ifdef EXPANSIONMODAI
    //! Called from ExpansionHardlineModule::OnFactionChange
	void Expansion_OnPlayerFactionChange(PlayerBase player, int oldFactionID, int newFactionID);
#endif

    //! Called from ExpansionHardlineModule::HandlePlayerKilledPlayer
    void Expansion_OnPlayerKilledPlayer(PlayerBase killer, PlayerBase victim, int reputation, bool victimIsAI);

#ifdef EXPANSIONMODAI
    //! Called from ExpansionHardlineModule::HandlePlayerKilledByAI
    void Expansion_OnPlayerKilledAI(PlayerBase killerAI, PlayerBase victim, int reputation);
#endif

    //! Called from ExpansionHardlineModule::HandlePlayerKilledEntity
    void Expansion_OnPlayerKilledEntity(PlayerBase killer, EntityAI victim, int reputation);

    //! Called from ExpansionHardlineModule::HandlePlayerDeath
    void Expansion_OnPlayerDeath(PlayerBase victim, int removedReputation);
};
