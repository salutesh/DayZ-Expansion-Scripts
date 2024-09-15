modded class ZombieBase
{
	float m_Expansion_OverrideAlertLevel;

	void Expansion_OverrideAlertLevel(float level)
	{
		m_Expansion_OverrideAlertLevel = level;
		//! XXX: Doesn't seem to affect mind state
		//GetInputController().OverrideAlertLevel(true, true, 1, m_Expansion_OverrideAlertLevel);
	}

#ifdef DIAG_DEVELOPER
	override bool HandleMindStateChange(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (m_Expansion_OverrideAlertLevel)
		{
			int mindState = pInputController.GetMindState();
			if (m_LastMindState != mindState)
			{
				int alertLevel = pInputController.GetAlertLevel();
				float alertInLevel = pInputController.GetAlertInLevel();
				bool isAlerted = pInputController.IsAlerted();
				string mindStateStr;
				switch (mindState)
				{
					case DayZInfectedConstants.MINDSTATE_CALM:
						mindStateStr = "MINDSTATE_CALM(" + mindState + ")";
						break;
					case DayZInfectedConstants.MINDSTATE_DISTURBED:
						mindStateStr = "MINDSTATE_DISTURBED(" + mindState + ")";
						break;
					case DayZInfectedConstants.MINDSTATE_ALERTED:
						mindStateStr = "MINDSTATE_ALERTED(" + mindState + ")";
						break;
					case DayZInfectedConstants.MINDSTATE_CHASE:
						mindStateStr = "MINDSTATE_CHASE(" + mindState + ")";
						break;
					case DayZInfectedConstants.MINDSTATE_FIGHT:
						mindStateStr = "MINDSTATE_FIGHT(" + mindState + ")";
						break;
				}
				EXTrace.Print(EXTrace.MISSIONS, this, "mindstate " + mindStateStr + " alerted " + isAlerted + " level " + alertLevel + " inlevel " + alertInLevel);
			}
		}

		return super.HandleMindStateChange(pCurrentCommandID, pInputController, pDt);
	}
#endif
}
