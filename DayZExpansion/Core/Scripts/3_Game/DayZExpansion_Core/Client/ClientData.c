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
#ifdef DAYZ_1_23
	override static void SyncEvent_OnRecievedPlayerList( SyncPlayerList player_list )
	{
		auto trace = EXTrace.Start(EXTrace.PLAYER, ClientData);

		if (m_PlayerList && m_PlayerList.m_PlayerList)
			m_PlayerList.Expansion_Decode();

		if (player_list && player_list.m_PlayerList)
			player_list.Expansion_Decode();
		
		SyncPlayerList new_players = SyncPlayerList.Compare(m_PlayerList, player_list);
		
		if ( !m_LastNewPlayers )
		{
			m_LastNewPlayers = player_list;
		}
		else
		{
			m_LastNewPlayers = SyncPlayerList.Compare( m_LastNewPlayers, new_players );
		}
		
		m_PlayerList = SyncPlayerList.Compare(player_list, null);
		
		#ifdef PLATFORM_CONSOLE
			#ifndef PLATFORM_WINDOWS // if app is not on Windows with -XBOX parameter
				array<string> newPlayersSimple = GetSimplePlayerList( new_players );
				OnlineServices.LoadPermissions( newPlayersSimple );
				OnlineServices.m_ClientServices.GetSessionService().OnSessionPlayerListUpdate(newPlayersSimple);
				
				if ( new_players.m_PlayerList.Count() > 0 )
				{
					OnlineServices.LoadFriends();
				}
		
				if ( g_Game.GetGameState() == DayZGameState.IN_GAME )
				{
					OnlineServices.SetMultiplayState(OnlineServices.GetMultiplayState());
				}
			#endif
		#endif
		
		SyncEvent_OnPlayerListUpdate.Invoke( player_list );
	}
#else
	//! 1.24+
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
#endif
}
