#ifdef DIAG_DEVELOPER
modded class ZmbM_Gamedev_Base
{
	override bool HandleMindStateChange(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		int mindState = pInputController.GetMindState();
		EntityAI target = pInputController.GetTargetEntity();

		if (mindState != m_LastMindState || target != m_Expansion_Target)
		{
			string ms;

			switch (mindState)
			{
				case DayZInfectedConstants.MINDSTATE_CALM:
					ms = "CALM";
					break;
				case DayZInfectedConstants.MINDSTATE_DISTURBED:
					ms = "DISTURBED";
					break;
				case DayZInfectedConstants.MINDSTATE_ALERTED:
					ms = "ALERTED";
					break;
				case DayZInfectedConstants.MINDSTATE_CHASE:
					ms = "CHASE";
					break;
				case DayZInfectedConstants.MINDSTATE_FIGHT:
					ms = "FIGHT";
					break;
				default:
					ms = "UNKNOWN";
					break;
			}

			ExpansionStatic.MessageNearPlayers(GetPosition(), 100, "state=" + ms + "(" + mindState + ") target=" + target);
		}

		return super.HandleMindStateChange(pCurrentCommandID, pInputController, pDt);
	}
}
#endif
