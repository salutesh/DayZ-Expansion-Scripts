/**
 * ExpansionQuestStaticObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionQuestStaticObject
 * @brief		This class handles static quest objects used to head-out or take-in quests
 **/
class ExpansionQuestStaticObject extends BuildingSuper
{
	private static ref set<ExpansionQuestStaticObject> m_allTraders = new set<ExpansionQuestStaticObject>;
	private int m_QuestNPCID = -1;
	private ref ExpansionQuestNpcData m_QuestNPCData;

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject Constructor
	// ------------------------------------------------------------
	void ExpansionQuestStaticObject()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		m_allTraders.Insert(this);

		RegisterNetSyncVariableInt("m_QuestNPCID");
	}

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject DeferredInit
	// ------------------------------------------------------------
	override void DeferredInit()
    {
		super.DeferredInit();

		SetSynchDirty();

	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print("-----------------------------------------------------------------------------------------");
		Print(ToString() + "::DeferredInit - NPC ID: " + m_QuestNPCID);
		Print(ToString() + "::DeferredInit - NPC type: " + GetType());
		Print("-----------------------------------------------------------------------------------------");
	#endif
    }

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject Destructor
	// ------------------------------------------------------------
	void ~ExpansionQuestStaticObject()
	{
		if (!GetGame())
			return;

		int idx = m_allTraders.Find(this);
		if (idx >= 0)
		{
			m_allTraders.Remove(idx);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject GetAll
	// ------------------------------------------------------------
	static set<ExpansionQuestStaticObject> GetAll()
	{
		return m_allTraders;
	}

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject SetQuestNPCID
	// ------------------------------------------------------------
	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject GetQuestNPCID
	// ------------------------------------------------------------
	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject SetQuestNPCData
	// ------------------------------------------------------------
	void SetQuestNPCData(ExpansionQuestNpcData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestStaticObject GetQuestNPCData
	// ------------------------------------------------------------
	ExpansionQuestNpcData GetQuestNPCData()
	{
		return m_QuestNPCData;
	}
};

class ExpansionQuestObjectBoard: ExpansionQuestStaticObject {};