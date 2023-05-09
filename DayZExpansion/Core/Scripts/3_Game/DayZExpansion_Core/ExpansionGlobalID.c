/**
 * ExpansionGlobalID.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Similar to persistent ID, but not necessarily tied to storage
class ExpansionGlobalID
{
	//! @note {0, 0, 0, 0} is reserved
	static autoptr TIntArray s_NextID = {int.MIN, int.MIN, int.MIN, int.MIN};

	static bool s_IsMissionLoaded;

	int m_ID[4];

	[NonSerialized()]
	bool m_IsSet;

	void Acquire()
	{
	#ifndef SERVER
		Error("Can't Acquire() global ID on client!");
		return;
	#endif

		if (!s_IsMissionLoaded)
		{
			Error("Can't Acquire() global ID before mission is loaded!");
			return;
		}

		for (int i = 0; i < 4; i++)
			m_ID[i] = s_NextID[i];

		m_IsSet = true;

		IncrementNext(3);
	}

	void Set(TIntArray id)
	{
	#ifndef SERVER
		Error("Can't Set() global ID on client!");
		return;
	#endif

		if (s_IsMissionLoaded)
		{
			Error("Can't Set() global ID after mission is loaded!");
			return;
		}

		for (int i = 0; i < 4; i++)
			m_ID[i] = id[i];

		m_IsSet = true;

		UpdateNext();
	}

	bool IsEqual(int id[4])
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_ID[i] != id[i])
				return false;
		}

		return true;
	}

	bool IsEqual(TIntArray id)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_ID[i] != id[i])
				return false;
		}

		return true;
	}

	bool IsZero()
	{
		return m_ID[0] == 0 && m_ID[1] == 0 && m_ID[2] == 0 && m_ID[3] == 0;
	}

	#ifdef EXPANSION_MODSTORAGE
	void OnStoreSave(CF_ModStorage ctx)
	{
		for (int i = 0; i < 4; i++)
			ctx.Write(m_ID[i]);
	}

	bool OnStoreLoad(CF_ModStorage ctx)
	{
		for (int i = 0; i < 4; i++)
		{
			int id;
			if (!ctx.Read(id))
				return false;

			m_ID[i] = id;
		}

		if (IsZero())
		{
			EXPrint(ToString() + "::OnStoreLoad - WARNING: ID is zero!");
			return true;
		}

		m_IsSet = true;

		UpdateNext();

		return true;
	}
	#endif

	protected void UpdateNext()
	{
		int i;
		int same;
		bool inc;

		for (i = 0; i < 4; i++)
		{
			if (m_ID[i] == s_NextID[i])
			{
				same++;
			}
			else
			{
				if (m_ID[i] > s_NextID[i])
					inc = true;

				break;
			}
		}

		if (inc)
		{
			for (int j = i; j < 4; j++)
			{
				s_NextID[j] = m_ID[j];
			}
		}

		if (inc || same == 4)
			IncrementNext(3);
	}

	protected static void IncrementNext(int i)
	{
		if (s_NextID[i] < int.MAX)
		{
			s_NextID[i] = s_NextID[i] + 1;

			//! @note {0, 0, 0, 0} is reserved
			if (s_NextID[0] == 0 && s_NextID[1] == 0 && s_NextID[2] == 0 && s_NextID[3] == 0)
				s_NextID[3] = s_NextID[3] + 1;
		}
		else
		{
			if (i)
			{
				s_NextID[i] = 0;
				IncrementNext(i - 1);
			}
			else
			{
				ResetNext();
			}
		}
	}

	protected static void ResetNext()
	{
		for (int i = 0; i < 4; i++)
			s_NextID[i] = int.MIN;
	}

	string IDToHex()
	{
		string id;
		for (int i = 0; i < 4; i++)
			id += ExpansionStatic.IntToHex(m_ID[i]);
		return id;
	}

	string IDToString()
	{
		TStringArray ids = new TStringArray;
		for (int i = 0; i < 4; i++)
			ids.Insert(m_ID[i].ToString());
		return ExpansionString.JoinStrings(ids);
	}
}
