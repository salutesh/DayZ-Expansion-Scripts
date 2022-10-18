/**
 * ExpansionQuestObjectSet.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectSet
{
	protected int m_QuestID;
	protected bool m_Spawned;
	protected ref ExpansionObjectSet m_ObjectsSet;

	void ~ExpansionQuestObjectSet()
	{
		if (m_Spawned && m_ObjectsSet)
			m_ObjectsSet.Delete();
	}

	void SetQuestID(int questID)
	{
		m_QuestID = questID;
	}

	int GetQuestID()
	{
		return m_QuestID;
	}

	void SetObjectSetFileName(string path, string fileName)
	{
		m_ObjectsSet = new ExpansionObjectSet(path, fileName);
	}

	void Spawn()
	{
		if (!m_Spawned && m_ObjectsSet)
		{
			m_ObjectsSet.SpawnObjects();
			m_Spawned = true;
		}
	}

	void Delete()
	{
		if (m_Spawned && m_ObjectsSet)
		{
			m_ObjectsSet.Delete();
			m_Spawned = false;
		}
	}

	bool IsSpawned()
	{
		return m_Spawned;
	}

	ExpansionObjectSet GetObjectsSet()
	{
		return m_ObjectsSet;
	}
};