/**
 * ClientData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ClientData
{
	override static void SyncEvent_PreprocessPlayerList(SyncPlayerList player_list)
	{
		foreach (auto sync_player : player_list.m_PlayerList)
		{
			PlayerIdentity identity = sync_player.m_Identity;
			if (!identity)
			{
				ErrorEx("PlayerIdentity not synchronized before SyncPlayer", ErrorExSeverity.WARNING);
				continue;
			}

			sync_player.m_UID = identity.GetPlainId();
			sync_player.m_RUID = identity.GetId();
			sync_player.m_PlayerName = identity.GetPlainName();
		}
	}
}
