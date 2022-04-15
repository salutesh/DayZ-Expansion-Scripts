/*
class eAIScriptJunctureData
{
	autoptr ScriptReadWriteContext m_RWContext;
	autoptr ScriptJunctureData m_Juncture;

	ParamsWriteContext m_Writer;

	bool m_IsAI;
	DayZPlayerImplement m_Player;

	void eAIScriptJunctureData(DayZPlayer pPlayer)
	{
		Class.CastTo(m_Player, pPlayer);

		m_IsAI = m_Player.IsAI();
		if (m_IsAI)
		{
			m_RWContext = new ScriptReadWriteContext();
			m_Writer = m_RWContext.GetWriteContext();
		}
		else
		{
			m_Juncture = new ScriptJunctureData();
			m_Writer = m_Juncture;
		}
	}

	void Write(bool value)
	{
		m_Writer.Write(value);
	}

	void Write(int value)
	{
		m_Writer.Write(value);
	}

	void Write(float value)
	{
		m_Writer.Write(value);
	}

	void Write(string value)
	{
		m_Writer.Write(value);
	}

	void Write(vector value)
	{
		m_Writer.Write(value);
	}

	void Write(Class value)
	{
		m_Writer.Write(value);
	}

	void Send(int id)
	{
		Print(m_Player);
		Print(m_IsAI);
		Print(id);
		if (m_IsAI)
		{
			m_Player.OnSyncJuncture(id, m_RWContext.GetReadContext());
		}
		else
		{
			m_Player.SendSyncJuncture(id, m_Juncture);
		}
	}
};
*/