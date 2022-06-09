/**
 * ExpansionQuestNpcAIBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef ENFUSION_AI_PROJECT
/**@class		ExpansionQuestNpcAIBase
 * @brief		This class quest AI NPCs
 **/
class ExpansionQuestNpcAIBase extends eAIBase
{
	private int m_QuestNPCID = -1;
	private ExpansionQuestNpcData m_QuestNPCData;
	private int m_NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestNpcAIBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}
		
#ifdef EXPANSIONMODAI
		SetMovementSpeedLimit(1.0);  //! Always walk
#endif

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
	// ExpansionQuestNpcAIBase Constructor
	// ------------------------------------------------------------
#ifdef EXPANSIONMODAI
	override bool PlayerIsEnemy(EntityAI other)
#else
	override bool PlayerIsEnemy(PlayerBase other)
#endif
	{
		if ( !IsInSafeZone() )
			return false;

		return super.PlayerIsEnemy(other);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase SetQuestNPCID
	// ------------------------------------------------------------
	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase GetQuestNPCID
	// ------------------------------------------------------------
	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase SetQuestNPCData
	// ------------------------------------------------------------
	void SetQuestNPCData(ExpansionQuestNpcData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase GetQuestNPCData
	// ------------------------------------------------------------
	ExpansionQuestNpcData GetQuestNPCData()
	{
		return m_QuestNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase PlayRandomEmote
	// ------------------------------------------------------------
#ifdef EXPANSIONMODAI
	void PlayRandomEmote()
	{
		EmoteManager emoteManager = GetEmoteManager();
		if (!emoteManager.IsEmotePlaying())
		{
			emoteManager.PlayEmote(EmoteConstants.ID_EMOTE_WATCHING);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(emoteManager.ServerRequestEmoteCancel, 2000);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase SetEmote
	// ------------------------------------------------------------
	void SetEmote(int emoteID)
	{
		m_NPCEmoteID = emoteID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase PlayEmote
	// ------------------------------------------------------------
	void PlayEmote()
	{
		EmoteManager emoteManager = GetEmoteManager();
		if (!emoteManager.IsEmotePlaying())
		{
			emoteManager.PlayEmote(m_NPCEmoteID);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(emoteManager.ServerRequestEmoteCancel, 2000);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase FixPositionAndOrientation
	// ------------------------------------------------------------
	void FixPositionAndOrientation()
	{
		if (m_QuestNPCData)
		{
			if (GetPosition() != m_QuestNPCData.GetPosition())
				SetPosition(m_QuestNPCData.GetPosition());

			if (GetOrientation() != m_QuestNPCData.GetOrientation())
				SetOrientation(m_QuestNPCData.GetOrientation());
		}
	}
#endif
};

class ExpansionQuestNpcAIMirek: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIDenis: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIBoris: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAICyril: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIElias: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIFrancis: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIGuo: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIHassan: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIIndar: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIJose: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIKaito: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAILewis: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIManua: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAINiki: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIOliver: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIPeter: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIQuinn: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIRolf: ExpansionQuestNpcAIBase  {};
class ExpansionQuestNpcAISeth: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAITaiki: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAILinda: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIMaria: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIFrida: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIGabi: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIHelga: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIIrena: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIJudy: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIKeiko: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIEva: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAINaomi: ExpansionQuestNpcAIBase {};
class ExpansionQuestNpcAIBaty: ExpansionQuestNpcAIBase {};
#endif