/**
 * SyncPlayerList.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class SyncPlayerList
{
	// ------------------------------------------------------------
	// Override CreatePlayerList
	// ------------------------------------------------------------
	override void CreatePlayerList()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("SyncPlayerList::CreatePlayerList - Start");
		#endif

		if ( GetGame().IsServer() )
		{
			if ( m_PlayerList )
			{
				m_PlayerList.Clear();
			} else
			{
				m_PlayerList = new array< ref SyncPlayer >;
			}

			array< Man > players = new array< Man >;
			GetGame().GetWorld().GetPlayerList( players );

			for ( int i = 0; i < players.Count(); ++i )
			{
				Man player = players[i];
				PlayerIdentity p_identity = player.GetIdentity();

				if  ( p_identity )
				{
					SyncPlayer sync_player = new SyncPlayer;
					sync_player.m_UID = p_identity.GetPlainId();
					sync_player.m_RUID = p_identity.GetId();
					sync_player.m_SteamID = p_identity.GetPlainId();
					sync_player.m_PlayerID = p_identity.GetPlayerId();
					sync_player.m_PlayerName = p_identity.GetName();
					m_PlayerList.Insert( sync_player );
				} else
				{
					DebugPrint.LogErrorAndTrace("No Identity in Server Player List");
				}
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("SyncPlayerList::CreatePlayerList - End");
		#endif
	}
};