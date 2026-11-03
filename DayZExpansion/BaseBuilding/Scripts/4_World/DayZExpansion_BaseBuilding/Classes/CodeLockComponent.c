modded class CodeLockComponent
{
	override protected void CheckCodeRequest(string pin, PlayerIdentity identity)
	{
		bool hadPIN = m_HasPIN;
		bool wasLocked = m_IsLocked;
		string lockPINBefore = m_LockPIN;

		super.CheckCodeRequest(pin, identity);

		if (g_Game.IsServer() && GetExpansionSettings().GetBaseBuilding().RememberCode)
		{
			DigitalCodeLock codeLock;
			if (Class.CastTo(codeLock, m_Parent))
			{
				PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(identity);
				if (player)
				{
					if ((m_HasPIN && !hadPIN) || (m_LockPIN != lockPINBefore))  //! New PIN was set
					{
						codeLock.SetUser(player);
					}
					else if (hadPIN && m_HasPIN && pin == m_LockPIN && wasLocked && !m_IsLocked)  //! PIN was entered correctly (unlocked)
					{
						codeLock.AddUser(player);
					}
				}
			}
		}
	}
}
