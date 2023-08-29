#ifdef EXPANSION_MODSTORAGE
modded class PlayerBase
{
	protected static ref TStringArray s_Expansion_QueuedIdentityIDs = new TStringArray();
	protected string m_Expansion_QueuedIdentityID;

	static void Expansion_QueueIdentityId(string id)
	{
#ifdef EXPANSION_MODSTORAGE_DEBUG
		EXPrint("[MODSTORAGE] Queueing identity ID " + id);
#endif
		s_Expansion_QueuedIdentityIDs.Insert(id);
	}

	static string Expansion_DequeueIdentityId()
	{
		string id = s_Expansion_QueuedIdentityIDs[0];
		s_Expansion_QueuedIdentityIDs.RemoveOrdered(0);
		return id;
	}

	string Expansion_GetQueuedIdentityId()
	{
		//! Always prefer identity when it is available
		if (GetIdentity())
		{
			if (m_Expansion_QueuedIdentityID != GetIdentity().GetId())
			{
				if (m_Expansion_QueuedIdentityID)
					EXPrint("[MODSTORAGE] " + ToString() + " WARNING: Queued identity ID " + m_Expansion_QueuedIdentityID + " doesn't match actual ID " + GetIdentity().GetId());
				m_Expansion_QueuedIdentityID = GetIdentity().GetId();
			}
		}
		else if (!m_Expansion_QueuedIdentityID && s_Expansion_QueuedIdentityIDs.Count())
		{
			array<PlayerIdentity> identities();
			GetDayZGame().GetPlayerIndentities(identities);
			string id;
			while (s_Expansion_QueuedIdentityIDs.Count())
			{
				id = Expansion_DequeueIdentityId();
				if (!id) break;
				//! Make sure this is not a stale ID. If it's not in the list,
				//! the player didn't finish connecting (client crash, connection error etc)
				foreach (PlayerIdentity identity: identities)
				{
					if (id == identity.GetId())
					{
#ifdef EXPANSION_MODSTORAGE_DEBUG
						EXPrint("[MODSTORAGE] " + ToString() + " got queued identity ID " + id);
#endif
						m_Expansion_QueuedIdentityID = id;
						return m_Expansion_QueuedIdentityID;
					}
				}
			}
		}

		return m_Expansion_QueuedIdentityID;
	}
}
#endif
