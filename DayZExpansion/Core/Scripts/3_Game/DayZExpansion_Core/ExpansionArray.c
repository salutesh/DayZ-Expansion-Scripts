/**
 * ExpansionStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionArray<Class T>
{
	protected ref array<T> m_Elements;
	protected ref TIntArray m_Indexes;
	protected int m_LastSelectedIdx;

	void ExpansionArray(array<T> elements = null)
	{
		if (elements)
			m_Elements = elements;
		else
			m_Elements = new array<T>;
		m_Indexes = new TIntArray;
		m_LastSelectedIdx = -1;
	}

	void Clear()
	{
		m_Elements.Clear();
		m_Indexes.Clear();
		m_LastSelectedIdx = -1;
	}

	int Count()
	{
		return m_Elements.Count();
	}

	void Set(int n, T value)
	{
		m_Elements[n] = value;
	}

	T Get(int n)
	{
		return m_Elements[n];
	}

	int Insert(T element)
	{
		return m_Elements.Insert(element);
	}

	void InsertAll(notnull array<T> from)
	{
		foreach (T element: from)
		{
			Insert(element);
		}
	}

	int InsertAt(T element, int index)
	{
		return m_Elements.InsertAt(element, index);
	}

	//! Get a quasi-random element, preferring ones that haven't been picked yet
	T GetQuasiRandomElementAvoidRepetition()
	{
		//EXPrint(ToString() + "::GetQuasiRandomElementAvoidRepetition");
		if (!m_Indexes.Count())
		{
			//EXPrint(ToString() + "::GetQuasiRandomElementAvoidRepetition - resetting");
			for (int i = 0; i < m_Elements.Count(); i++)
			{
				if (m_LastSelectedIdx > 0 && i == m_LastSelectedIdx)
					continue;
				m_Indexes.Insert(i);
			}
		}

		m_LastSelectedIdx = m_Indexes.GetRandomIndex();
		T element = m_Elements[m_Indexes[m_LastSelectedIdx]];

		m_Indexes.Remove(m_LastSelectedIdx);

		return element;
	}

	T GetRandomElement()
	{
		return m_Elements.GetRandomElement();
	}

	void Each(ScriptCaller callback, bool reverse = false)
	{
		Each(m_Elements, callback, reverse);
	}

	static void Each(array<T> elements, ScriptCaller callback, bool reverse = false)
	{
		//! @note: Enforce garbage collects references to function arguments inside loops after the first iteration,
		//! so we have to assign a local variable and use that instead to avoid premature garbage collection
		ScriptCaller localCallback = callback;

		int count = elements.Count();
		int i, delta;

		if (reverse)
		{
			i = count - 1;
			delta = -1;
		}
		else
		{
			i = 0;
			delta = 1;
		}

		while (count--)
		{
			localCallback.Invoke(elements[i]);
			i += delta;
		}
	}

	static void RefCopy(array<ref T> from, array<ref T> to)
	{
		to.Clear();
		foreach (T value: from)
		{
			to.Insert(value);
		}
	}
}

class ExpansionArrayTools<Class T1, Class T2>
{
	/**
	 * @brief Insert all values from source array `from` down-casted to T2 into destination array `to`
	 * 
	 * @note No typechecking on values is done, if a value from source array cannot be cast to T2, null will be inserted instead
	 */
	static void InsertAll_DownCast(array<T1> from, array<T2> to)
	{
		typename t1 = T1;
		typename t2 = T2;
		if (!t2.IsInherited(t1))
		{
			Error("'" + t1 + "' cannot be down-casted to '" + t2 + "'");
			return;
		}

		foreach (T1 value1: from)
		{
			T2 value2;
			Class.CastTo(value2, value1);
			to.Insert(value2);
		}
	}

	/**
	 * @brief Insert all values from source array `from` up-casted to T2 into destination array `to`
	 */
	static void InsertAll_UpCast(array<T1> from, array<T2> to)
	{
		typename t1 = T1;
		typename t2 = T2;
		if (!t1.IsInherited(t2))
		{
			Error("'" + t1 + "' cannot be up-casted to '" + t2 + "'");
			return;
		}

		foreach (T1 value: from)
		{
			to.Insert(value);
		}
	}
}
