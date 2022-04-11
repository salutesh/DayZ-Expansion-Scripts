/**
 * ExpansionMarkerClientData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

#ifdef EXPANSIONMODNAVIGATION
modded class ExpansionMarkerClientData
{
	ExpansionPartyModule m_PartyModule;

	void ExpansionMarkerClientData()
	{
		Class.CastTo(m_PartyModule, GetModuleManager().GetModule(ExpansionPartyModule));
	}

	override int PartyCount()
	{
		if (!m_PartyModule || !m_PartyModule.GetParty())
			return 0;

		return m_PartyModule.GetParty().GetAllMarkers().Count();
	}

	override ExpansionMarkerData PartyGet(int i)
	{
		return m_PartyModule.GetParty().GetAllMarkers()[i];
	}

	override ExpansionMarkerData PartyGet(string uid)
	{
		array<ref ExpansionMarkerData> markers = m_PartyModule.GetParty().GetAllMarkers();
		for (int i = 0; i < markers.Count(); ++i)
		{
			if (markers[i].GetUID() == uid)
			{
				return markers[i];
			}
		}

		return NULL;
	}

	override void PartyInsert(ExpansionMarkerData data)
	{
		m_PartyModule.CreateMarker(data);
	}

	override void PartyUpdate(ExpansionMarkerData data)
	{
		m_PartyModule.UpdateMarker(data);
	}

	override bool PartyRemove(string uid)
	{
		if (m_PartyModule)
		{
			m_PartyModule.DeleteMarker(uid);
			return true;
		}

		return false;
	}

	override int PartyPlayerCount()
	{
		if (!m_PartyModule || !m_PartyModule.GetParty())
			return 0;

		return m_PartyModule.GetParty().GetPlayers().Count();
	}

	override ExpansionPlayerMarkerData PartyPlayerGet(int i)
	{
		return m_PartyModule.GetParty().GetPlayers()[i].Marker;
	}

	override ExpansionPlayerMarkerData PartyPlayerGet(string uid)
	{
		return m_PartyModule.GetParty().GetPlayer(uid).Marker;
	}

	override int PartyQuickCount()
	{
		if (!m_PartyModule || !m_PartyModule.GetParty())
			return 0;

		return m_PartyModule.GetParty().GetPlayers().Count();
	}

	override ExpansionPartyQuickMarkerData PartyQuickGet(int i)
	{
		return m_PartyModule.GetParty().GetPlayers()[i].QuickMarker;
	}

	override ExpansionPartyQuickMarkerData PartyQuickGet(string uid)
	{
		return m_PartyModule.GetParty().GetPlayer(uid).QuickMarker;
	}

	private override void RefreshPartyMarkers()
	{
		if (!m_PartyModule || !m_PartyModule.HasParty())
		{
			if (m_PartyModule && m_PartyModule.IsClientInitialized())
			{
				//! Player was in party and has left party, clear party marker info
				//! @note party marker info is *only* synched when changed, so *only* clear if party module is initialized!
				m_MarkerInfo_Party.Clear();
			}

			//! Player was in party and has left party, clear player marker info
			//! @note party player marker info is synched on each update, so it's ok to clear here
			m_MarkerInfo_PartyPlayers.Clear();

			return;
		}

		ExpansionMarkerData data;
		ExpansionMarkerClientInfo info;

		ExpansionPartyData party = m_PartyModule.GetParty();

		if (!party.m_MarkersSynced)
			return;

		int index;
		string uid;
		int removeIndex;

		//! Refresh client info for party markers
		array<string> checkArr = m_MarkerInfo_Party.GetKeyArray();
		array<ref ExpansionMarkerData> markers = party.GetAllMarkers();

		for (index = 0; index < markers.Count(); ++index)
		{
			uid = markers[index].GetUID();
			removeIndex = checkArr.Find(uid);
			if (removeIndex != -1)
			{
				//! Already have client info for this marker
				checkArr.Remove(removeIndex);
				markers[index].ApplyVisibility(m_MarkerInfo_Party[uid].GetVisibility());
			}
			else
			{
				//! It's a new marker
				info = new ExpansionMarkerClientInfo(uid);
				m_MarkerInfo_Party.Insert(uid, info);
			}
		}

		for (index = 0; index < checkArr.Count(); ++index)
		{
			//! Remove client info for markers that have been removed
			m_MarkerInfo_Party.Remove(checkArr[index]);
		}

		//! Refresh client info for party member (player) markers
		checkArr = m_MarkerInfo_PartyPlayers.GetKeyArray();
		array<ref ExpansionPartyPlayerData> players = party.GetPlayers();

		for (index = 0; index < players.Count(); ++index)
		{
			uid = players[index].UID;
			removeIndex = checkArr.Find(uid);
			if (removeIndex != -1)
			{
				//! Already have client info for this marker
				checkArr.Remove(removeIndex);
				if (players[index].Marker)
					players[index].Marker.ApplyVisibility(m_MarkerInfo_PartyPlayers[uid].GetVisibility());
			}
			else
			{
				//! It's a new marker
				info = new ExpansionMarkerClientInfo(uid);
				m_MarkerInfo_PartyPlayers.Insert(uid, info);
			}
		}

		for (index = 0; index < checkArr.Count(); ++index)
		{
			//! Remove client info for markers that have been removed
			m_MarkerInfo_PartyPlayers.Remove(checkArr[index]);
		}
	}
};
#endif
