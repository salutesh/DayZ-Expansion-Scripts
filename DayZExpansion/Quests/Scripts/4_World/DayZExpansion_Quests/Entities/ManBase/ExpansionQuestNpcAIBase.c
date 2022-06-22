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
	private ref ExpansionQuestNpcData m_QuestNPCData;
	private int m_NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;
	private bool m_IsEmoteStatic = false;

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestNpcAIBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionFixPositionAndOrientation, 10000);
		}

#ifdef EXPANSIONMODAI
		SetMovementSpeedLimit(1.0);  //! Always walk
#endif

		RegisterNetSyncVariableInt("m_QuestNPCID");
	}
	
	void ~ExpansionQuestNpcAIBase()
	{
		if (IsMissionHost())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionFixPositionAndOrientation);
			if (!m_IsEmoteStatic)
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionPlayEmote);
		}
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
	void ExpansionPlayRandomEmote()
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
	void ExpansionSetEmote(int emoteID, bool isStatic = false)
	{
		m_NPCEmoteID = emoteID;
		m_IsEmoteStatic = isStatic;
		
		int emoteTime = Math.RandomInt(5000, 10000);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionPlayEmote, emoteTime, !m_IsEmoteStatic);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase PlayEmote
	// ------------------------------------------------------------
	void ExpansionPlayEmote()
	{
		EmoteManager emoteManager = GetEmoteManager();
		if (!emoteManager.IsEmotePlaying())
		{
			emoteManager.PlayEmote(m_NPCEmoteID);
			if (!m_IsEmoteStatic)
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(emoteManager.ServerRequestEmoteCancel, 2000);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcAIBase FixPositionAndOrientation
	// ------------------------------------------------------------
	void ExpansionFixPositionAndOrientation()
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
class ExpansionQuestNpcAIRolf: ExpansionQuestNpcAIBase {};
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