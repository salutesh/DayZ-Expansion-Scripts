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

	int Count()
	{
		return m_Elements.Count();
	}

	int Insert(T element)
	{
		return m_Elements.Insert(element);
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

	static void RefCopy(array<ref T> from, array<ref T> to)
	{
		to.Clear();
		foreach (T value: from)
		{
			to.Insert(value);
		}
	}
}
