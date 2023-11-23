/**
 * SyncEvents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class SyncEvents
{
	static ref map<string, bool> s_Expansion_RespawningUIDs = new map<string, bool>;
	static bool s_Expansion_IsClientRespawn;

	override static void SendPlayerList()
	{
		if (s_Expansion_IsClientRespawn)
		{
			EXTrace.Print(EXTrace.PLAYER, SyncEvents, "Client respawn - not sending playerlist");
			s_Expansion_IsClientRespawn = false;
			return;
		}

		SyncData data = new SyncData;
		data.m_ServerPlayerList = new SyncPlayerList;
		data.m_ServerPlayerList.CreatePlayerList();

		SendSyncEvent( ERPCs.RPC_SYNC_EVENT, ESyncEvent.PlayerList, data, true );
	}
}
