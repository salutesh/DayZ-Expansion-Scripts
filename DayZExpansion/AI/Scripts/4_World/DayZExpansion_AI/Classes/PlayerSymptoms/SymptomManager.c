modded class SymptomManager
{
	override int GetCurrentCommandID()
	{
		if (m_Player.IsAI() && m_CurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT && eAICommandMove.Cast(m_Player.GetCommand_Script()))
		{
			if (m_Player.IsSwimming())
				return DayZPlayerConstants.COMMANDID_SWIM;

			return DayZPlayerConstants.COMMANDID_MOVE;
		}

		return m_CurrentCommandID;
	}
}
