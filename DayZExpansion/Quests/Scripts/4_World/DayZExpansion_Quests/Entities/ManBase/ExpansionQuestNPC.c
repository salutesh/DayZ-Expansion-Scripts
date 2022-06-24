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

class ExpansionQuestNpcBase extends ExpansionNPCBase
{
	private int m_QuestNPCID = -1;
	private ref ExpansionQuestNpcData m_QuestNPCData;

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestNpcBase()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		RegisterNetSyncVariableInt("m_QuestNPCID");
	}

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
	// ExpansionQuestNpcBase SetQuestNPCID
	// ------------------------------------------------------------
	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase GetQuestNPCID
	// ------------------------------------------------------------
	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase SetQuestNPCData
	// ------------------------------------------------------------
	void SetQuestNPCData(ExpansionQuestNpcData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase GetQuestNPCData
	// ------------------------------------------------------------
	ExpansionQuestNpcData GetQuestNPCData()
	{
		return m_QuestNPCData;
	}
};

class ExpansionNpcMirek: ExpansionQuestNpcBase {};
class ExpansionNpcDenis: ExpansionQuestNpcBase {};
class ExpansionNpcBoris: ExpansionQuestNpcBase {};
class ExpansionNpcCyril: ExpansionQuestNpcBase {};
class ExpansionNpcElias: ExpansionQuestNpcBase {};
class ExpansionNpcFrancis: ExpansionQuestNpcBase {};
class ExpansionNpcGuo: ExpansionQuestNpcBase {};
class ExpansionNpcHassan: ExpansionQuestNpcBase {};
class ExpansionNpcIndar: ExpansionQuestNpcBase {};
class ExpansionNpcJose: ExpansionQuestNpcBase {};
class ExpansionNpcKaito: ExpansionQuestNpcBase {};
class ExpansionNpcLewis: ExpansionQuestNpcBase {};
class ExpansionNpcManua: ExpansionQuestNpcBase {};
class ExpansionNpcNiki: ExpansionQuestNpcBase {};
class ExpansionNpcOliver: ExpansionQuestNpcBase {};
class ExpansionNpcPeter: ExpansionQuestNpcBase {};
class ExpansionNpcQuinn: ExpansionQuestNpcBase {};
class ExpansionNpcRolf: ExpansionQuestNpcBase  {};
class ExpansionNpcSeth: ExpansionQuestNpcBase {};
class ExpansionNpcTaiki: ExpansionQuestNpcBase {};
class ExpansionNpcLinda: ExpansionQuestNpcBase {};
class ExpansionNpcMaria: ExpansionQuestNpcBase {};
class ExpansionNpcFrida: ExpansionQuestNpcBase {};
class ExpansionNpcGabi: ExpansionQuestNpcBase {};
class ExpansionNpcHelga: ExpansionQuestNpcBase {};
class ExpansionNpcIrena: ExpansionQuestNpcBase {};
class ExpansionNpcJudy: ExpansionQuestNpcBase {};
class ExpansionNpcKeiko: ExpansionQuestNpcBase {};
class ExpansionNpcEva: ExpansionQuestNpcBase {};
class ExpansionNpcNaomi: ExpansionQuestNpcBase {};
class ExpansionNpcBaty: ExpansionQuestNpcBase {};