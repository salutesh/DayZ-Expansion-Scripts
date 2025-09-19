/**
 * SyncPlayer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class SyncPlayer
{
#ifdef EXPANSION_SYNCPLAYERLIST_SENDID
	int m_Expansion_Id[9];
#endif

	[NonSerialized()]
	string m_RUID;

	void Expansion_CopyFrom(SyncPlayer syncPlayer)
	{
	#ifdef EXPANSION_SYNCPLAYERLIST_SENDID
		for (int i = 0; i < 9; i++)
		{
			m_Expansion_Id[i] = syncPlayer.m_Expansion_Id[i];
		}
	#endif
		m_UID = syncPlayer.m_UID;
		m_RUID = syncPlayer.m_RUID;
		m_PlayerName = syncPlayer.m_PlayerName;
	}

	//! @note ID can be either plain (steam64) ID or normal (hashed) ID
	static SyncPlayer Expansion_GetByID(string id)
	{
		if (!ClientData.m_PlayerList || !ClientData.m_PlayerList.m_PlayerList)
			return null;

		foreach (SyncPlayer syncPlayer: ClientData.m_PlayerList.m_PlayerList)
		{
			if (!syncPlayer)
				continue;

			if (syncPlayer.m_UID == id || syncPlayer.m_RUID == id)
			{
				//! Create a copy
				SyncPlayer player();
				player.Expansion_CopyFrom(syncPlayer);
				return player;
			}
		}

		return null;
	}

	static set<ref SyncPlayer> Expansion_GetAll()
	{
		set<ref SyncPlayer> players();

		if (!ClientData.m_PlayerList || !ClientData.m_PlayerList.m_PlayerList)
			return players;

		foreach (SyncPlayer syncPlayer: ClientData.m_PlayerList.m_PlayerList)
		{
			if (!syncPlayer)
				continue;

			//! Create a copy
			SyncPlayer player();
			player.Expansion_CopyFrom(syncPlayer);
			players.Insert(player);
		}
		
		return players;
	}

	static set<ref SyncPlayer> Expansion_GetInSphere(vector position, int radius)
	{
		int radiusSq = radius * radius;
		set<ref SyncPlayer> players();

		SyncPlayer syncPlayer;
		//! @note ClientData.m_PlayerBaseList only contains players in network bubble
		foreach (Man player: ClientData.m_PlayerBaseList)
		{
			if (!player.GetIdentity())
				continue;

			if (vector.DistanceSq(position, player.GetPosition()) > radiusSq)
				continue;

			syncPlayer = Expansion_GetByID(player.GetIdentity().GetId());
			if (syncPlayer)
				players.Insert(syncPlayer);
		}
		
		return players;
	}
};
