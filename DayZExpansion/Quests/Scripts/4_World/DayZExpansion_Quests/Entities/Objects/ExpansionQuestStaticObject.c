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
class ExpansionQuestStaticObject: ExpansionStaticObjectBase
{
	protected int m_QuestNPCID = -1;
	protected ref ExpansionQuestNPCData m_QuestNPCData;
	protected ParticleSource m_Expansion_QuestIndicator;

	void ExpansionQuestStaticObject()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	}

	void ~ExpansionQuestStaticObject()
	{
		if (m_Expansion_QuestIndicator)
		{
			m_Expansion_QuestIndicator.StopParticle();
			m_Expansion_QuestIndicator = null;
		}
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_QuestNPCID > -1)
		{
			if (!ExpansionQuestModule.GetQuestObjectByID(m_QuestNPCID))
				EXPrint(ToString() + "::OnVariablesSynchronized - NPC ID: " + m_QuestNPCID);
			ExpansionQuestModule.AddStaticQuestObject(m_QuestNPCID, this);
		}

		if (!m_Expansion_QuestIndicator)
			ExpansionQuestModule.SetQuestNPCIndicator(m_QuestNPCID, this, m_Expansion_QuestIndicator);
	}

	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		
		ExpansionQuestModule.AddStaticQuestObject(id, this);
		
		SetSynchDirty();
	}

	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	void SetQuestNPCData(ExpansionQuestNPCData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	ExpansionQuestNPCData GetQuestNPCData()
	{
		return m_QuestNPCData;
	}

	void Expansion_SetQuestIndicator(ExpansionQuestIndicatorState state)
	{
		ExpansionQuestModule.SetQuestNPCIndicator(this, m_Expansion_QuestIndicator, state);
	}

	void Expansion_SetQuestIndicator(ParticleSource particle)
	{
		m_Expansion_QuestIndicator = particle;
	}
};

class ExpansionQuestObjectBoard: ExpansionQuestStaticObject {};
class ExpansionQuestBoardSmall: ExpansionQuestStaticObject {};
class ExpansionQuestBoardLarge: ExpansionQuestStaticObject {};
class ExpansionQuestObjectPaper: ExpansionQuestStaticObject {};
class ExpansionQuestObjectLocker: ExpansionQuestStaticObject {};