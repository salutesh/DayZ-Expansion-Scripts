/**
 * ExpansionQuestNpc.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestNPCBase: ExpansionNPCBase
{
	protected int m_QuestNPCID = -1;
	protected ref ExpansionQuestNPCData m_QuestNPCData;
	protected ParticleSource m_Expansion_QuestIndicator;
	
	void ExpansionQuestNPCBase()
	{
		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);
	}

	void ~ExpansionQuestNPCBase()
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
			if (!ExpansionQuestModule.GetQuestNPCByID(m_QuestNPCID))
				EXPrint(ToString() + " ::OnVariablesSynchronized - NPC ID: " + m_QuestNPCID);
			ExpansionQuestModule.AddQuestNPC(m_QuestNPCID, this);
		}

		if (!m_Expansion_QuestIndicator)
			ExpansionQuestModule.SetQuestNPCIndicator(m_QuestNPCID, this, m_Expansion_QuestIndicator);
	}

	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		
		ExpansionQuestModule.AddQuestNPC(id, this);
		
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

class ExpansionQuestNPCMirek: ExpansionQuestNPCBase {};
class ExpansionQuestNPCDenis: ExpansionQuestNPCBase {};
class ExpansionQuestNPCBoris: ExpansionQuestNPCBase {};
class ExpansionQuestNPCCyril: ExpansionQuestNPCBase {};
class ExpansionQuestNPCElias: ExpansionQuestNPCBase {};
class ExpansionQuestNPCFrancis: ExpansionQuestNPCBase {};
class ExpansionQuestNPCGuo: ExpansionQuestNPCBase {};
class ExpansionQuestNPCHassan: ExpansionQuestNPCBase {};
class ExpansionQuestNPCIndar: ExpansionQuestNPCBase {};
class ExpansionQuestNPCJose: ExpansionQuestNPCBase {};
class ExpansionQuestNPCKaito: ExpansionQuestNPCBase {};
class ExpansionQuestNPCLewis: ExpansionQuestNPCBase {};
class ExpansionQuestNPCManua: ExpansionQuestNPCBase {};
class ExpansionQuestNPCNiki: ExpansionQuestNPCBase {};
class ExpansionQuestNPCOliver: ExpansionQuestNPCBase {};
class ExpansionQuestNPCPeter: ExpansionQuestNPCBase {};
class ExpansionQuestNPCQuinn: ExpansionQuestNPCBase {};
class ExpansionQuestNPCRolf: ExpansionQuestNPCBase  {};
class ExpansionQuestNPCSeth: ExpansionQuestNPCBase {};
class ExpansionQuestNPCTaiki: ExpansionQuestNPCBase {};
class ExpansionQuestNPCLinda: ExpansionQuestNPCBase {};
class ExpansionQuestNPCMaria: ExpansionQuestNPCBase {};
class ExpansionQuestNPCFrida: ExpansionQuestNPCBase {};
class ExpansionQuestNPCGabi: ExpansionQuestNPCBase {};
class ExpansionQuestNPCHelga: ExpansionQuestNPCBase {};
class ExpansionQuestNPCIrena: ExpansionQuestNPCBase {};
class ExpansionQuestNPCJudy: ExpansionQuestNPCBase {};
class ExpansionQuestNPCKeiko: ExpansionQuestNPCBase {};
class ExpansionQuestNPCEva: ExpansionQuestNPCBase {};
class ExpansionQuestNPCNaomi: ExpansionQuestNPCBase {};
class ExpansionQuestNPCBaty: ExpansionQuestNPCBase {};