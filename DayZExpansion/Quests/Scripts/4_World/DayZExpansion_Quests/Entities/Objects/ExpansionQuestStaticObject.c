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
	protected const int PARTICLE_QUEST_MARKER = ParticleList.EXPANSION_PARTICLE_QUEST_MARKER;

	protected int m_QuestNPCID = -1;
	protected ref ExpansionQuestNPCData m_QuestNPCData;
	protected Particle m_QuestParticle;

	void ExpansionQuestStaticObject()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	}
	
	/*void ~ExpansionQuestStaticObject()
	{
		if (m_QuestNPCID > -1)
			ExpansionQuestModule.RemoveStaticQuestObject(m_QuestNPCID);
	}*/

	override void DeferredInit()
    {
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.DeferredInit();

	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, "-----------------------------------------------------------------------------------------");
		EXTrace.Print(EXTrace.QUESTS, this, "NPC ID: " + m_QuestNPCID);
		EXTrace.Print(EXTrace.QUESTS, this, "NPC type: " + GetType());
		EXTrace.Print(EXTrace.QUESTS, this, "-----------------------------------------------------------------------------------------");
	#endif
		
		if (GetGame().IsClient() && m_QuestNPCID > -1)
			ExpansionQuestModule.AddStaticQuestObject(m_QuestNPCID, this);
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
	
	void UpdateQuestMarker(bool show, int state = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "::UpdateQuestMarker - Show: " + show.ToString() + " | NPC ID: " + m_QuestNPCID + " | State: " + state.ToString());

		if ( GetGame() && GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{	
			if ( show )
			{
				if (m_QuestParticle)
					m_QuestParticle.Stop();

				m_QuestParticle = ParticleManager.GetInstance().PlayOnObject(PARTICLE_QUEST_MARKER, this , "0 0.7 0", "0 0 0", true);
				
				if (state == 0)
				{
					ShowQuestionmark(true);
					ShowExclamantionmark(false);
				}
				else if (state == 1)
				{
					ShowQuestionmark(false);
					ShowExclamantionmark(true);
				}
			} 
			else if ( !show && m_QuestParticle )
			{
				m_QuestParticle.Stop();
			}
		}
	}
	
	void ShowQuestionmark(bool show)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_QuestParticle)
			return;
		
		if (show)
		{
			m_QuestParticle.SetParameter(0, EmitorParam.LIFETIME, 4);
			m_QuestParticle.SetParameter(0, EmitorParam.REPEAT, 1);
			m_QuestParticle.SetParameter(0, EmitorParam.SIZE, 0.3);
		}
		else
		{
			m_QuestParticle.SetParameter(0, EmitorParam.LIFETIME, 0);
			m_QuestParticle.SetParameter(0, EmitorParam.REPEAT, 0);
			m_QuestParticle.SetParameter(0, EmitorParam.SIZE, 0);
		}
	}
	
	void ShowExclamantionmark(bool show)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_QuestParticle)
			return;
		
		if (show)
		{
			m_QuestParticle.SetParameter(1, EmitorParam.LIFETIME, 4);
			m_QuestParticle.SetParameter(1, EmitorParam.REPEAT, 1);
			m_QuestParticle.SetParameter(1, EmitorParam.SIZE, 0.3);
		}
		else
		{
			m_QuestParticle.SetParameter(1, EmitorParam.LIFETIME, 0);
			m_QuestParticle.SetParameter(1, EmitorParam.REPEAT, 0);
			m_QuestParticle.SetParameter(1, EmitorParam.SIZE, 0);
		
		}
	}
};

class ExpansionQuestObjectBoard: ExpansionQuestStaticObject {};
class ExpansionQuestBoardSmall: ExpansionQuestStaticObject {};
class ExpansionQuestBoardLarge: ExpansionQuestStaticObject {};
class ExpansionQuestObjectPaper: ExpansionQuestStaticObject {};
class ExpansionQuestObjectLocker: ExpansionQuestStaticObject {};