modded class PlayerBase
{
	protected static ref TStringArray s_CF_QueuedIdentityIDs = new TStringArray();
	protected string m_CF_QueuedIdentityID;

	static void CF_QueueIdentityId(string id)
	{
		s_CF_QueuedIdentityIDs.Insert(id);
	}

	static string CF_DequeueIdentityId()
	{
		string id = s_CF_QueuedIdentityIDs[0];
		s_CF_QueuedIdentityIDs.RemoveOrdered(0);
		return id;
	}

	string CF_GetQueuedIdentityId()
	{
		//! Always prefer identity when it is available
		if (GetIdentity())
		{
			if (m_CF_QueuedIdentityID != GetIdentity().GetId())
			{
				CF_Log.Warn("Queued identity ID %1 doesn't match actual ID %2 for player %3", m_CF_QueuedIdentityID, GetIdentity().GetId(), ToString());
				m_CF_QueuedIdentityID = GetIdentity().GetId();
			}
		}
		else if (!m_CF_QueuedIdentityID && s_CF_QueuedIdentityIDs.Count())
		{
			array<PlayerIdentity> identities();
			GetDayZGame().GetPlayerIndentities(identities);
			string id;
			while (s_CF_QueuedIdentityIDs.Count())
			{
				id = CF_DequeueIdentityId();
				if (!id) break;
				//! Make sure this is not a stale ID. If it's not in the list,
				//! the player didn't finish connecting (client crash, connection error etc)
				foreach (PlayerIdentity identity: identities)
				{
					if (id == identity.GetId())
					{
						m_CF_QueuedIdentityID = id;
						return m_CF_QueuedIdentityID;
					}
				}
			}
		}

		return m_CF_QueuedIdentityID;
	}
};
