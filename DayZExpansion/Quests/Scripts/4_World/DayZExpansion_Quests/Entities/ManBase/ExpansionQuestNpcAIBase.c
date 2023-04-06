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
	static ref CF_DoublyLinkedNodes_WeakRef<ExpansionQuestNPCAIBase> s_Expansion_AllNPCs = new CF_DoublyLinkedNodes_WeakRef<ExpansionQuestNPCAIBase>();
	ref CF_DoublyLinkedNode_WeakRef<ExpansionQuestNPCAIBase> m_Expansion_NPCNode = new CF_DoublyLinkedNode_WeakRef<ExpansionQuestNPCAIBase>(this);
	
	protected int m_QuestNPCID = -1;
	protected ref ExpansionQuestNPCData m_QuestNPCData;

	void ExpansionQuestNPCAIBase()
	{
		m_Expansion_NPCNode = s_Expansion_AllNPCs.Add(this);

		SetMovementSpeedLimits(1.0);  //! Always walk

		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);
	}
	
	void ~ExpansionQuestNPCAIBase()
	{
		if (s_Expansion_AllNPCs)
			s_Expansion_AllNPCs.Remove(m_Expansion_NPCNode);
	}
	
	override void Expansion_Init()
	{
		if (GetGame().IsServer())
		{
			m_eAI_FactionType = eAIFactionInvincibleObservers;
		}

		super.Expansion_Init();
	}

	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
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