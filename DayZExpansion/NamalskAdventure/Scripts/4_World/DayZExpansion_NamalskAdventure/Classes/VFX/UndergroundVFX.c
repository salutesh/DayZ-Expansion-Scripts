#ifdef NAMALSK_ISLAND
modded class UndergroundVFX
{
	override private void UndergroundCheck()
	{
		//! Namalsk adventure bunker entrance
		if (((m_PlayerPos[0]) > 4223) && ((m_PlayerPos[0]) < 4229))
		{
			if (((m_PlayerPos[2]) > 8205) && ((m_PlayerPos[2]) < 8220))
			{
				if ((m_PlayerPos[1]) > 79 && ((m_PlayerPos[1]) < 81))
				{
					m_ExposureDtTarget = 0.1;
					return;
				}
				else if ((m_PlayerPos[1]) >= 81)
				{
					m_ExposureDtTarget = 0.0;
					return;
				}
			}
		}
		//! Namalsk adventure bunker
		if (((m_PlayerPos[0]) > 1800) && ((m_PlayerPos[0]) < 2000))
		{
			if (((m_PlayerPos[2]) > 1100) && ((m_PlayerPos[2]) < 1500))
			{
				m_ExposureDtTarget = 0.0;
				return;
			}
		}
		if ((m_PlayerPos[0]) == 0)
		{
			if ((m_PlayerPos[2]) == 0)
			{
				m_ExposureDtTarget = 0.0;
				return;
			}
		}
		
		super.UndergroundCheck();
	}
};
#endif
