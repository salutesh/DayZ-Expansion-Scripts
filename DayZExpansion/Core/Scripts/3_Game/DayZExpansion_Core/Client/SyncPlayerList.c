/**
 * SyncPlayerList.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef DAYZ_1_23
modded class SyncPlayerList
{
	override void CreatePlayerList()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER, this);
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
					//sync_player.m_UID = p_identity.GetPlainId();
					//sync_player.m_RUID = p_identity.GetId();
					p_identity.Expansion_PlainIdToInt(sync_player.m_Expansion_PlainId);
				#ifdef EXPANSION_SYNCPLAYERLIST_SENDID
					p_identity.Expansion_IdToInt(sync_player.m_Expansion_Id);
				#endif
					sync_player.m_PlayerName = p_identity.GetName();
					m_PlayerList.Insert( sync_player );
				} else
				{
					DebugPrint.LogErrorAndTrace("No Identity in Server Player List");
				}
			}
		}
	}

	void Expansion_Decode()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER, this);
#endif

		foreach (SyncPlayer syncPlayer: m_PlayerList)
		{
			syncPlayer.m_UID = PlayerIdentity.Expansion_PlainIdToString(syncPlayer.m_Expansion_PlainId);
		#ifdef EXPANSION_SYNCPLAYERLIST_SENDID
			syncPlayer.m_RUID = PlayerIdentity.Expansion_IdToString(syncPlayer.m_Expansion_Id);
		#else
			syncPlayer.m_RUID = PlayerIdentity.Expansion_PlainIdToId(syncPlayer.m_UID);
		#endif
		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.PLAYER, this, string.Format("UID %1 RUID %2", syncPlayer.m_UID, syncPlayer.m_RUID));
		#endif
		}
	}
};
#endif
