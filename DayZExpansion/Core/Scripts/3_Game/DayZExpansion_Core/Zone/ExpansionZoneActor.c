/**
 * ExpansionZoneActor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionZoneActor
{
	static const int COUNT = ExpansionZoneType.Count;

	static ExpansionZoneActor s_Head = null;
	static ExpansionZoneActor s_Current = null;

	ExpansionZoneActor m_Next = null;
	ExpansionZoneActor m_Prev = null;

	bool m_Inside[COUNT];

	vector m_Position;

	void ExpansionZoneActor()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "ExpansionZoneActor");
#endif

		m_Next = s_Head;
		m_Prev = null;

		if (s_Head)
		{
			s_Head.m_Prev = this;
		}

		s_Head = this;
	}

	void ~ExpansionZoneActor()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "~ExpansionZoneActor");
#endif

		if (!g_Game)
			return;

		if (!s_Head)
			return;

		if (s_Head == this)
		{
			s_Head = m_Next;
		}

		if (s_Current == this)
		{
			s_Current = m_Next;
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

	static void UpdateAll(int max)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, "ExpansionZoneActor", "UpdateAll");
#endif

		int index = 0;
		bool passedHead = false;

		if (!s_Current)
		{
			s_Current = s_Head;
			passedHead = true;
		}

		for (int i = 0; i < COUNT; i++)
			ExpansionZone.s_InsideBuffer[i] = false;

		while (s_Current)
		{
			ExpansionZoneActor next = s_Current.m_Next;
			s_Current.OnUpdate();
			s_Current = next;

			if ((index++) > max)
				return;

			// small chance duplicate entries will be processed, but doing a proper check will prove to be ineffcient
			if (!passedHead && !s_Current)
			{
				s_Current = s_Head;
				passedHead = true;
			}
		}
	}

	void Update()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "Update");
#endif

		for (int i = 0; i < COUNT; i++)
			ExpansionZone.s_InsideBuffer[i] = false;

		OnUpdate();
	}

	protected void OnUpdate()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnUpdate");
#endif

		m_Position = GetPosition();

		ExpansionZone element = ExpansionZone.s_Head;
		while (element)
		{
			element.Check(m_Position);
			element = element.m_Next;
		}

		for (int i = 0; i < COUNT; i++)
		{
			if (ExpansionZone.s_InsideBuffer[i] && !m_Inside[i])
				OnEnterZone(i);
			else if (!ExpansionZone.s_InsideBuffer[i] && m_Inside[i])
				OnExitZone(i);

			m_Inside[i] = ExpansionZone.s_InsideBuffer[i];
			ExpansionZone.s_InsideBuffer[i] = false;
		}
	}

	void OnEnterZone(ExpansionZoneType type);

	void OnExitZone(ExpansionZoneType type);

	vector GetPosition()
	{
		return vector.Zero;
	}

	bool InZone()
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (m_Inside[i])
				return true;
		}

		return false;
	}
};

class ExpansionZoneActorT<Class T> : ExpansionZoneActor
{
	T m_Instance;

	void ExpansionZoneActorT(T instance)
	{
		m_Instance = instance;
	}

	override void OnEnterZone(ExpansionZoneType type)
	{
		m_Instance.OnEnterZone(type);
	}

	override void OnExitZone(ExpansionZoneType type)
	{
		m_Instance.OnExitZone(type);
	}

	override vector GetPosition()
	{
		return m_Instance.GetPosition();
	}
};

// Optimized for entities, reduces number of functions called in iterator
class ExpansionZoneEntity<Class T> : ExpansionZoneActor
{
	T m_Instance;

	void ExpansionZoneEntity(T instance)
	{
		m_Instance = instance;
	}

	override void OnUpdate()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnUpdate");
#endif

		m_Position = m_Instance.GetPosition();

		ExpansionZone element = ExpansionZone.s_Head;
		while (element)
		{
			element.Check(m_Position);
			element = element.m_Next;
		}

		for (int i = 0; i < COUNT; i++)
		{
			if (ExpansionZone.s_InsideBuffer[i] && !m_Inside[i])
				m_Instance.OnEnterZone(i);
			else if (!ExpansionZone.s_InsideBuffer[i] && m_Inside[i])
				m_Instance.OnExitZone(i);

			m_Inside[i] = ExpansionZone.s_InsideBuffer[i];
			ExpansionZone.s_InsideBuffer[i] = false;
		}
	}
};
