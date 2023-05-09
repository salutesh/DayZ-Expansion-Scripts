modded class ZombieBase
{
	int m_Expansion_OverrideAlertLevel;

	override void EOnInit(IEntity other, int extra)
	{
		//! XXX: Need more info how OverrideAlertLevel is to be used
		//if (m_Expansion_OverrideAlertLevel)
			//GetInputController().OverrideAlertLevel(true, true, m_Expansion_OverrideAlertLevel, 1.0);
	}

#ifdef DIAG
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
