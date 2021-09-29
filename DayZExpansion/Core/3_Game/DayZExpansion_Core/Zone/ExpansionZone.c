
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
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZone::ExpansionZone start - ExpansionZone.COUNT: " + ExpansionZone.COUNT);
#endif

		m_Type = type;

		m_Next = g_ExpansionZoneHead;
		if (m_Next)
			m_Next.m_Prev = this;

		g_ExpansionZoneHead = this;

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZone::ExpansionZone end");
#endif
	}

	void ~ExpansionZone()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZone::~ExpansionZone start");
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

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZone::~ExpansionZone end");
#endif
	}

	void Check(vector position)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZone::Check");
#endif
	}

	string ToStr()
	{
		return ToString();
	}
};
