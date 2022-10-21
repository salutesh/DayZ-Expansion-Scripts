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

	// ------------------------------------------------------------
	// ExpansionQuestNPCBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestNPCBase()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCBase DeferredInit
	// ------------------------------------------------------------
	override void DeferredInit()
    {
		super.DeferredInit();

	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		CF_Log.Debug("-----------------------------------------------------------------------------------------");
		CF_Log.Debug(ToString() + "::DeferredInit - NPC ID: " + m_QuestNPCID);
		CF_Log.Debug(ToString() + "::DeferredInit - NPC type: " + GetType());
		CF_Log.Debug("-----------------------------------------------------------------------------------------");
	#endif
    }

	// ------------------------------------------------------------
	// ExpansionQuestNPCBase SetQuestNPCID
	// ------------------------------------------------------------
	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCBase GetQuestNPCID
	// ------------------------------------------------------------
	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCBase SetQuestNPCData
	// ------------------------------------------------------------
	void SetQuestNPCData(ExpansionQuestNPCData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCBase GetQuestNPCData
	// ------------------------------------------------------------
	ExpansionQuestNPCData GetQuestNPCData()
	{
		return m_QuestNPCData;
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