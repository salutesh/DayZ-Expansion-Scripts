
static ref ExpansionZone g_ExpansionZoneHead;

class ExpansionZone
{
	static const int COUNT = ExpansionZoneType.Count;

	static bool s_InsideBuffer[COUNT];

	int m_ID;
	ExpansionZoneType m_Type;

	ref ExpansionZone m_Next = null;
	ExpansionZone m_Prev = null;

	void ExpansionZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "ExpansionZone");
#endif

		m_Type = type;

		m_Next = g_ExpansionZoneHead;
		if (m_Next)
			m_Next.m_Prev = this;

		g_ExpansionZoneHead = this;
	}

	void ~ExpansionZone()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "~ExpansionZone");
#endif

		if (g_ExpansionZoneHead)
		{
			if (g_ExpansionZoneHead == this)
			{
				g_ExpansionZoneHead = m_Next;
			}

			if (m_Next)
			{
				m_Next.m_Prev = m_Prev;
			}

			if (m_Prev)
			{
				m_Prev.m_Next = m_Next;
			}
		}
	}

	void Check(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "Check");
#endif
	}

	string ToStr()
	{
		return ToString();
	}
};
