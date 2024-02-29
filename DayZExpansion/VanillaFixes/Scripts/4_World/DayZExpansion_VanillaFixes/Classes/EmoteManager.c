modded class EmoteManager
{
	//! Restore emote logging behavior to 1.23 (https://feedback.bistudio.com/T179312)
	override bool PlayEmote(int id)
	{
		PluginAdminLog adminLog;

		if (m_AdminLog && !LogManager.IsActionLogEnable())
		{
			adminLog = m_AdminLog;
			m_AdminLog = null;
		}

		bool ret = super.PlayEmote(id);

		if (adminLog)
			m_AdminLog = adminLog;

		return ret;
	}
}
