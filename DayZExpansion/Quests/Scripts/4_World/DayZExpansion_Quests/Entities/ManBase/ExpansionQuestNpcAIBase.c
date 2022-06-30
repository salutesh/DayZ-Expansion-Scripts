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

#ifdef ENFUSION_AI_PROJECT
/**@class		ExpansionQuestNPCAIBase
 * @brief		This class quest AI NPCs
 **/
#ifdef EXPANSIONMODAI
class ExpansionQuestNPCAIBase extends eAINPCBase
#else
class ExpansionQuestNPCAIBase extends eAIBase
#endif
{
	private int m_QuestNPCID = -1;
	private ref ExpansionQuestNPCData m_QuestNPCData;
	private int m_NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;
	private bool m_IsEmoteStatic = false;

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestNPCAIBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionFixPositionAndOrientation, 10000);
		}

#ifdef EXPANSIONMODAI
		SetMovementSpeedLimit(1.0);  //! Always walk
#endif

		RegisterNetSyncVariableInt("m_QuestNPCID", 1, int.MAX);
	}
	
	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionQuestNPCAIBase()
	{
		if (IsMissionHost())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionFixPositionAndOrientation);
			if (!m_IsEmoteStatic)
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionPlayEmote);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase DeferredInit
	// ------------------------------------------------------------
	override void DeferredInit()
    {
		super.DeferredInit();

	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print("-----------------------------------------------------------------------------------------");
		Print(ToString() + "::DeferredInit - NPC ID: " + m_QuestNPCID);
		Print(ToString() + "::DeferredInit - NPC type: " + GetType());
		Print("-----------------------------------------------------------------------------------------");
	#endif
    }

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase Constructor
	// ------------------------------------------------------------
#ifdef EXPANSIONMODAI
	override bool PlayerIsEnemy(EntityAI other)
#else
	override bool PlayerIsEnemy(PlayerBase other)
#endif
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase SetQuestNPCID
	// ------------------------------------------------------------
	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase GetQuestNPCID
	// ------------------------------------------------------------
	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase SetQuestNPCData
	// ------------------------------------------------------------
	void SetQuestNPCData(ExpansionQuestNPCData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase GetQuestNPCData
	// ------------------------------------------------------------
	ExpansionQuestNPCData GetQuestNPCData()
	{
		return m_QuestNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase PlayRandomEmote
	// ------------------------------------------------------------
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
	// ExpansionQuestNPCAIBase SetEmote
	// ------------------------------------------------------------
	void ExpansionSetEmote(int emoteID, bool isStatic = false)
	{
		m_NPCEmoteID = emoteID;
		m_IsEmoteStatic = isStatic;
		
		int emoteTime = Math.RandomInt(5000, 10000);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionPlayEmote, emoteTime, !m_IsEmoteStatic);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNPCAIBase PlayEmote
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
	// ExpansionQuestNPCAIBase FixPositionAndOrientation
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