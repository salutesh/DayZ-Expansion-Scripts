modded class DigitalCodeLockUI
{
	bool m_Expansion_Change;

	override void Expansion_SetChange(bool change)
	{
		m_Expansion_Change = change;
	}

	override protected void Select()
	{
		super.Select();

		if (m_SelectedButtonIndex != CONFIRM_BUTTON_INDEX && m_SelectedButtonIndex != RESET_BUTTON_INDEX)
		{
			if (m_EnteredPIN && !GetExpansionClientSettings().ShowPINCode)
			{
				int codeLength = m_EnteredPIN.Length();
				if (codeLength < MAX_LOCK_PIN_LENGTH)
				{
					for (int i = 0; i < m_EnteredPIN.Length(); ++i)
					{
						TextWidget digitTextW = TextWidget.Cast(layoutRoot.FindAnyWidget("PasswordDigitNumber_Text" + i));
						if (digitTextW)
							digitTextW.SetText("*");
					}
				}
			}
		}
	}

	override protected void ConfirmCode()
	{
		if (m_Expansion_Change)
			Expansion_ChangeCode();
		else
			super.ConfirmCode();
	}

	void Expansion_ChangeCode()
	{
		if (m_EnteredPIN == "" || !m_TargetLock)
			return;

		int pinLength = m_EnteredPIN.Length();
		int minPINLength = MIN_LOCK_PIN_LENGTH;

		DigitalCodeLock codeLock;
		if (Class.CastTo(codeLock, m_TargetLock))
			minPINLength = codeLock.GetMinLockPINLength();

		if (pinLength < minPINLength || pinLength > MAX_LOCK_PIN_LENGTH)
			return;

		for (int i = 0; i < MAX_LOCK_PIN_LENGTH - 1; ++i)
		{
			TextWidget digitTextW = TextWidget.Cast(layoutRoot.FindAnyWidget("PasswordDigitNumber_Text" + i));
			if (digitTextW)
				digitTextW.SetText("");
		}

		auto rpc = ExpansionScriptRPC.Create(ItemBase.s_Expansion_ChangeCode_RPCID);
		rpc.Write(m_EnteredPIN);
		rpc.Write("");
		rpc.Expansion_Send(m_TargetLock, true);

		m_EnteredPIN = "";

		Close();
	}
}
