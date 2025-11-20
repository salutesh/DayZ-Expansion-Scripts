/**
 * ExpansionQuestNPCAIBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionQuestNPCAIBase
 * @brief		This class quest AI NPCs
 **/

#ifdef EXPANSIONMODAI
class ExpansionQuestNPCAIBase: eAINPCBase
{
	protected int m_QuestNPCID = -1;
	protected ref ExpansionQuestNPCData m_QuestNPCData;
	protected ParticleSource m_Expansion_QuestIndicator;

	void ExpansionQuestNPCAIBase()
	{
		SetMovementSpeedLimits(1.0);  //! Always walk
		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);
	}
	
	void ~ExpansionQuestNPCAIBase()
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
			if (!ExpansionQuestModule.GetQuestNPCAIByID(m_QuestNPCID))
				EXPrint(ToString() + " ::OnVariablesSynchronized - NPC ID: " + m_QuestNPCID);
			ExpansionQuestModule.AddQuestNPCAI(m_QuestNPCID, this);
		}

		if (!m_Expansion_QuestIndicator)
			ExpansionQuestModule.SetQuestNPCIndicator(m_QuestNPCID, this, m_Expansion_QuestIndicator);
	}

	override void Expansion_Init()
	{
		if (g_Game.IsServer())
		{
			m_eAI_FactionType = eAIFactionInvincibleObservers;
		}

		super.Expansion_Init();
	}

	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		
		ExpansionQuestModule.AddQuestNPCAI(id, this);
		
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

class ExpansionQuestNPCAIMirek: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIDenis: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIBoris: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAICyril: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIElias: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIFrancis: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIGuo: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIHassan: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIIndar: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIJose: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIKaito: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAILewis: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIManua: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAINiki: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIOliver: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIPeter: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIQuinn: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIRolf: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAISeth: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAITaiki: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAILinda: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIMaria: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIFrida: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIGabi: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIHelga: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIIrena: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIJudy: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIKeiko: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIEva: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAINaomi: ExpansionQuestNPCAIBase {};
class ExpansionQuestNPCAIBaty: ExpansionQuestNPCAIBase {};
#endif